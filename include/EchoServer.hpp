#include "BaseServer.hpp"

class EchoServer : public BaseServer {
public:
    EchoServer();
    virtual ~EchoServer();

    virtual void handleEvent(int fd);
	void	echoing(int fd);

// protected:
};