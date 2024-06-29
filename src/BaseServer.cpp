#include "BaseServer.hpp"

BaseServer::BaseServer() : _serverFd(-1) {}

BaseServer::~BaseServer()
{
	if (_serverFd != -1)
		close(_serverFd);
}

void BaseServer::init(int port)
{
	std::cout << "[Server] initialized on port " << port << std::endl;
	struct sockaddr_in	addr;

	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY;
	addr.sin_port = htons(port);

	
	_serverFd = socket(AF_INET, SOCK_STREAM, 0);
	if (bind(_serverFd, (struct sockaddr*)&addr, sizeof(addr)) < 0)
	{
		std::cerr << "[Server] Error binding on port " << port << std::endl;
		throw std::runtime_error("Error binding on port");
	}
	if (listen(_serverFd, 10) < 0)
	{
		std::cerr << "[Server] Error listening on port " << port << std::endl;
		throw std::runtime_error("Error listening on port");
	}
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
