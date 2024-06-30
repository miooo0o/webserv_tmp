#include "Client.hpp"
#include <iostream>
#include <string.h>
#include <sstream>
#include <cstring>

Client::Client(int socketFd)
	: _socketFd(socketFd) {}

Client::~Client() { close(_socketFd); }

ssize_t Client::read()
{
	char buffer[1024];

	ssize_t totalBytesRead = 0;
	ssize_t bytesRead;
	std::ostringstream oss;
	while ((bytesRead = ::read(_socketFd, buffer, sizeof(buffer))) > 0)
	{
		oss.write(buffer, bytesRead);
		totalBytesRead += bytesRead;
		if (std::memchr(buffer, '\n', bytesRead))
			break;
	}
	if (totalBytesRead > 0)
		_request.append(oss.str());
	if (bytesRead < 0)
	{
		std::cerr << "[Client] Error reading from socket: " << strerror(errno) << std::endl;
		throw std::runtime_error("Error reading from socket");
	}
	return (totalBytesRead);
}

ssize_t Client::write(const std::string& response)
{
	return (::write(_socketFd, response.c_str(), response.size()));
}


const std::string& Client::getRequest() const
{
	return (_request);
}

std::string& Client::getModifyRequest()
{
	return (_request);
}

std::string& Client::getResponse()
{
	return (_response);
}

void Client::setResponse(const std::string& response)
{
	_response = response;
}