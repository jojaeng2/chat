#include "ChatServer.h"
#include "ChatServer.cpp"
using namespace std;

int main() {
    ChatServerImpl* server = new ChatServerImpl(8080);
    server -> start();
    delete server;
}