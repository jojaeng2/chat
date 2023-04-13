#include <string>
#include <mysqlx/xdevapi.h>
#include "ChatRepository.h"
#include <iostream>
using namespace std;
using namespace mysqlx;


ChatRepository::ChatRepository(std::string tableName)
    : session("localhost", "root", ""), db(session.getSchema("test")), table(db.getTable(tableName))
{}


void ChatRepository::save(int clientId, std::string message) {
    table.insert("client_id", "message", "created").values(clientId, message, '"NOW()"').execute();
}

void ChatRepository::close() {
    session.close();
}
