#pragma once

#include <map>
#include "Poller.hpp"
#include "Client.hpp"

class BaseServer {
public:
	BaseServer();
	virtual ~BaseServer();

	void NewFunction();

	void init(int port);
	void run();

	// std::string	getType() const { return (_type); }

protected:
	virtual void handleEvent(int fd) = 0;

	int						_serverFd;
	Poller					_poller;
	std::map<int, Client*>	_clients;
	std::string				_type;
};
