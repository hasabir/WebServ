/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hasabir <hasabir@student.1337.ma>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 20:01:49 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/05/29 17:02:29 by hasabir          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <unistd.h>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <sys/errno.h>
# include <arpa/inet.h>
# include <sys/select.h>
# include <sys/event.h>
# include <sys/time.h>
# include <fcntl.h>
# include <cmath>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cctype>
#include <sys/stat.h>

# define MAX_CONNECTION 355
# define HOST "localhost"
# define MAX_CLIENTS 10
# define CONTINUE "HTTP/1.0 100-continue"
# define CONFIGFILE	"./configFile/server.conf";


struct body
{
	int				get_body_type;
	int				rd_bytes;
	int				chunks_flag;
	int				boundary_flag;
	int				content_length_flag;
	int				cr_nl_flag;
	int				n_chunks;
	int				cr_index;
	unsigned long	chunks_len;
	unsigned long	chunks_con_len;
	unsigned long	content_len;
	std::string		boundary;
};


struct	location
{
	std::string											pattern;
	std::vector<std::string>							allow;//methods allowed in a location
	std::string											root;
	std::string											index;
	std::string											autoindex;
	std::vector<std::pair<std::string, std::string> >	error_page;
	std::string											upload;
	std::string											upload_store;
	std::string											cgi_ext;
	std::string											cgi_path;
	std::string											redirect;
};
// serverfile struct hold information from confige file each variable is hold one value or multiple if it std::string so its hold one value
// and if its std::vector so it hold mutiple value 
// the struct serverfile have internal vector the take a struct location in its template argument
// there some variable that have the same name in serverfile sturct and location struct and thats mean that the variable can
// being in server block and also in location block but the variable in location block override thet one in server block
// except for root varibale (directive) that if there is two root directive in server and location block the root in 
// location block is relative and must be add to the end of root variable in server block and that what i try to do 
// listen = the port(s) witch the server must be listen to
// server_name = the domain name(s) in that server
// error page = it consist of two value error code and default file , default file that needs to be serve is a particular error happen
// max body size = maximum number of byte that allowed to be uploded , only three unit i cover G M K (byte)
// root = the root director in witch you will be search to serve a particular request
// index = default file to serve
// location = block that holde muliple value
// pattern = uri for that loaction 
// allow = allowed methods in this loaction
// autoindex = hold on or off value , allow directory listen in a particular location
// upload = hold on or of value , allow upload of file in a particular location 
// cgi = extension of cgi file or request (i don't know till now) to be serve in a partocular location
// cgi_path = path to the file that will execute for a cgi
struct	serverfile
{
	std::string											host;
	std::vector<std::string>							listen;
	std::vector<std::string>							server_name;
	std::vector<std::pair<std::string, std::string> >	error_page;
	std::string											max_body_size;
	std::string											root;
	std::string											index;
	std::vector<struct location>						location;
};


// client struct the shall be hold the information about the request or response 
// reqest and response is std::vector that take pair as its template arg so you can save key value from the reqest and response
// addr is a sockaddr_storage struct the can serve ipv4 and ipv6 address but we can change it to ipv4 struct cause we just handle ipv4 connections
// len is the length of the addr struct a variable that we need it in a call of some functions
// fd is the file descriptor of the client returned from  accept function

struct uploadFiles
{
	std::string		filename;
	std::fstream	*file;
};

struct client
{
	std::vector<std::pair<std::string, std::string> >	request;
	std::vector<std::pair<std::string, std::string> >	response;
	struct sockaddr_storage								addr;
	socklen_t											len;
	int													fd;
	bool												request_is_ready;
	bool												response_is_ready;
	struct body											bodys;
	std::string											headers;
	std::fstream										*file;			
	std::string											file_name;
	std::string											body_data;
	int													nbr_of_reads;
	int													post_flag;
	std::vector<struct uploadFiles>						upload_files;

	client();
	~client();
	client(const client& rhs);
	client&		operator=(const client& rhs);
};

struct server
{
	std::vector<struct	addrinfo *>	socket;
	std::vector<int>				socketFd;
	struct serverfile				serverConfig;
};

//le request data;
struct	webserv
{
	struct addrinfo						hints;
	std::vector<server>					servers;
	std::vector<client>					clients;
	int									status;
	fd_set								reads;
	fd_set								writes;
	fd_set								exceps;
	int									maxReadFd;
	int									maxWriteFd;
	fd_set								cReads;
	fd_set								tmp_read;
	fd_set								tmp_write;
	std::vector<serverfile>				config;
	int									nbr_of_connections;
};

//handleConnection.cpp
void    handleConnection(struct webserv& web);

//handleRequest.cpp
int		handleContinue(char *line); 
int		endOfTheRequest(std::string buffer, struct body& bodys);
void    closeConnection(struct webserv& web, int client_i);
void    handleRequest(struct webserv& web);


//init/initwebStructAndFree.cpp
void	getServerConfig(struct webserv& web);
void	initWebStrcut(struct webserv& web);
void	freedWeb(struct webserv& web);

//init/displayHostPort.cpp
void    displayHostPort(struct webserv& web);

//init/activeSocket.cpp
void	activeReadSocket(struct webserv& web);
void	activeWriteSocket(struct webserv& web);
void	activeExceptSocket(struct webserv& web);
void	activeSocket(struct webserv& web);

//init/initServer.cpp
int 	initServer(struct webserv& web);

// parse configue file functions



//valides.cpp
int		alphaDigit(int c);
int		getNbr(std::string line);
int		validePort(std::string line);
int		valideDomainName(std::string line);
int		valideFile(std::string line);
int		valideUnit(std::string line);
int		valideOnOff(std::string line);
int		valideExtension(std::string line);
int		valideScript(std::string line);
int		validePath(std::string line);


//parseConfigFile.cpp
int		parseConfigData(std::vector<std::string>& file);
void    parseConfigFile(struct webserv& web, int ac, char **av);

//getConfigData.cpp
std::string 	getDirectiveKey(std::string line);
int 			nbrOfServerBlock(std::vector<std::string> file);
void    		getKeyandValue(std::string& key, std::string& value, std::string line);
void    		getMultivalue(std::vector<std::string>& holder, std::string values);
void    		getPairValue(std::vector<std::pair<std::string, std::string> >& holder, std::string value    );
void    		fillServerBlock(struct webserv& web, std::vector<std::string> serv);
void			getConfigData(struct webserv& web, std::vector<std::string> file);

//parseConfigFun1.cpp
int		isEmptyLines(std::string line);
int		isComment(std::string line);
int		isServerBlock(std::string line);
int		isLocationBlock(std::string line);
int		isBracket(std::string line);
int		endsWithSemiColon(std::string line);
void	countServerBlock(std::vector<std::string> file, int& nbr_of_server);
int		nbrOfLocBlock(std::vector<std::string> file);
int		closedBlock(std::vector<std::string> file);
void	removeEmptyLineAndComments(std::vector<std::string>& file);

//moveBracketsToNextLine.cpp
void	moveBracketsToNextLine(std::vector<std::string>& file);
int		isaBlockWithOpenBrackets(std::string& line);
int		isDirectiveWithCloseBrackets(std::string& line);

//splitSemicolons.cpp
void    removeDoubleSemiColons(std::string& line);
void	splitSemiColons(std::vector<std::string>& file);
int		isAloneSemiColon(std::string  line);
void    removeEmptySemiColons(std::vector<std::string>& file);

//valideServersBlock.cpp
int		valideServerName(std::string line);
int		isAclosedServer(std::vector<std::string> file, int line_index);
int		valideServersBlock(std::vector<std::string> file);

//checkConfigData.cpp
int	checkConfigData(struct webserv& web);

// displayServerFile.cpp
void	displayServerFile(std::vector<struct serverfile> conf);

//response
//getRequestData.cpp
void    getRequestData(struct webserv& web, std::string buff, int index);

//response.cpp
std::string		getRequestValue(struct client clt, std::string key);
struct server   getServerForResponse(struct webserv web, std::string port);
void			getResponse(struct webserv web, struct client& clt);
void			response(struct webserv& web, int fd);
void			responseToRequest(struct server serv, struct client clt, struct respReslt& hepl);

//parseRequest.cpp

void    parseRequest(struct webserv& web, struct client& clt);

// parse request // 
//void	parseRequests(struct webserv web, std::stringstream& buffer);
void    receiveRequest(struct webserv& web, struct client& clt, int i);
void    maxFd(struct webserv& web);
int		toInt(std::string nbr);
void    splitBody(std::string buffer, struct client& clt);
#endif
