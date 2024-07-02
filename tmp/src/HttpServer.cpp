#include "HttpServer.hpp"
#include  <sstream>

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
		ssize_t bytesRead = client->read();
		if (bytesRead > 0)
		{
			handleRequest(client);
			sendResponse(client);
			closeClient(fd);
		}
		else if (bytesRead == 0)
		{
			closeClient(fd);
			std::cout << "[Http Server] Client disconnected: " << fd << std::endl;
		}
		else
		{
			closeClient(fd);
			throw std::runtime_error("Failed to read from socket.");
		}
	}
}

void	HttpServer::closeClient(int fd)
{
	close(fd);
	_poller.removeFd(fd);
	delete _clients[fd];
	_clients.erase(fd);
}


std::string HttpServer::handleError(int type)
{
	std::string response;

	switch (type)
	{
		case 405:
			response = std::string("HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 0\r\n\r\n");
			break;
		case 404:
			response = std::string("HTTP/1.1 404 Not Found\r\nContent-Length: 0\r\n\r\n");
			break;
		case 500:
			response = std::string("HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n");
			break;
	}
	return (response);
}


std::string HttpServer::handleGet(std::string& path)
{
	std::string file_path = "." + path;
	std::ifstream ifs (file_path.data());

	std::stringstream response;

	if (!ifs.is_open())
	{
		return (handleError(404));
	}
	response << "HTTP/1.1 200 OK\r\n";
	response << "Content-Type: text/html\r\n";
	response << "Connection: close\r\n\r\n";
	response << ifs.rdbuf();

	return (response.str());
}

void	HttpServer::handleRequest(Client* client)
{
	std::istringstream	request_stream(client->getRequest());
	std::string			method, path, version;

	request_stream >> method >> path >> version;
	std::cout << "\r" << method << " | " << path << " | " << version << std::endl;

	if (method == "GET")
	{
		client->setResponse(handleGet(path));
	}
	else
		client->setResponse(handleError(405));
}

void	HttpServer::sendResponse(Client* client)
{
	client->write(client->getResponse());
}