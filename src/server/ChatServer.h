#ifndef CHAT_SERVER_SERVICE_H
#define CHAT_SERVER_SERVICE_H

#include <set>
#include <string>
#include "../../src/db/ChatRepository.h"

using namespace std;

class ChatServer {
    public:
        ChatServer(int port, ChatRepository& chatRepository);
        ChatServer(const ChatServer&) = delete;
        void start();
        void stop();
    private:
        int port;
        int serverId;
        set<int> clients;
        ChatRepository& chatRepository;
};

#endif
