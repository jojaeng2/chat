#include "ChatServer.h"
#include "ChatServer.cpp"
using namespace std;

int main() {
    ChatServerImpl server = ChatServerImpl(8080);
    server.start();
}