#ifndef UTIL_SERVER_HPP
#define UTIL_SERVER_HPP

#include <cstdlib>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/_types/_socklen_t.h>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <logger.hpp>
#include <logger_console_sink.hpp>
#include <exception>
#include <future>
#include <vector>
#include <map>
#include <sys/time.h>
#include <arpa/inet.h>
#include <algo.hpp>
#include <event.hpp>

namespace util {

enum class SERVER_METHOD {
    TCP,
    UDP,
};

enum class SERVER_EVENTS {
    // * send/receve events
    send,
    recv,

    // * client connection events
    client_connect,
    client_disconnect,

    // * logger events
    before_log,
    after_log,
};

class server_before_log_event : public util::event<SERVER_EVENTS> {
    
public:

    EVENT_CLASS_TYPE(SERVER_EVENTS, before_log);

};

class server_after_log_event : public util::event<SERVER_EVENTS> {
    
public:

    EVENT_CLASS_TYPE(SERVER_EVENTS, after_log);

};

class server_client_event : public util::event<SERVER_EVENTS> {

public:

    int getClientSd() const { return m_clientSd; }
    int getClientPort() const { return m_clientPort; }
    std::string getClientIp() const { return m_clientIp; }
    std::string getClientId() const { return m_clientId; }

protected:

    server_client_event(const int& t_clientSd, const int& t_clientPort, const std::string& t_clientIp, const std::string& t_clientId) : 
        m_clientSd(t_clientSd), m_clientPort(t_clientPort), m_clientIp(t_clientIp), m_clientId(t_clientId) {}

    int m_clientSd;
    int m_clientPort;
    std::string m_clientIp;
    std::string m_clientId;
};

class server_client_disconnect_event : public server_client_event {

public:

    server_client_disconnect_event(const int& t_clientSd, const int& t_clientPort, const std::string& t_clientIp, const std::string& t_clientId) : 
       server_client_event(t_clientSd, t_clientPort, t_clientIp, t_clientId) {}

    EVENT_CLASS_TYPE(SERVER_EVENTS, client_disconnect);
};

class server_client_connect_event : public server_client_event {

public:

    server_client_connect_event(const int& t_clientSd, const int& t_clientPort, const std::string& t_clientIp, const std::string& t_clientId) : 
       server_client_event(t_clientSd, t_clientPort, t_clientIp, t_clientId) {}

    EVENT_CLASS_TYPE(SERVER_EVENTS, client_connect);
};

class server_read_event : public server_client_event {

public:
        
    server_read_event(const std::string& t_clientId, const int& t_clientSd, const std::string& t_clientIp, const int& t_clientPort, char* t_clientBuff, const int& t_clientBuffSize) :
        server_client_event(t_clientSd, t_clientPort, t_clientIp, t_clientId), m_clientBuffSize(t_clientBuffSize) {
        m_clientBuff = new char[m_clientBuffSize];
        memcpy(m_clientBuff, t_clientBuff, (size_t)m_clientBuffSize);
    }

    ~server_read_event() {
        delete[] m_clientBuff;
    }

    int getBuffSize() const { return m_clientBuffSize; }
    const char *getClientBuff() const{ return m_clientBuff; }

    EVENT_CLASS_TYPE(SERVER_EVENTS, recv);

private:

    char* m_clientBuff;
    int m_clientBuffSize;
};

class server_send_event : public server_client_event {

public:
        
    server_send_event(const std::string& t_clientId, const int& t_clientSd, const std::string& t_clientIp, const int& t_clientPort, const char* t_clientBuff, const int& t_clientBuffSize) :
        server_client_event(t_clientSd, t_clientPort, t_clientIp, t_clientId), m_clientBuffSize(t_clientBuffSize) {
        m_clientBuff = new char[m_clientBuffSize];
        memcpy(m_clientBuff, t_clientBuff, (size_t)m_clientBuffSize);
    }

    ~server_send_event() {
        delete[] m_clientBuff;
    }

    int getBuffSize() const { return m_clientBuffSize; }
    const char *getClientBuff() const{ return m_clientBuff; }

    EVENT_CLASS_TYPE(SERVER_EVENTS, send);

private:

    char* m_clientBuff;
    int m_clientBuffSize;
};

class server {

    // * the event function that will be called to run events
    using eventFn = std::function<void(util::event<SERVER_EVENTS>&)>;

public:

    server(const int& t_port, const SERVER_METHOD& t_serverMethod, eventFn t_eventFn, util::logger& t_serverLogger) : m_serverLogger(t_serverLogger) {
        // * set the port, server method and event function
        m_port = t_port;
        m_serverMethod = t_serverMethod;
        m_eventFunction = t_eventFn;

        // * initalize listening to false
        m_listening = false;

        // * pre-declare vars
        sockaddr_in address;
        int addrlen = sizeof(address);
        int opt = 1;

        // * create main server socket
        if ((m_serverFd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            logMessage(util::LOGGER_LEVEL::CRITICAL, "failed to create server socket");
            logMessage(util::LOGGER_LEVEL::ERROR, util::fmt("socket error: {}", strerror(errno)));
            throw std::runtime_error("failed to create server socket");
        }

        // * set socket options
        if (setsockopt(m_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
            logMessage(util::LOGGER_LEVEL::CRITICAL, "failed to set socket options");
            logMessage(util::LOGGER_LEVEL::ERROR, util::fmt("setsockopt error: {}", strerror(errno)));
            throw std::runtime_error("failed to set socket options");
        }

        // * set the socket ports and adresses
        address.sin_family = AF_INET;
        address.sin_port = htons(m_port);
        address.sin_addr.s_addr = INADDR_ANY;

        // * bind the server to the addresses and ports
        if (bind(m_serverFd, (sockaddr*)&address, addrlen) < 0) {
            logMessage(util::LOGGER_LEVEL::CRITICAL, "failed to bind server socket");
            logMessage(util::LOGGER_LEVEL::ERROR, util::fmt("bind error: {}", strerror(errno)));
            throw std::runtime_error("failed to bind server socket");
        }

        // * mark the server socket as set to listen
        if (listen(m_serverFd, 3) < 0) {
            logMessage(util::LOGGER_LEVEL::CRITICAL, "failed to set server socket as listening");
            logMessage(util::LOGGER_LEVEL::ERROR, util::fmt("listen error: {}", strerror(errno)));
            throw std::runtime_error("failed to set server socket as listening");
        }

        // * log the the server was properly initalized
        logMessage(util::LOGGER_LEVEL::INFO, util::fmt("initalized server on port {}", m_port));

        // * set the listening flag
        m_listening = true;
        m_listenThread = std::async(std::launch::async, &server::serverListen, this);
    }

    /*
    * TODO
    */
    void sendClient(const std::string& t_clientId, const std::string& t_message) {
        handleSend(t_clientId, t_message.c_str(), strlen(t_message.c_str()));
    }

    /*
    * TODO
    */
    void sendClient(const std::string& t_clientId, char* t_buff, const size_t& t_buffSize) {

    }

    /*
    * TODO
    */
    void sendAllClients(const std::string& t_message) {
        for (auto &clientConn : m_clientMap) {
            sendClient(clientConn.first, t_message);
        }
    }

    /*
    * TODO
    */
    void sendAllClients(char* t_buff, const size_t& t_buffSize) {

    }

    ~server() {
        logMessage(util::LOGGER_LEVEL::INFO, "stopping listener thread...");
        // * unset the listening flag and wait for listneing thread to join
        m_listening = false;
        m_listenThread.wait();

        // * close all outgoing server connections
        std::vector<std::string> clientsToDisconnect;
        for (auto &con : m_clientMap) {
            clientsToDisconnect.push_back(con.first);
        }
        for (auto &cid : clientsToDisconnect) {
            disconnectClient(cid);
        }

        // * shutdown the server socket
        logMessage(util::LOGGER_LEVEL::INFO, "shutting down server");
        shutdown(m_serverFd, SHUT_RDWR);
    }

    util::logger& getLogger() { return m_serverLogger; }

private:

    /**
    * TODO
    */
    void handleSend(const std::string& t_clientId, const char* t_data, const size_t& t_dataSize) {
        // * declare vars needed for rest of func
        int clientFd = m_clientMap[t_clientId];
        std::string ip;
        int port;
        // * get client details to log and create event
        getClientDetails(clientFd, ip, port);

        // * get message in human readable format to log
        std::string message(t_data, t_dataSize);
        // * remove newline to not mess up terminal output
        message.erase(std::remove(message.begin(), message.end(), '\n'), message.cend());

        // * log the message was sent
        logMessage(util::LOGGER_LEVEL::INFO, util::fmt("sending client ({}) at ip {} on port {} data: {}", t_clientId, ip, port, message));

        // * create send message event and dispatch to event function
        server_send_event sendEv = server_send_event(t_clientId, clientFd, ip, port, t_data, t_dataSize);
        m_eventFunction(sendEv);

        // * send the bytes to the client
        send(clientFd, (void*)t_data, t_dataSize, 0);
    }

    void logMessage(const util::LOGGER_LEVEL& t_level, const std::string& t_message) const {
        // * creater the before log event and send it to the event function
        server_before_log_event beforeLog;
        m_eventFunction(beforeLog);

        // * log the message to ALL sinks
        m_serverLogger.log(t_message, t_level);

        // * create the after log event and send it to the event function
        server_after_log_event afterLog;
        m_eventFunction(afterLog);
    }

    void serverListen() {
        // * log server listen
        logMessage(util::LOGGER_LEVEL::INFO, "server now listening for connections");

        // * setup file discriptor set and timeout
        fd_set readfds;
        timeval tv;
        tv.tv_sec = 3;
        tv.tv_usec = 50;

        while (m_listening) {
            // * clear the socket discriptor set
            FD_ZERO(&readfds);

            // * add the server socket to the fd set
            FD_SET(m_serverFd, &readfds);
            int max_sd = m_serverFd;

            for (auto &con : m_clientMap) {
                int sd = con.second;

                // * if the client socket descriptor is valid, add it to the set
                if (sd > 0) {
                    FD_SET(sd, &readfds);
                }

                // * get the max sokcet discriptor (for select)
                max_sd = std::max(max_sd, sd);
            }

            // * get the activity of the socket discriptors
            int activity = select(max_sd + 1, &readfds, NULL, NULL, &tv);


            // * negative activity is an error, deal with accordingly
            if (activity < 0) {
                
            }

            // * 0 activity is timeout (or no activity)
            if (activity == 0) {
                continue;
            }

            // * if the master socket is set, new incoming connection
            if (FD_ISSET(m_serverFd, &readfds)) {
                connectClient();
            }
            
            // * if the client sockets are set, incoming message or client disconnection
            // * loop through all the clients and check if they are set
            std::vector<std::string> disconnectedClients;
            for (auto& clientCon : m_clientMap) {
                int sd = clientCon.second;

                if (FD_ISSET(sd, &readfds)) {
                    // * check if a client has disconnected from the server
                    if (handleClient(clientCon.first)) {
                        // * if client disconnected, add to list of disconnecrted clients
                        disconnectedClients.push_back(clientCon.first);
                        continue;
                    }
                }
            }

            for (auto& clientId : disconnectedClients) {
                disconnectClient(clientId);
            }
        }

        logMessage(util::LOGGER_LEVEL::INFO, "server stopped listening for connections");
    }

    void connectClient() {
        // * declare vars needed for accept()
        sockaddr_in address;
        int addrlen = sizeof(address);
        int clientFd;

        // * accept client conn and check for error
        if ((clientFd = accept(m_serverFd, (sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
            logMessage(util::LOGGER_LEVEL::CRITICAL, "failed to accept new client");
            logMessage(util::LOGGER_LEVEL::ERROR, util::fmt("accept error: {}", strerror(errno)));
            throw std::runtime_error("failed to accept new client");
        }

        // * setup variables to be logged
        std::string connId = generate_uuid_v4();
        std::string clientIp = inet_ntoa(address.sin_addr);
        int clientPort = ntohs(address.sin_port);

        // * log client connet
        logMessage(LOGGER_LEVEL::INFO, util::fmt("new client ({}) connection at ip {} on port {}", connId, clientIp, clientPort));

        // * create and dispatch the client connected event
        server_client_connect_event connEv = server_client_connect_event(clientFd, clientPort, clientIp, connId);
        m_eventFunction(connEv);

        // * add the client to the client map
        m_clientMap[connId] = clientFd;
    }

    bool handleClient(const std::string& t_clientId) {
        constexpr int bufSize = 1024;
        char buf[bufSize];
        int size;
        int clientSd = m_clientMap[t_clientId];

        if ((size = read(clientSd, buf, bufSize)) == 0) {
            return true;
        }

        handleRead(t_clientId, buf, size);
        return false;
    }

    void handleRead(const std::string& t_clientId, char* t_buf, const int& t_bufSize) {
        // * conver the buffer to a string
        std::string message(t_buf, t_bufSize);
        // * remove newline to not mess up terminal output
        message.erase(std::remove(message.begin(), message.end(), '\n'), message.cend());

        // * get required client info for read event
        int sd = m_clientMap[t_clientId];
        std::string ip;
        int port;
        getClientDetails(sd, ip, port);

        // * log read
        logMessage(util::LOGGER_LEVEL::INFO, util::fmt("client ({}) at ip {} on port {} sent: {}", t_clientId, ip, port, message));

        // * create and dispatch read event
        server_read_event readEv = server_read_event(t_clientId, sd, ip, port, t_buf, t_bufSize);
        m_eventFunction(readEv);
    }

    void getClientDetails(const int& t_sd, std::string& t_ip, int& t_port) const {
        // * declate the address struct
        sockaddr_in address;
        int addrlen = sizeof(address);

        // * get peer name for the socet discriptor
        getpeername(t_sd, (sockaddr *)&address, (socklen_t *)&addrlen);
        // * convert the address struct to human readable data
        t_ip = std::string(inet_ntoa(address.sin_addr));
        t_port = ntohs(address.sin_port);
    }

    void disconnectClient(const std::string& t_clientId) {
        // * get client data to log client dissconnected
        int clientSd = m_clientMap[t_clientId];
        int clientPort;
        std::string clientIp;
        getClientDetails(clientSd, clientIp, clientPort);
        logMessage(util::LOGGER_LEVEL::INFO, util::fmt("dissconnecting client ({}), at ip {} on port {}", t_clientId, clientIp, clientPort));

        // * create and dispatch client disconencted event
        server_client_disconnect_event disconevent = server_client_disconnect_event(clientSd, clientPort, clientIp, t_clientId);
        m_eventFunction(disconevent);

        // * close the client connection
        close(clientSd);

        // * remove the client from the client map
        m_clientMap.erase(t_clientId);
    }

private:

    // * a map of all the clients
    std::map<std::string, int> m_clientMap;
    // * the futre of the thread listening for connections and messages asyncronsly
    std::future<void> m_listenThread;

    // * store the event function that should be called for every event
    eventFn m_eventFunction;

    // * the class logger
    util::logger& m_serverLogger;


    int m_port, m_serverFd;
    SERVER_METHOD m_serverMethod;
    bool m_listening;
};


}

#endif
