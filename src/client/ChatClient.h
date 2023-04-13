#ifndef CHAT_CLIENT_SERVICE_H
#define CHAT_CLIENT_SERVICE_H

class ChatClient {
    public:
        ChatClient(int port);
        ChatClient(const ChatClient&) = delete;
        void start();
        void stop();
    private:
        int port;
        int clientId;
};

#endif
