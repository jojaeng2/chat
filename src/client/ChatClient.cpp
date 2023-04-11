#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>
#include "ChatClient.h"
using namespace std;

class ChatClientImpl : public ChatClient {

public:
    ChatClientImpl(int port) {
        clientId = createSocket(port);
    }

    void handle_message(int client_id) {
        while (true) {
            char buffer[1024];
            int bytes_received = recv(client_id, buffer, sizeof(buffer), 0);
            if (bytes_received <= 0) {
                cout << "Failed to receive data" << endl;
                return;
            }
            
            // 메시지 출력
            cout << "Server: " << string(buffer, bytes_received) << endl;
        }
    }

    void start() {
        thread client_thread(client_input, clientId);
        client_thread.detach();

        // 채팅 시작
        while (true) {
            // 메시지 입력
            string message;
            cin >> message;
            // 메시지 전송
            if (send(clientId, message.c_str(), message.size(), 0) == -1) {
                cout << "Failed to send message" << endl;
                break;
            }
        }

        // 소켓 종료
        close(clientId);
    }

private:
    int clientId;

    static void client_input(int cient_fd) {
    // 서버로부터 메시지 수신
        while (true) {
            char buffer[1024];
            int bytes_received = recv(cient_fd, buffer, sizeof(buffer), 0);
            if (bytes_received <= 0) {
                cout << "Failed to receive data" << endl;
                return;
            }
            
            // 메시지 출력
            cout << "Server: " << string(buffer, bytes_received) << endl;
        }
    }

    int createSocket(int port) {
        // 서버 주소 설정
        sockaddr_in server_address;
        memset(&server_address, 0, sizeof(server_address));
        server_address.sin_family = AF_INET;
        server_address.sin_port = htons(IP_PORTRANGE_HIGH);
        inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
        
        // 소켓 생성
        int client_fd = socket(AF_INET, SOCK_STREAM, 0);
        if (client_fd == -1) {
            cout << "Failed to create socket" << endl;
            return 0;
        }
        
        // 서버에 연결
        if (connect(client_fd, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
            cout << "Failed to connect to server" << endl;
            return 0;
        }
        return client_fd;
    }
};
