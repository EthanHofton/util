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

/**
* @brief enum for the different types of connection protocalls for the server
*/
enum class SERVER_METHOD {
    TCP,
    UDP,
};

/**
* @brief enum for all the different events the server may dispatch
*/
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

    // * startup and shutdown events
    server_startup,
    server_shutdown,
};

/**
* @brief server before log event class
*/
class server_before_log_event : public util::event<SERVER_EVENTS> {
    
public:

    EVENT_CLASS_TYPE(SERVER_EVENTS, before_log);

};

/**
* @brief server after log event class
*/
class server_after_log_event : public util::event<SERVER_EVENTS> {
    
public:

    EVENT_CLASS_TYPE(SERVER_EVENTS, after_log);

};

/**
* @brief server client event class, stores all the fields and methods common to the clinet events
*/
class server_client_event : public util::event<SERVER_EVENTS> {

public:

    /**
    * @brief clinet socket discriptor getter
    *
    * @return the clinets socket discriptor
    */
    int getClientSd() const { return m_clientSd; }
    /**
    * @brief client port getter
    *
    * @return the clients port 
    */
    int getClientPort() const { return m_clientPort; }
    /**
    * @brief client ip getter
    *
    * @return the clients ip address
    */
    std::string getClientIp() const { return m_clientIp; }
    /**
    * @brief client id getter
    *
    * @return the clinets ID in the server
    */
    std::string getClientId() const { return m_clientId; }

protected:

    /**
    * @brief server client event constructor. set all of the fields the constructor takes
    *
    * @param t_clientSd the file discriptor of the client
    * @param t_clientPort the port the clinet is on
    * @param t_clientIp the ip adress of the client
    * @param t_clientId the address of the client on the server
    */
    server_client_event(const int& t_clientSd, const int& t_clientPort, const std::string& t_clientIp, const std::string& t_clientId) : 
        m_clientSd(t_clientSd), m_clientPort(t_clientPort), m_clientIp(t_clientIp), m_clientId(t_clientId) {}

    /**
    * @brief the socket discriptor of the client
    */
    int m_clientSd;
    /**
    * @brief the port the client is on
    */
    int m_clientPort;
    /**
    * @brief the ip of the client
    */
    std::string m_clientIp;
    /**
    * @brief the id of the client
    */
    std::string m_clientId;
};

/**
* @brief server client disconnection event
*/
class server_client_disconnect_event : public server_client_event {

public:

    /**
    * @brief construcor. pass the varaibles to the server_client_event consturcotr
    *
    * @param t_clientSd the socket discriptor of the client
    * @param t_clientPort the port of the client
    * @param t_clientIp the ip of the client
    * @param t_clientId the Id of the client
    */
    server_client_disconnect_event(const int& t_clientSd, const int& t_clientPort, const std::string& t_clientIp, const std::string& t_clientId) : 
       server_client_event(t_clientSd, t_clientPort, t_clientIp, t_clientId) {}

    EVENT_CLASS_TYPE(SERVER_EVENTS, client_disconnect);
};

/**
* @brief client connection event
*/
class server_client_connect_event : public server_client_event {

public:

    /**
    * @brief server-clinet connection evnent. gets called when a new client connections to the server
    *
    * @param t_clientSd the socket discriptor of the client
    * @param t_clientPort the port the client is on
    * @param t_clientIp the ip of the client
    * @param t_clientId the ID of the client
    */
    server_client_connect_event(const int& t_clientSd, const int& t_clientPort, const std::string& t_clientIp, const std::string& t_clientId) : 
       server_client_event(t_clientSd, t_clientPort, t_clientIp, t_clientId) {}

    EVENT_CLASS_TYPE(SERVER_EVENTS, client_connect);
};

/**
* @brief server read event. gets called after the server has read a new message from the client
*/
class server_read_event : public server_client_event {

public:
        
    /**
    * @brief server read event constuctor
    *
    * @param t_clientId the id of the client
    * @param t_clientSd the socket discriptor of the client
    * @param t_clientIp the ip of the client
    * @param t_clientPort the port the client is on
    * @param t_clientBuff the buff the server read
    * @param t_clientBuffSize the size of the buffer the server read
    */
    server_read_event(const std::string& t_clientId, const int& t_clientSd, const std::string& t_clientIp, const int& t_clientPort, char* t_clientBuff, const int& t_clientBuffSize) :
        server_client_event(t_clientSd, t_clientPort, t_clientIp, t_clientId), m_clientBuffSize(t_clientBuffSize) {
        m_clientBuff = new char[m_clientBuffSize];
        memcpy(m_clientBuff, t_clientBuff, (size_t)m_clientBuffSize);
    }

    /**
    * @brief server read desctructor
    */
    ~server_read_event() {
        delete[] m_clientBuff;
    }

    /**
    * @brief buffer size getter
    *
    * @return the size of the buffer 
    */
    int getBuffSize() const { return m_clientBuffSize; }
    /**
    * @brief client buffer getter
    *
    * @return the buffer the client sent
    */
    const char *getClientBuff() const{ return m_clientBuff; }

    EVENT_CLASS_TYPE(SERVER_EVENTS, recv);

private:

    char* m_clientBuff;
    int m_clientBuffSize;
};

/**
* @brief server send message event. gets called when the server sends a message to a socket
*/
class server_send_event : public server_client_event {

public:
        
    /**
    * @brief server send event constructor
    *
    * @param t_clientId the Id of the client
    * @param t_clientSd the sokcet descriptor of the client
    * @param t_clientIp the ip of the client
    * @param t_clientPort the port the client is on 
    * @param t_clientBuff the buffer the client sent 
    * @param t_clientBuffSize the size of the buffer the client send
    */
    server_send_event(const std::string& t_clientId, const int& t_clientSd, const std::string& t_clientIp, const int& t_clientPort, const char* t_clientBuff, const int& t_clientBuffSize) :
        server_client_event(t_clientSd, t_clientPort, t_clientIp, t_clientId), m_clientBuffSize(t_clientBuffSize) {
        m_clientBuff = new char[m_clientBuffSize];
        memcpy(m_clientBuff, t_clientBuff, (size_t)m_clientBuffSize);
    }

    /**
    * @brief server send event desctructor 
    */
    ~server_send_event() {
        delete[] m_clientBuff;
    }

    /**
    * @brief get the size of the client buffer
    *
    * @return the size of the client buffer
    */
    int getBuffSize() const { return m_clientBuffSize; }
    /**
    * @brief get the buffer the client sent
    *
    * @return the clinet buffer
    */
    const char *getClientBuff() const{ return m_clientBuff; }

    EVENT_CLASS_TYPE(SERVER_EVENTS, send);

private:

    char* m_clientBuff;
    int m_clientBuffSize;
};

/**
 * @brief server startup event. gets called when the server has started up
 */
class server_startup_event : public util::event<SERVER_EVENTS> {

public:

    /**
    * @brief server statrup event consturcotr
    *
    * @param t_serverFd the server socket descriptor
    * @param t_port the port the server is listening on
    * @param t_method the protocal used for connections on the socket
    */
    server_startup_event(const int& t_serverFd, const int& t_port, const SERVER_METHOD& t_method) :
        m_serverFd(t_serverFd), m_port(t_port), m_serverMethod(t_method) {}

    /**
    * @brief get the server file descriptor
    *
    * @return the server file descript
    */
    int getServerFd() const { return m_serverFd; }
    /**
    * @brief get the listen port
    *
    * @return the port the server is listening on
    */
    int getPort() const { return m_port; }
    /**
    * @brief server method getter
    *
    * @return the protocal the server is using
    */
    SERVER_METHOD getServerMethod() const { return m_serverMethod; }

    EVENT_CLASS_TYPE(SERVER_EVENTS, server_startup)

private:

    int m_serverFd;
    int m_port;
    SERVER_METHOD m_serverMethod;
};

/**
* @brief server shutdown evetn. is triggered just before the server shuts down
*/
class server_shutdown_event : public util::event<SERVER_EVENTS> {

public:

    /**
    * @brief server shut down event cosntructor
    *
    * @param t_serverFd the file descriptor of the server
    * @param t_port the port the server is listening on
    * @param t_clientMap the map of client ids to client file descriptors
    */
    server_shutdown_event(const int& t_serverFd, const int& t_port, std::map<std::string, int> t_clientMap) : m_serverFd(t_serverFd), m_port(t_port), m_clientMap(t_clientMap) {}

    /**
    * @brief get the server file descript
    *
    * @return the server file descript
    */
    int getServerFd() const { return m_serverFd; }
    /**
    * @brief get the port the server is listening on
    *
    * @return the port the server is listening on
    */
    int getPort() const { return m_port; }
    /**
    * @brief get the map of client ids to file descritors
    *
    * @return the map of client ids to fds 
    */
    std::map<std::string, int> getClientMap() const { return m_clientMap; }

    EVENT_CLASS_TYPE(SERVER_EVENTS, server_shutdown);

private:

    int m_serverFd;
    int m_port;
    std::map<std::string, int> m_clientMap;
};

/**
* @brief server class. uses RAII. the server is started in the constructor and shutdown in the destructor
*/
class server {

    // * the event function that will be called to run events
    using eventFn = std::function<void(util::event<SERVER_EVENTS>&)>;

public:

    /**
    * @brief server constructor. on construction, the server is started
    *
    * @param t_port the port to listen for new connections on
    * @param t_serverMethod the protocol the server will use for connections
    * @param t_eventFn the event function that will be called when an event is diespacthed
    * @param t_serverLogger the logger the server should use. PRE-INITALIZED
    */
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

        // * create a server startup event and dispactch
        server_startup_event startupEv(m_serverFd, m_port, m_serverMethod);
        m_eventFunction(startupEv);

        // * set the listening flag
        m_listening = true;
        m_listenThread = std::async(std::launch::async, &server::serverListen, this);
    }

    /**
    * @brief send a (string) message to a client
    *
    * @param t_clientId the id of the client the server should send the message to
    * @param t_message the message to send to the client
    */
    void sendClient(const std::string& t_clientId, const std::string& t_message) {
        handleSend(t_clientId, t_message.c_str(), strlen(t_message.c_str()));
    }

    /**
    * @brief send an array of bytes to a client
    *
    * @param t_clientId the id of the client to send the bytes to
    * @param t_buff the buffer of bytes to send
    * @param t_buffSize the lenght of the buffer to send
    */
    void sendClient(const std::string& t_clientId, char* t_buff, const size_t& t_buffSize) {
        handleSend(t_clientId, t_buff, t_buffSize);
    }

    /**
    * @brief send a (string) message to all clients connected
    *
    * @param t_message the message to send
    */
    void sendAllClients(const std::string& t_message) {
        for (auto &clientConn : m_clientMap) {
            sendClient(clientConn.first, t_message);
        }
    }

    /**
    * @brief send an array of bytes to all clients
    *
    * @param t_buff the bytes to send
    * @param t_buffSize the size of the array
    */
    void sendAllClients(char* t_buff, const size_t& t_buffSize) {
        for (auto &clientConn : m_clientMap) {
            sendClient(clientConn.first, t_buff, t_buffSize);
        }
    }

    /**
    * @brief server desctutor. closes the server and all outgoing connections
    */
    ~server() {
        logMessage(util::LOGGER_LEVEL::INFO, "stopping listener thread...");
        // * unset the listening flag and wait for listneing thread to join
        m_listening = false;
        m_listenThread.wait();

        // * server shutdown event and dispatch
        server_shutdown_event shutdownEv(m_serverFd, m_port, m_clientMap);
        m_eventFunction(shutdownEv);

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

    /**
    * @brief server logger getter
    *
    * @return a reference to the server logger
    */
    util::logger& getLogger() { return m_serverLogger; }

    /**
    * @brief getter for the client map
    *
    * @return the map of client ids to fds 
    */
    std::map<std::string, int> getClientMap() const { return m_clientMap; }


private:

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
