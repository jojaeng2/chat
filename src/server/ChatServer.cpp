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
        serverId = createSocket();
    }

    int start() override {
        while (true) {
            cout << "Clients size = " << clients.size() << '\n';

            sockaddr_in client_address;
            socklen_t client_address_len = sizeof(client_address);
            int clientId = accept(serverId, (sockaddr*)&client_address, &client_address_len);
            if (clientId == -1) {
                cout << "Failed to accept client" << endl;
                continue;
            }
            clients.insert(clientId);

            // 새로운 클라이언트를 처리하는 스레드 생성
            thread client_thread(handle_message, clientId, ref(clients));
            client_thread.detach();
        }
        stop();
    }


    void stop() override {
        for (auto& clientId : clients) {
            close(clientId);
        }
        close(serverId);
    }


private:
    int port;
    int serverId;
    set<int> clients;

    static void handle_message(int clientId, set<int>& clients) {
        for (auto& other_clientId : clients) {
            string message = "Client " + to_string(clientId) + " has joined the chat.\n";
            send(other_clientId, message.c_str(), message.size(), 0);
        }

        while (true) {
            char buffer[1024];
            int bytes_received = recv(clientId, buffer, sizeof(buffer), 0);
            if (bytes_received == 0) {
                for (auto& other_clientId : clients) {
                    string message = "Client " + to_string(clientId) + " has left the chat.\n";
                    send(other_clientId, message.c_str(), message.size(), 0);
                }
                break;
            }
            string message = string(buffer, bytes_received);
            for (auto& other_clientId : clients) {
                if (other_clientId != clientId) {
                    send(other_clientId, message.c_str(), message.size(), 0);
                }
            }
        }
        clients.erase(clientId);
        close(clientId);
    }


    int createSocket() {
        int serverId = socket(AF_INET, SOCK_STREAM, 0);
        if (serverId == -1) {
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
        if (::bind(serverId, (sockaddr*)&address, sizeof(address)) == -1) {
            cout << "Failed to bind socket" << endl;
            return -1;
        }
        
        // 소켓 대기
        if (listen(serverId, 10) == -1) {
            cout << "Failed to listen on socket" << endl;
            return -1;
        }
        
        return serverId;
    }
};