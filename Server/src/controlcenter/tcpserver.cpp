#include "tcpserver.h"
#include "utils/shared.h"
#include <iostream>
#include <string>



TcpServer::TcpServer(int port)
    :m_port(port)
{
    // Windows平台初始化
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        throw std::runtime_error("WSAStartup failed: " + std::to_string(result));
    }

    // 创建套接字
    m_listen_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (m_listen_socket == INVALID_SOCKET) {
        throw std::runtime_error("Socket creation failed: " + std::to_string(WSAGetLastError()));
    }

    // 绑定套接字
    sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    result = bind(m_listen_socket, (sockaddr*)&server_addr, sizeof(server_addr));
    if (result == SOCKET_ERROR) {
        throw std::runtime_error("Bind failed: " + std::to_string(WSAGetLastError()));
    }

    // 监听连接
    result = listen(m_listen_socket, SOMAXCONN);
    if (result == SOCKET_ERROR) {
        throw std::runtime_error("Listen failed: " + std::to_string(WSAGetLastError()));
    }

}

TcpServer::~TcpServer()
{
    Stop();
    closesocket(m_listen_socket);
    WSACleanup();
}

ErrorCode TcpServer::Start(){
    if(m_isRunning)
        return SUCCESS;
    
    m_isRunning = true;
    m_acceptThread = std::thread(&TcpServer::acceptClients, this);

    return  SUCCESS;
}

ErrorCode TcpServer::Stop(){
    if(!m_isRunning)
        return SUCCESS;

    m_isRunning = false;
    if(m_acceptThread.joinable())
        m_acceptThread.join();
    
    std::lock_guard < std::mutex > lock(m_mutex);
    for(auto& client : m_clients){
        closesocket(client.sock);
    }

    m_clients.clear();
    return SUCCESS;
}


ErrorCode TcpServer::acceptClients(){
    while (m_isRunning) {
        if(m_clients.size() >= MAX_CLIENT_COUNT){
            std::cerr << "Maximum number of clients reached." << std::endl;
            continue;
        }

        sockaddr_in client_addr;
        int client_addr_size = sizeof(client_addr);
        SOCKET client_socket = accept(m_listen_socket, (sockaddr*)&client_addr, &client_addr_size);
        if (client_socket == INVALID_SOCKET) {
            std::cerr << "Accept failed: " << WSAGetLastError() << std::endl;
            continue;
        }

        // 获取客户端信息
        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &client_addr.sin_addr, client_ip, INET_ADDRSTRLEN);
        uint16_t client_port = ntohs(client_addr.sin_port);

        // 将客户端信息添加到列表
        std::lock_guard<std::mutex> lock(m_mutex);
        ClientInfo ci;
        ci.ipAddress = client_ip;
        ci.sock = client_socket;   
        m_clients.emplace_back(std::move(ci));

        // 在新线程中处理客户端连接
        std::thread client_thread(&TcpServer::handleClient, this, m_clients.back());
        m_clientThreads.push_back(std::move(client_thread));
        m_clientThreads.back().detach();
    }

    return SUCCESS;
}

ErrorCode TcpServer::handleClient(ClientInfo& clientInfo) {
    char buffer[1024];
    SOCKET client_socket = clientInfo.sock;
    while (true) {
        int bytes_received = recv(client_socket, buffer, sizeof(buffer), 0);
        if (bytes_received <= 0) {
            break; // 客户端断开连接
        }

        // 处理接收到的数据
        std::string message(buffer, bytes_received);
        std::cout << "Received from client: " << message << std::endl;

        // 回应客户端
        std::string response = "Message received: " + message;
        send(client_socket, response.c_str(), response.size(), 0);
    }

    // 关闭客户端连接
    std::lock_guard<std::mutex> lock(m_mutex);
    m_clients.erase(std::remove(m_clients.begin(), m_clients.end(), clientInfo), m_clients.end());
    closesocket(client_socket);
}