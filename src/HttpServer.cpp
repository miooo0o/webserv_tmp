#include "HttpServer.hpp"

HttpServer::HttpServer() {};

HttpServer::~HttpServer()
{
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it)
		delete it->second;
}

void	HttpServer::handleEvent(int fd)
{
	if (fd == _serverFd)
	{
		int clientFd = accept(_serverFd, NULL, NULL);
			_poller.addFd(clientFd, POLLIN);
			_clients[clientFd] = new Client(clientFd);
	}
	else
	{
		Client* client = _clients[fd];
		if (client->read() > 0)
		{
			handleRequest(client);
			sendResponse(client);
		}
		else
		{
			_poller.removeFd(fd);
			delete _clients[fd];
			_clients.erase(fd);
			throw std::runtime_error("Failed to read from socket.");
		}
	}
}

void	HttpServer::handleRequest(Client* client)
{
	(void)client;
}

void	HttpServer::sendResponse(Client* client)
{
	(void)client;
}