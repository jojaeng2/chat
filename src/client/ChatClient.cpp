#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <thread>
#include <unistd.h>
#include <arpa/inet.h>
#include "ChatClient.h"
using namespace std;

int createSocket(int port) {
    // 서버 주소 설정
    sockaddr_in server_address;
    memset(&server_address, 0, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    inet_pton(AF_INET, "127.0.0.1", &server_address.sin_addr);
    
    // 소켓 생성
    int clientId = socket(AF_INET, SOCK_STREAM, 0);
    if (clientId == -1) {
        cout << "Failed to create socket" << endl;
        return 0;
    }
    
    // 서버에 연결
    if (connect(clientId, (sockaddr*)&server_address, sizeof(server_address)) == -1) {
        cout << "Failed to connect to server" << endl;
        return 0;
    }
    return clientId;
}

static void receiveMessage(int cient_fd) {
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

ChatClient::ChatClient(int port) : port(port){
    clientId = createSocket(port);
}

void ChatClient::start() {
    thread client_thread(receiveMessage, clientId);
    client_thread.detach();
    // 채팅 시작
    while (true) {
        string message;
        cin >> message;
        // 메시지 전송
        if (send(clientId, message.c_str(), message.size(), 0) == -1) {
            cout << "Failed to send message" << endl;
            break;
        }
    }
}

void ChatClient::stop() {
    close(clientId);
}
