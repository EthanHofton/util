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
* @brief cleint events enum class. stores all the client event types
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
    client_start_listen,
    client_finished_listen,
};

/**
* @brief cleint before log event class. is dispached before a log message
*/
class client_before_log_event : public util::event<CLIENT_EVENTS> {
public:

    EVENT_CLASS_TYPE(CLIENT_EVENTS, before_log);

};

/**
* @brief client after log event. dispached after a message is logged
*/
class client_after_log_event : public util::event<CLIENT_EVENTS> {
public:

    EVENT_CLASS_TYPE(CLIENT_EVENTS, after_log);

};

/**
* @brief client listen event. is used to generalize events that invole the client listen thread
*/
class client_listen_event : public util::event<CLIENT_EVENTS> {

};

/**
* @brief client finished listen event. is dispatched once the client listen thread is finished
*/
class client_finished_listen_event : public client_listen_event {
public:

    EVENT_CLASS_TYPE(CLIENT_EVENTS, client_finished_listen);

};

/**
* @brief client start listen evnet. is dispached once the client starts a new listen thread
*/
class client_start_listen_event : public client_listen_event {
public:

    EVENT_CLASS_TYPE(CLIENT_EVENTS, client_finished_listen);

};

/**
* @brief client connect event. is dispached when the client connects to the server
*/
class client_connect_event : public util::event<CLIENT_EVENTS> {
public:

    client_connect_event(const int& t_serverFd, const int& t_clientFd, const std::string& t_ip, const int& t_port) 
        : m_serverFd(t_serverFd), m_clientFd(t_clientFd), m_ip(t_ip), m_port(t_port) {}

    int getServerFd() const { return m_serverFd; }
    int getClientFd() const { return m_clientFd; }
    int getPort() const { return m_port; }
    std::string getIp() const { return m_ip; }

    EVENT_CLASS_TYPE(CLIENT_EVENTS, server_connected);

private:

    int m_serverFd;
    int m_clientFd;
    std::string m_ip;
    int m_port;

};

/**
* @brief client disconnect event. is dispached once a client disconnects for the server
*/
class client_disconnect_event : public util::event<CLIENT_EVENTS> {
public:

    EVENT_CLASS_TYPE(CLIENT_EVENTS, server_disconccected);

};

/**
    * @brief client communication evnet. is used to generalize events that involve comunnivation (e.g send, recv)
    */
class client_communication_event : public util::event<CLIENT_EVENTS> {

public:
    
    /**
    * @brief client communication event constructor. copied the data buffer
    *
    * @param t_serverFd the server file discriptor
    * @param t_data the data that was comminicated
    * @param t_dataSize the size of the data buffer
    */
    client_communication_event(const int& t_serverFd, const char* t_data, const size_t& t_dataSize) : m_serverFd(t_serverFd), m_dataSize(t_dataSize) {
        m_data = new char[m_dataSize];
        memcpy(m_data, t_data, (size_t)m_dataSize);
    }

    /**
    * @brief client comminication event disctrictor, free the memory that was created
    */
    ~client_communication_event() {
        delete[] m_data;
    }

    /**
    * @brief return the data buffer as a string
    *
    * @return the data buffer as a string
    */
    std::string getDataAsString() const { return std::string(m_data, m_dataSize); }
    /**
    * @brief get the data as a cosnt char *
    *
    * @return the data buffer
    */
    const char* getData() const { return m_data; }
    /**
    * @brief get the size of the data buffer 
    *
    * @return the size of the data buffer
    */
    size_t getDataSize() const { return m_dataSize; }

protected:

    /**
    * @brief the server file discriptor
    */
    int m_serverFd;
    /**
    * @brief the data that was comminicated
    */
    char* m_data;
    /**
    * @brief the size of the data buffer
    */
    size_t m_dataSize;
};


/**
* @brief client send event. dispatched when the client sends a message to the server
*/
class client_send_event : public client_communication_event {
public:

    /**
    * @brief client send event consturtor
    *
    * @param t_serverFd the server file disctiptor
    * @param t_data the data that was sent
    * @param t_dataSize the size of the data that was sent
    */
    client_send_event(const int& t_serverFd, const char* t_data, const size_t& t_dataSize) : client_communication_event(t_serverFd, t_data, t_dataSize) {}

    EVENT_CLASS_TYPE(CLIENT_EVENTS, client_send);

};

/**
* @brief client receve event. dispatched when the client receves data from the server
*/
class client_receve_event : public client_communication_event {
public:

    /**
    * @brief client receve event constuctor
    *
    * @param t_serverFd the server file desciptor
    * @param t_data the data that was receved
    * @param t_dataSize the size of the data that was sent
    */
    client_receve_event(const int& t_serverFd, const char* t_data, const size_t& t_dataSize) : client_communication_event(t_serverFd, t_data, t_dataSize) {}

    EVENT_CLASS_TYPE(CLIENT_EVENTS, client_receve);

};

/**
* @brief client class. uses RIAA. when the client is created, it connects to the server and starts the listen thread. when it is deleted it disconnects and 
* distroies the socket. However, the reconnect and disconnect will allow to do this early (the methods have been set up so that if disconnect/reconnect is called twice, it is ignored)
*/
class client {

    /**
    * @brief the event function that is called when an event needs to be dispactched
    * the function should return void but take `util::event<CLIENT_EVENTS>&` as param
    */
    using eventFn = std::function<void(util::event<CLIENT_EVENTS>&)>;

public:

    /**
    * @brief client constructor. whenn the consturcor is called, it connects to the server, and starts the listen thread
    *
    * @param t_ip the IP of the server
    * @param t_port the listen port of the server
    * @param t_eventFn the event function to use
    * @param t_logger the logger to use (PRE-INITALIZED)
    */
    client(const std::string& t_ip, const int& t_port, eventFn t_eventFn, util::logger& t_logger) : m_ip(t_ip), m_port(t_port), m_logger(t_logger), m_eventFunction(t_eventFn) {
        m_alive = false;
        m_closed = true;
        connectServer();
        startListenThread();
    }

    /**
    * @brief disconnect from the server. can be called more then once. If allready disconnected, the function will do nothing.
    */
    void disconnect() {
        handleDisconnect();
    }

    /**
    * @brief reconnect to the server. If client is allready connected, the function will do nothing
    */
    void reconnect() {
        connectServer();
        startListenThread();
    }

    /**
    * @brief send the server a (string) message
    *
    * @param t_message the message to send
    */
    void sendServer(const std::string& t_message) {
        handleSend(t_message.c_str(), strlen(t_message.c_str()));
    }

    /**
    * @brief send the server a message
    *
    * @param t_buff the bytes to send
    * @param t_buffSize the size of the message
    */
    void sendServer(char* t_buff, const size_t& t_buffSize) {
        handleSend(t_buff, t_buffSize);
    }

    /**
    * @brief client destructor. disconnects from the server. If allready disconnected, will do nothing
    */
    ~client() {
        handleDisconnect();
    }

    /**
    * @brief alive getter. will return true if the client is currently listening
    *
    * @return if the server is alive or not
    */
    bool alive() const { return m_alive; }

private:

    void startListenThread() {
        // * only restart the listen thread if it is not started allready 
        using namespace std::chrono_literals;
        if (!m_clientThread.valid() || m_clientThread.wait_for(0ms) == std::future_status::ready) {
            // * set active to true as server is initalized and launch the listen thread
            logMessage(util::LOGGER_LEVEL::INFO, "starting client listen thread...");
            m_alive = true;
            m_clientThread = std::async(std::launch::async, &client::clientListen, this);

            // * dispatch the listen event
            client_start_listen_event listenStartEv;
            m_eventFunction(listenStartEv);
        }
    }

    void connectServer() {
        // * only connect if the server socket has been closed
        if (m_closed == true) {
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
            client_connect_event connEv(m_serverFd, m_clientFd, m_ip, m_port);
            m_eventFunction(connEv);

            // * log client is connected
            logMessage(util::LOGGER_LEVEL::INFO, util::fmt("connected to server at ip {} on port {}", m_ip, m_port));
        }
    }

    void handleSend(const char* t_data, const size_t& t_dataSize) {
        // * get message in human readable format to log
        std::string message(t_data, t_dataSize);
        // * remove newline to not mess up terminal output
        message.erase(std::remove(message.begin(), message.end(), '\n'), message.cend());

        // * log the message was sent
        logMessage(util::LOGGER_LEVEL::INFO, util::fmt("sending server (ip {}, port {}) data: {}", m_ip, m_port, message));

        // * create send message event and dispatch to event function
        client_send_event sendEv = client_send_event(m_serverFd, t_data, t_dataSize);
        m_eventFunction(sendEv);

        // * send the bytes to the client
        send(m_serverFd, (void*)t_data, t_dataSize, 0);
    }


    void clientListen() {
        client_start_listen_event startListenEv;
        m_eventFunction(startListenEv);
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
            logMessage(util::LOGGER_LEVEL::INFO, "connection closed by forigin host");
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
        client_receve_event readEv(m_serverFd, t_buf, t_size); 
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
