#pragma once

#include <unistd.h>
#include <string>
#include <cerrno>

class Client {
public:
	Client(int socketFd);
	virtual ~Client();

	virtual ssize_t	read();
	virtual ssize_t	write(const std::string& response);

	const std::string&	getRequest() const;
	std::string&		getModifyRequest();
private:
	int			_socketFd;
	std::string	_request;
};
