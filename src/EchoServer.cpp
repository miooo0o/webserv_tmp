#include "EchoServer.hpp"
#include <cerrno>
#include <string.h>

EchoServer::EchoServer() {}

EchoServer::~EchoServer()
{
	std::map<int, Client*>::iterator it;
	for (it = _clients.begin(); it != _clients.end(); ++it)
		delete it->second;
}

void EchoServer::handleEvent(int fd) 
{
	if (fd == _serverFd)
	{
		int clientFd = accept(_serverFd, NULL, NULL);
		if (clientFd < 0)
		{
			std::cerr << "[Echo Server] Error accepting new connection: " << strerror(errno) << std::endl;
			return;
		}
		std::cout << "[Echo Server] New client connected: " << clientFd << std::endl;
		_clients[clientFd] = new Client(clientFd);
		_poller.addFd(clientFd, POLLIN);
	}
	else
	{
		if (_clients.find(fd) == _clients.end())
			throw std::runtime_error("[Echo Server] Client not found");

		Client* client = _clients[fd];
		ssize_t bytesRead = client->read();
		if (bytesRead > 0)
			echoing(fd);
		else if (bytesRead == 0)
		{
			_poller.removeFd(fd);
			delete _clients[fd];
			_clients.erase(fd);
			std::cout << "[Echo Server] Client disconnected: " << fd << std::endl;
		}
		else
		{
			_poller.removeFd(fd);
            delete _clients[fd];
            _clients.erase(fd);
			std::cerr << "[Echo Server] Error reading from socket: " << strerror(errno) << std::endl;
			throw std::runtime_error("Failed to read from socket.");
		
		}
	}
}

void EchoServer::echoing(int fd)
{
	Client* client = _clients[fd];
	std::string& buffer = client->getModifyRequest();

	if (!buffer.empty())
	{
		std::cout << "[Echo Server] Received: " << buffer;
		write(fd, buffer.c_str(), buffer.size());
		buffer.clear();
	}
	else
	{
		std::cerr << "[Echo Server] Error reading from socket: " << strerror(errno) << std::endl;
		if (errno == ENOTCONN)
		{
			std::cerr << "[Echo Server] Socket " << fd << " is not connected. Closing socket." << std::endl;
			close(fd);
		}
		throw std::runtime_error("Failed to read from socket.");
	}
}