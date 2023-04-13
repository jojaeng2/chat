#include "ChatClient.h"

int main() {
    ChatClient* client = new ChatClient(8080);

    client -> start();
    client -> stop();
    delete client;
}