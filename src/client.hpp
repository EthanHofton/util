#ifndef UTIL_CLIENT_HPP
#define UTIL_CLIENT_HPP

#include <functional>
#include <future>
#include <iostream>
#include <event.hpp>
#include <logger.hpp>

#include <arpa/inet.h>
#include <stdexcept>
#include <sys/socket.h>
#include <unistd.h>

namespace util {

/**
* TODO: fill out event fields
*/

enum class CLIENT_EVENTS {
    // * logging events
    before_log,
    after_log,

    // * connect events
    server_connected,
    server_disconccected,

    // * send/recv events
    client_send,
    client_receve,

    // * intialzieation/shutdown events
    client_finished_listen,
};

class client_before_log_event : public util::event<CLIENT_EVENTS> {
public:

    EVENT_CLASS_TYPE(CLIENT_EVENTS, before_log);

};

class client_after_log_event : public util::event<CLIENT_EVENTS> {
public:

    EVENT_CLASS_TYPE(CLIENT_EVENTS, after_log);

};

class client_finished_listen_event : public util::event<CLIENT_EVENTS> {
public:

    EVENT_CLASS_TYPE(CLIENT_EVENTS, client_finished_listen);

};

class client_connect_event : public util::event<CLIENT_EVENTS> {
public:

    EVENT_CLASS_TYPE(CLIENT_EVENTS, server_connected);

};

class client_disconnect_event : public util::event<CLIENT_EVENTS> {
public:

    EVENT_CLASS_TYPE(CLIENT_EVENTS, server_disconccected);

};

class client_send_event : public util::event<CLIENT_EVENTS> {
public:

    EVENT_CLASS_TYPE(CLIENT_EVENTS, client_send);

};

class client_receve_event : public util::event<CLIENT_EVENTS> {
public:

    EVENT_CLASS_TYPE(CLIENT_EVENTS, client_receve);

};

class client {

    using eventFn = std::function<void(util::event<CLIENT_EVENTS>&)>;

public:

    client(const std::string& t_ip, const int& t_port, eventFn t_eventFn, util::logger& t_logger) : m_ip(t_ip), m_port(t_port), m_logger(t_logger), m_eventFunction(t_eventFn) {
        m_alive = false;

        // * create server socket and check for error
        if ((m_serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            logMessage(util::LOGGER_LEVEL::CRITICAL, "failed to create server socket");
            logMessage(util::LOGGER_LEVEL::ERROR, util::fmt("socket error: {}", strerror(errno)));
            throw std::runtime_error("failed to create server socket");
        }
        m_closed = false;

        // * define server port and address
        sockaddr_in serveraddr;
        serveraddr.sin_port = htons(m_port);
        serveraddr.sin_family = AF_INET;

        // * conver IPv4 to IPv6 and set in sockaddr_in struct and check for error
        if (inet_pton(AF_INET, m_ip.c_str(), &serveraddr.sin_addr) <= 0) {
            logMessage(util::LOGGER_LEVEL::CRITICAL, "server IP address not valid");
            logMessage(util::LOGGER_LEVEL::ERROR, util::fmt("inet_pron error: {}", strerror(errno)));
            throw std::runtime_error("server IP address not valid");
        }

        // * log that the clinet is about to connect to server
        logMessage(util::LOGGER_LEVEL::INFO, util::fmt("connecting to server at ip {} on port {}", m_ip, m_port));

        // * connect to server
        if ((m_clientFd = connect(m_serverFd, (sockaddr*)&serveraddr, sizeof(serveraddr))) < 0) {
            logMessage(util::LOGGER_LEVEL::CRITICAL, "failed to connect to server");
            logMessage(util::LOGGER_LEVEL::ERROR, util::fmt("connect error: {}", strerror(errno)));
            throw std::runtime_error("failed to connect to server");
        }

        // * server connected evnet and dispatch
        client_connect_event connEv;
        m_eventFunction(connEv);

        // * log client is connected
        logMessage(util::LOGGER_LEVEL::INFO, util::fmt("connected to server at ip {} on port {}", m_ip, m_port));

        // * set active to true as server is initalized and launch the listen thread
        logMessage(util::LOGGER_LEVEL::INFO, "starting client listen thread...");
        m_alive = true;
        m_clientThread = std::async(std::launch::async, &client::clientListen, this);
    }

    void disconnect() {
        handleDisconnect();
    }

    void sendServer(const std::string& t_message) {
        handleSend(t_message.c_str(), strlen(t_message.c_str()));
    }

    void sendServer(char* t_buff, const size_t& t_buffSize) {
        handleSend(t_buff, t_buffSize);
    }

    ~client() {
        handleDisconnect();
    }

    bool alive() const { return m_alive; }

private:

    void handleSend(const char* t_data, const size_t& t_dataSize) {
        // * get message in human readable format to log
        std::string message(t_data, t_dataSize);
        // * remove newline to not mess up terminal output
        message.erase(std::remove(message.begin(), message.end(), '\n'), message.cend());

        // * log the message was sent
        logMessage(util::LOGGER_LEVEL::INFO, util::fmt("sending server (ip {}, port {}) data: {}", m_ip, m_port, message));

        // * create send message event and dispatch to event function
        client_send_event sendEv;
        m_eventFunction(sendEv);

        // * send the bytes to the client
        send(m_serverFd, (void*)t_data, t_dataSize, 0);
    }


    void clientListen() {
        logMessage(util::LOGGER_LEVEL::INFO, "client listining for activity");

        // * setup file discriptor set and timeout
        fd_set readfds;
        timeval tv;
        tv.tv_sec = 3;
        tv.tv_usec = 50;

        while (m_alive) {
            // * clear the file discriptor set
            FD_ZERO(&readfds);
            // * add the server socket to the file discriptor set
            FD_SET(m_serverFd, &readfds);

            // * get the activity of the set
            int activity = select(m_serverFd + 1, &readfds, NULL, NULL, &tv);

            // * negative activity means error, deal with accordingly
            if (activity < 0) {

            }

            // * zero activity means timeout (or no activity)
            if (activity == 0) {
                continue;
            }

            // * check for activity of the socket
            if (FD_ISSET(m_serverFd, &readfds)) {
                if (handleServerActivity()) {
                    m_alive = false;
                    continue;
                }
            }
        }

        client_finished_listen_event finishedListenEv;
        m_eventFunction(finishedListenEv);
        logMessage(util::LOGGER_LEVEL::INFO, "client stopped listinig for activity");
    }

    bool handleServerActivity() {
        constexpr int bufSize = 1024;
        char buf[bufSize];
        size_t size;

        if ((size = read(m_serverFd, buf, bufSize)) == 0) {
            // * server disconnected
            return true;
        }

        handleRead(buf, size);
        return false;
    }

    void handleRead(char *t_buf, const size_t& t_size) {
        // * conver the buffer to a string
        std::string message(t_buf, t_size);
        // * remove newline to not mess up terminal output
        message.erase(std::remove(message.begin(), message.end(), '\n'), message.cend());

        // * log read
        logMessage(util::LOGGER_LEVEL::INFO, util::fmt("server (ip {}, port {}) sent: {}", m_ip, m_port, message));

        // * create and dispatch read event
        client_receve_event readEv; 
        m_eventFunction(readEv);
    }


    void logMessage(const util::LOGGER_LEVEL& t_level, const std::string& t_message) const {
        // * creater the before log event and send it to the event function
        client_before_log_event beforeLog;
        m_eventFunction(beforeLog);

        // * log the message to ALL sinks
        m_logger.log(t_message, t_level);

        // * create the after log event and send it to the event function
        client_after_log_event afterLog;
        m_eventFunction(afterLog);
    }

    void handleDisconnect() {
        using namespace std::chrono_literals;
        if (m_clientThread.wait_for(0ms) != std::future_status::ready) {
            logMessage(util::LOGGER_LEVEL::INFO, "stopping client listen thread...");
            // * set active to false and wait for listen thread to join back
            m_alive = false;
            m_clientThread.wait();
        }

        if (!m_closed) {
            // * log client disconnect
            logMessage(util::LOGGER_LEVEL::INFO, util::fmt("client disconnecting from server at ip {} on port {}", m_ip, m_port));

            // * close the client server connection
            close(m_serverFd);
            m_closed = true;

            // * client shutdown and dispach event
            client_disconnect_event disconEv;
            m_eventFunction(disconEv);

            logMessage(util::LOGGER_LEVEL::INFO, "client shutdown");
        }
    }

    int m_serverFd, m_clientFd;
    int m_port;
    std::string m_ip;
    util::logger& m_logger;
    eventFn m_eventFunction;
    bool m_alive;
    std::future<void> m_clientThread;
    std::future<void> m_shutdownThread;
    bool m_closed;
};

}

#endif
