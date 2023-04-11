#include <iostream>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <thread>
#include <set>
#include "ChatServer.h"

using namespace std;

class ChatServerImpl : public ChatServer {

public:
    ChatServerImpl(int port) {
        port = port;
        server_fd = createSocket();
    }

    int start() override {
        while (true) {
            for (auto client : clients) {
                cout << client << '\n';
            }
            cout << "Clients size = " << clients.size() << '\n';

            sockaddr_in client_address;
            socklen_t client_address_len = sizeof(client_address);
            int client_fd = accept(server_fd, (sockaddr*)&client_address, &client_address_len);
            if (client_fd == -1) {
                cout << "Failed to accept client" << endl;
                continue;
            }
            clients.insert(client_fd);

            // 새로운 클라이언트를 처리하는 스레드 생성
            thread client_thread(handle_message, client_fd, ref(clients));
            client_thread.detach();
        }
        stop();
    }


    void stop() override {
        for (auto& client_fd : clients) {
            close(client_fd);
        }
        close(server_fd);
    }


private:
    int port;
    int server_fd;
    set<int> clients;

    static void handle_message(int client_fd, set<int>& clients) {
        for (auto& other_client_fd : clients) {
            string message = "Client " + to_string(client_fd) + " has joined the chat.\n";
            send(other_client_fd, message.c_str(), message.size(), 0);
        }

        while (true) {
            char buffer[1024];
            int bytes_received = recv(client_fd, buffer, sizeof(buffer), 0);
            if (bytes_received == 0) {
                for (auto& other_client_fd : clients) {
                    string message = "Client " + to_string(client_fd) + " has left the chat.\n";
                    send(other_client_fd, message.c_str(), message.size(), 0);
                }
                break;
            }
            string message = string(buffer, bytes_received);
            for (auto& other_client_fd : clients) {
                if (other_client_fd != client_fd) {
                    send(other_client_fd, message.c_str(), message.size(), 0);
                }
            }
        }
        clients.erase(client_fd);
        close(client_fd);
    }


    int createSocket() {
        int server_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (server_fd == -1) {
            cout << "Failed to create socket" << endl;
            return -1;
        }
        
        // 소켓 설정
        sockaddr_in address;
        memset(&address, 0, sizeof(address));
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(8080);
        
        // 소켓 바인드
        if (::bind(server_fd, (sockaddr*)&address, sizeof(address)) == -1) {
            cout << "Failed to bind socket" << endl;
            return -1;
        }
        
        // 소켓 대기
        if (listen(server_fd, 10) == -1) {
            cout << "Failed to listen on socket" << endl;
            return -1;
        }
        
        return server_fd;
    }
};