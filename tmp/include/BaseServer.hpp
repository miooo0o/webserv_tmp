#pragma once

# include <map>
# include <sys/ioctl.h>
# include <unistd.h>
# include <fcntl.h>
# include <signal.h>

# include "Poller.hpp"
# include "Client.hpp"

#define INVALID_SOCKET_FD -1

class BaseServer {
public:
	BaseServer();
	virtual ~BaseServer();

	void init(int port);
	void run();

protected:
	void createSocket();
    void setSocketOptions();
    void bindSocket(int port);
    void startListening();
    void setNonBlocking(int sockfd);

	virtual void handleEvent(int fd) = 0;

	int						_serverFd;
	Poller					_poller;
	std::map<int, Client*>	_clients;
};
