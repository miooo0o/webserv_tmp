#pragma once

#include "BaseServer.hpp"
#include "Client.hpp"
#include <fstream>

class HttpServer : public BaseServer {
public:
	HttpServer();
	~HttpServer();

protected:
	void    handleEvent(int fd);

	void    handleRequest(Client* client);
	void    sendResponse(Client* client);
	void	closeClient(int fd);
	std::string     handleError(int type);
	std::string     handleGet(std::string& path);

};
