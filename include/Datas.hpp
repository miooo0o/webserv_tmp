# include "WebServ.hpp"

typedef enum s_event
{ 
	SERVER,
	CLIENT,
	WORK,
	LOGGER
} e_event;

typedef struct s_http
{

} t_http;

class IBase
{
 private:
	e_event _type;
	int _fd;

 public:
	IBase(e_event type, int fd);
	~IBase();
	void		setType(e_event type);
	e_event		getType() const;
	int			getFd() const;
	void		setFd(int fd);
};

typedef std::map<std::string, std::string> config_map;

typedef struct s_server {
	std::string							port;					// port number
	int									server_fd;				// server fd
	std::map<std::string, std::string>	config;			// main config value map
//   config_map	config;			// main config value map
} t_server;


class Logger : public IBase
{
 public:
	Logger();
	~Logger();

};

class Server : public IBase
{
 private:
	t_server	_server_config;
	
 public:
	Server();
	~Server();
	
	IBase*		createClient(int cleint_fd);

	t_server&	getServer();
};