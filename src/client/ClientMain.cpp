#include "ChatClient.h"
#include "ChatClient.cpp"

int main() {
    ChatClientImpl* client = new ChatClientImpl(8080);
    client -> start();
    delete client;
}