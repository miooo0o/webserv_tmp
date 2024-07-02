#pragma once

#include "Client.hpp"

class RequestHandler {
public:
    virtual ~RequestHandler() {}
    virtual void handle(Client* client) = 0;
};
