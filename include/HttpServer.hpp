#pragma once

#include "BaseServer.hpp"
#include "Client.hpp"

class HttpServer : public BaseServer {
public:
    HttpServer();
    ~HttpServer();

protected:
    void handleEvent(int fd);

    void handleRequest(Client* client);
    void sendResponse(Client* client);
};
