/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: anammal <anammal@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 10:23:38 by araiteb           #+#    #+#             */
/*   Updated: 2024/04/15 20:19:42 by anammal          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(std::string ipAdd, int port): m_pass(ipAdd),m_port(port)
{
	this->name = ":42_IRC ";
	this->num = 1;
	this->user_num = 1;
	this->on = 1;
	this->timeout = 3 * 60 * 10000000;
	time_t now = time(0);
	this->birthday = ctime(&now);
	memset(&address, 0, sizeof(address));
	memset(users, 0, sizeof(users));
}

Server::Server(Server &sr)
{
	*this = sr;
}

Server&	Server::operator=(const Server &sr)
{
	m_port = sr.m_port;
	m_socket = sr.m_socket;
	return (*this);
	
}

Server::~Server(){
	std::map<int, Client *>::iterator it;
	it = this->clients.begin();
	while (it != this->clients.end())
	{
		close(it->first);
		delete it->second;
		it++;
	}
	this->clients.clear();
	std::map<std::string, Channel *>::iterator it2 = this->channels.begin();
	while (it2 != channels.end())
	{
		delete it2->second;
		it2++;
	}
	channels.clear();	
	close(server_fd);
}


void 	Server::seTpass(std::string pass)
{
	this->m_pass = pass;
}
void 	Server::seTport(int p)
{
	this->m_port = p;
}
void 	Server::seTmsocket(int sock)
{
	this->m_socket = sock;
}
void 	Server::seTServerFd(int fd_serv)
{
	this->server_fd = fd_serv;
}
void 	Server::seTmapClient(std::map<int, Client *> mapClient)
{
	this->clients = mapClient;
}

std::string		Server::getPass()
{
	return (this->m_pass);
}
int 	Server::getport()
{
	return this->m_port;
}
int 	Server::getsocket()
{
	return this->m_socket;
}
int 	Server::getSeverSocket()
{
	return this->server_fd;
}
std::map<int, Client *>	Server::getClient(){
	return this->clients;
}

std::string 	Server::getNick(){
	return (this->name);
}
int	Server::CreateSocket()
{
	std::cout << "building socket ... " ;
	this->server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->server_fd < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		return 0;
	}
	std::cout << " created" << std::endl;
	return 1;
}
int	Server::OptionSocket()
{
	std::cout << "making socket reusable ... ";
	int rec = setsockopt(this->server_fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));
	if (rec < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		close (this->server_fd);
		return 0;
	}
	std::cout << " Finished." << std::endl;
	return 1;
}

int	Server::NnBlockFd()
{
	std::cout << "making socket non bloking ... ";
	int rec = fcntl(this->server_fd, F_SETFL, O_NONBLOCK);
	if (rec < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		close(this->server_fd);
		return 0;
	}
	std::cout << " Finished." << std::endl;
	return 1;
}

int		Server::BindSocket()
{
	std::cout << "binding socket . . . ";
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(this->getport());
	int rec = bind(this->server_fd, (struct sockaddr *)&address, sizeof(address));
	if (rec < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		close(this->server_fd);
		return 0;
	}
	std::cout << " Finished." << std::endl;
	return 1;
}
int	Server::listenSocket()
{
	std::cout <<  "Making socket lisening. . . ";
	int rec = listen(this->server_fd, LIMITCNX);
	if (rec < 0)
	{
		std::cout << " failed : " << errno << std::endl;
		close(this->server_fd);
		return 0;
	}
	std::cout << " Finished." << std::endl;
	return 1;
}

void 	sendResponce(int fd, const std::string &responce)
{
	send(fd, responce.c_str(), responce.length(), 0);
}

void 	Server::TraiteMessage(Message &msg) {
	std::vector<std::string> SplitedMsg;

	if (!msg.getMessage().empty()) {
		if (!split(msg.getMessage(), SplitedMsg))
			return ;
		this->commands(msg, SplitedMsg);
		SplitedMsg.clear();
	}
}

void	Server::clientLeft(int fd) {
	try
	{
		std::map<int, Client *>::iterator		client;

		client = this->clients.find(fd);
		if (client != this->clients.end()) {
			if (!client->second->geTPass().empty())
				close (fd);
			delete client->second;
			this->clients.erase(client);
		}
	} catch (std::exception &e) {}
}

void Server::quitServer() {
    close (this->server_fd);
	this->on = 1;
    this->~Server();
    exit (EXIT_FAILURE);
}

int 		Server::acceptingData(){
	int newfd = -1;
	struct sockaddr_in newAddresse;
	int lenadd = sizeof(newAddresse);
	do
	{
	   	newfd = accept (this->server_fd, (struct sockaddr *)&newAddresse, (socklen_t *)&lenadd);
		if (newfd < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cout << "Failed : " << errno << std::endl;
				return 0;
			}
			break ;
		}
		Client *c = new Client(newfd);
		c->sethostname(inet_ntoa(newAddresse.sin_addr));
		this->clients.insert(std::pair<int, Client *>(newfd, c));
		users[user_num].fd = newfd;
		users[user_num]. events = POLLIN;
		user_num++;

        // sendResponce(newfd, this->name + "NOTICE AUTH :*** Looking up your hostname . . .\n");
	    // sendResponce(newfd, this->name + "NOTICE AUTH :*** Found your hostname\n");
	} while (newfd != -1);
	return 1;
}

int 	Server::checkmsg(int fd){

	std::cout << " Receiving msg . . . " ;

	int			rec;
	std::string	msg;

	msg = "";
	memset(buffer, 0, sizeof(buffer));
	do
	{
		rec = recv(fd, buffer, sizeof(buffer), 0);
		if (rec < 0)
		{
			if (errno != EWOULDBLOCK)
			{
				std::cout << "Failed at receiving msg : " << strerror( errno )  << std::endl; 
				return 0;
			}
			continue ;
		}
		if (rec == 0)
		{
			std::cout << " Connection  closed " << std::endl;
			this->clientLeft(fd);
			return 0;
		}
		buffer[rec] = '\0';
		msg += buffer;
		if (msg.find_first_of("\r\n") != std::string::npos && msg != "\n")
		{
			size_t pos = msg.find_last_of("\r\n");
			msg = msg.substr(0, pos);
			Message mesg = Message(fd, msg);
			TraiteMessage(mesg);
			return 1;
		}
		else
			return 1 ;
	}while(1); // end of accept function
	return 1;
}

void	Server::PollingFd()
{

	users[0].fd = server_fd;
	users[0].events = POLLIN;
	std::cout << "Server is running . . . " << std::endl;
	do
	{
		int flg = poll(users , user_num, timeout);
		if (flg < 0)
		{
			std::cout << "Failed." << errno <<std::endl;
			this->quitServer();
		}
		if (flg == 0)
		{
			std::cout << "End program : time out" << std::endl;
			this->quitServer();
		}
		num = user_num;
		for (int i = 0; i < this->user_num; i++)
		{
			if (users[i].revents == 0)
				continue;
			if (this->users[i].fd == this->server_fd)
			{
				std::cout << "waiting for accept new connections" << std::endl;
				if (!acceptingData())
					this->quitServer();
			}
			else {
				if (!this->checkmsg(users[i].fd)) {
					this->clientLeft(users[i].fd);
					for (int j = i; j < this->user_num; j++)
                        memcpy(&this->users[j], &this->users[j + 1], sizeof(struct pollfd));
                    this->user_num -= 1;
                    i --;
				}
			}
		}
	} while (1);
}

int	Server::IsAuthorized(Client& client) {
	if (client.getNick().empty() || client.geTPass().empty() || client.getusername().empty())
		return 0;
	if (client.geTPass().compare(this->m_pass))
		return 2;
	return 1;
}

std::string Server::to_string(int number)
{
    std::stringstream ss;
    ss << number;
    return (ss.str());
}

Client*	Server::getClientByFd(int fdUser)
{
	Client *c;
	if (this->clients.find(fdUser) != this->clients.end())
	{
		c = this->clients.find(fdUser)->second;
		return c;
	}
	std::cout << "getting : " << fdUser << std::endl;
	return NULL;
}

Client*	Server::getClientByNickname(std::string nickname)
{
	std::map<int , Client *>::iterator it;

	for(it = this->clients.begin(); it != this->clients.end(); ++it)
	{
		if (it->second->getNick() == nickname)
			return (it->second);
	}
	return NULL;
}
