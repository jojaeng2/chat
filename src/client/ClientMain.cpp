#include "ChatClient.h"
#include "ChatClient.cpp"
using namespace std;

int mian() {
    ChatClientImpl client = ChatClientImpl(8080);
    client.start();
}