#include "src/server/ChatServer.h"
#include "src/server/ChatServer.cpp"
using namespace std;

int main() {
    ChatServerImpl server = ChatServerImpl(8080);
    server.start();
}