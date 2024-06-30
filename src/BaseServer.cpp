#include "BaseServer.hpp"

BaseServer::BaseServer() : _serverFd(INVALID_SOCKET_FD) {}

BaseServer::~BaseServer()
{
	if (_serverFd != INVALID_SOCKET_FD)
		close(_serverFd);
}

void BaseServer::createSocket()
{
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (_serverFd < 0)
	{
		throw std::runtime_error("creating socket");
	}
}

void BaseServer::setSocketOptions()
{
	int opt = 1;
	if (setsockopt(_serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0)
	{
		throw std::runtime_error("setsockopt SO_REUSEADDR failed");
	}
}

void BaseServer::bindSocket(int port)
{
	struct sockaddr_in addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	if (bind(_serverFd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		throw std::runtime_error("binding socket");
	}
}


void BaseServer::startListening()
{
	if (listen(_serverFd, 10) < 0)
	{
		throw std::runtime_error("listening on socket");
	}
}

void BaseServer::setNonBlocking(int sockfd)
{
	int flags = 1;
	if (ioctl(sockfd, FIONBIO, &flags) < 0)
	{
		throw std::runtime_error("ioctl FIONBIO failed");
	}
}

void BaseServer::init(int port)
{
	std::cout << "[Server] initialized on port " << port << std::endl;

	createSocket();
	setSocketOptions();
	bindSocket(port);
	setNonBlocking(_serverFd);
	startListening();

	_poller.addFd(_serverFd, POLLIN);
	std::cout << "[Server] initialized: " << port << std::endl;

}

void BaseServer::run()
{
	std::cout << "[Server] waiting for events...." << std::endl;
	while (true)
	{
		// int eventCount = _poller.waitForEvents();
		_poller.waitForEvents();
		for (size_t i = 0; i < _poller.getPollfds().size(); ++i)
		{
			struct pollfd&  pfd = _poller.getPollfds()[i];
			if (pfd.revents & POLLIN)
				handleEvent(pfd.fd);
		}
	}
	std::cout << "[Server] done" << std::endl;
	return ;
}
