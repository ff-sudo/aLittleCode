#ifndef TCPSERVER_H
#define TCPSERVER_H

#include "utils/err.h"
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#endif

struct ClientInfo
{
    SOCKET sock = INVALID_SOCKET;
    std::string clientId;
    std::string clientName;
    std::string ipAddress;
    uint16_t port;
    time_t connectionTime;
    std::string additionalInfo;
    enum Status
    {
        CONNECTED,
        DISCONNECTED
    } status = DISCONNECTED;

};

class TcpServer
{
public:
    explicit TcpServer(int port);
    ~TcpServer();

    ErrorCode Start();
    ErrorCode Stop();

private:
    ErrorCode acceptClients();
    ErrorCode handleClient(ClientInfo& clientInfo);

private:
    bool m_isRunning{ false };
    int m_port;
    SOCKET m_listen_socket;

    std::thread m_acceptThread;
    std::vector<std::thread> m_clientThreads;
    std::mutex m_mutex;
    std::condition_variable m_cv;
    std::vector<ClientInfo> m_clients;
    
};


#endif