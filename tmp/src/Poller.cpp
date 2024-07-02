#include "Poller.hpp"

/// @brief Add a file descriptor to the poller
/// @param fd The file descriptor to add
/// @param events The events to listen for
void	Poller::addFd(int fd, short events)
{
	struct pollfd pfd = {fd, events, 0};
	_pollfds.push_back(pfd);
}

/// @brief removes a file descriptor from the list of file descriptors that the Poller is monitoring.
/// @param fd The file descriptor to remove
void	Poller::removeFd(int fd)
{
	std::vector<struct pollfd>::iterator it = std::remove_if(
		_pollfds.begin(),
		_pollfds.end(),
		IsFdMatch(fd)
		);
		_pollfds.erase(it, _pollfds.end());
}

int		Poller::waitForEvents(int timeout)
{
	return (poll(&_pollfds[0], _pollfds.size(), timeout));
}

std::vector<struct pollfd>& Poller::getPollfds(void)
{
	return (_pollfds);
}

