#include "ChatServer.h"
#include "../../src/db/ChatRepository.h"
#include <iostream>
using namespace std;

ChatRepository& initChatRepository() {
    return *(new ChatRepository("chat"));
}

int main() {
    ChatRepository& chatRepository = initChatRepository();
    ChatServer * server = new ChatServer(8080, chatRepository);
    server->start();
    chatRepository.close();
    server -> stop();
    delete &chatRepository; 
    delete server;
}