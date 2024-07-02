#pragma once

#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <algorithm>
#include <vector>
#include <poll.h>

struct IsFdMatch
{
	IsFdMatch(int fd) : _fd(fd) {}
	~IsFdMatch() {}

	bool operator()(const struct pollfd& pfd) const
	{ return (pfd.fd == _fd); }

private:
	int _fd;
};

class Poller {
public:
	Poller() {};
	~Poller() {};

	void						addFd(int fd, short events);
	void						removeFd(int fd);
	int							waitForEvents(int timeout = -1);
	std::vector<struct pollfd>&	getPollfds();

private:
	std::vector<struct pollfd>  _pollfds;
};


// 쇼팽 콘체르토 1번