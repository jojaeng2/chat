#ifndef CHAT_SERVER_SERVICE_H
#define CHAT_SERVER_SERVICE_H

#include <set>
#include <string>

using namespace std;

class ChatServer {
    public:
        virtual int start() = 0;
        virtual void stop() = 0;
};

#endif
