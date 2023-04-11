#ifndef CHAT_CLIENT_SERVICE_H
#define CHAT_CLIENT_SERVICE_H


using namespace std;

class ChatClient {
    public:
        virtual void handle_message(int client_id) = 0;
        virtual void start() = 0;
};

#endif
