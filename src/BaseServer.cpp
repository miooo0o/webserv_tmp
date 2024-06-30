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
	int flags = fcntl(sockfd, F_GETFL, 0);

	if (flags == -1)
	{
		throw std::runtime_error("getting flags");
	}
	flags |= O_NONBLOCK;
	if (fcntl(sockfd, F_SETFL, flags) == -1)
	{
		throw std::runtime_error("setting non-blocking");
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

static void	ft_sigint_handler(int sig)
{
	(void)sig;
	std::cout << "\rserver shutting down..." << std::endl;
	exit(0);
}


void BaseServer::run()
{
	// Ignore SIGPIPE signals
	signal(SIGPIPE, SIG_IGN);
	// Set the signal handler for SIGINT signal
	signal(SIGINT, ft_sigint_handler);

	std::cout << "[Server] waiting for events...." << std::endl;
	while (true)
	{
		_poller.waitForEvents();
		for (size_t i = 0; i < _poller.getPollfds().size(); ++i)
		{
			struct pollfd&  pfd = _poller.getPollfds()[i];
			if (pfd.revents & POLLIN)
			{
				handleEvent(pfd.fd);
			}
			else if (pfd.revents & POLLERR)
			{
				std::cerr << "[Server] error on fd: " << pfd.fd << std::endl;
			}
		}
	}
	std::cout << "[Server] done" << std::endl;
	return ;
}
