// #include "HttpServer.hpp"
#include "EchoServer.hpp"

int main()
{
	EchoServer server;

	try
	{
		server.init(8080);
		server.run();   
	}
	catch(const std::exception& e)
	{
		std::cerr << "[Error] "<< e.what() << std::endl;
	}
	return 0;
}