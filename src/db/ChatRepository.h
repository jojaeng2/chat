#ifndef CHAT_REPOSITORY_H
#define CHAT_REPOSITORY_H
#include <mysqlx/xdevapi.h>
#include <string>
using namespace std;
using namespace mysqlx;

class ChatRepository {
    public:
        ChatRepository(std::string tableName);
        ChatRepository(const ChatRepository&) = delete; 
        void save(int clientId, std::string message);
        void close();


    private:
        Session session;
        Schema db;
        Table table;
};

#endif