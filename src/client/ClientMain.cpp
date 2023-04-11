#include "ChatClient.h"
#include "ChatClient.cpp"

int main() {
    ChatClientImpl client = ChatClientImpl(8080);
    client.start();
}