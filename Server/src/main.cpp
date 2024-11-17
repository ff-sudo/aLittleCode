#include "controlcenter/tcpserver.h"

#include <iostream>



int main(int argc, char *argv[]){

    auto exec = [](){
        try{
            TcpServer server(12345);
            server.Start();

            // 保持主线程运行
            while (true) {
                std::this_thread::sleep_for(std::chrono::seconds(1));
            }
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
        }
    };
    exec();

    return 0;
}