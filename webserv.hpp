/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hp <hp@student.42.fr>                      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/08 20:01:49 by tel-bouh          #+#    #+#             */
/*   Updated: 2023/07/10 13:09:21 by hp               ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# include <unistd.h>
# include <cstring>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <sys/errno.h>
# include <arpa/inet.h>
# include <sys/select.h>
// # include <sys/event.h>
# include <sys/time.h>
# include <fcntl.h>
# include <cmath>
# include <vector>
# include <map>
# include <iostream>
# include <sstream>
# include <fstream>
# include <string>
# include <cctype>
# include <sys/stat.h>
# include <cstdio>
# include <dirent.h>
# include <algorithm>
# include <cstdlib>
#include <utility>


# define MAX_CONNECTION 355
# define HOST "localhost"
# define MAX_CLIENTS 10
# define CONTINUE "HTTP/1.0 100-continue"
# define CONFIGFILE	"./configFile/server.conf";
# define MAXINT	100000
# define CRLF "\r\n"


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
	int				content_disposition;
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
	std::vector<std::pair<std::string, std::string> >	cgi;
	std::string											redirect;
};

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

struct uploadFiles
{
	unsigned long 	len;
	std::string		filename;
	std::fstream	*file;


	uploadFiles();
	~uploadFiles();
	uploadFiles(const uploadFiles& rhs);
	uploadFiles&	operator=(const uploadFiles& rhs);
};

struct Response
{
	public:
		bool				header;
		bool				error;
		bool				finishReading;
		bool				autoindex;
		bool				generateError;
		bool				body;
		int					statusCode;
		int					nbrFrames;
		long				sizeFrame;
		long				fileSize;
		long				len;
		std::string			filePath;
		std::streampos		position;
		std::vector<char>	responseBody;
		std::vector<char>	responseData;
		std::string			uri;

		Response():header(0), nbrFrames(-1),
		finishReading(0), autoindex(0),generateError(0),
		body(0), statusCode(0){};
};

class CGI
{
	public:
		std::map<std::string, std::string>	cgi_ENV;
		std::map<std::string, std::string>	query;
		std::vector<std::string> env;
};
struct client
{
	std::map<std::string, std::string>	map_request;	
	Response							response;
	CGI									cgi;
	
	int 								config;
	int									location;
	std::string							port;

	bool								response_is_ready;
	struct sockaddr_storage				addr;
	socklen_t							len;
	int									fd;
	bool								request_is_ready;
	struct body							bodys;
	std::string							headers;
	std::fstream						*file;			
	std::string							file_name;
	std::string							body_data;
	int									nbr_of_reads;
	int									post_flag;
	std::vector<struct uploadFiles>		upload_files;
	unsigned long						body_length;

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
	int									maxReadFd;
	int									maxWriteFd;
	fd_set								tmp_read;
	fd_set								tmp_write;
	std::vector<serverfile>				config;
	int									nbr_of_connections;
	unsigned int						req_nbr;
	unsigned int						next_read;
	unsigned int 						next_write;
};

/*     ***********************************************     */
// repo name : acceptConnectionAndGetRequestData
// file name : handleConnection.cpp
// fun  name :
void    closeConnection(struct webserv& web, int client_i);
void    handleConnection(struct webserv& web);

// repo name : acceptConnectionAndGetRequestData
// file name : recieveRequest.cpp
// fun  name :
void	maxFd(struct webserv& web);
void	receiveRequest(struct webserv& web, struct client& clt, int clt_i, int& flag_fail);

// repo name : acceptConnectionAndGetRequestData
// file name : endOfRequest.cpp
// fun  name :
int				endOfTheRequest(std::string buffer, struct body& bodys);
int				endOfChunks(std::string	buffer, struct body& bodys);
void			getBodyType(std::string buffer, struct body& bodys);
int				handleContinue(char	*line);
int				toInt(std::string nbr);
unsigned long	hexToDec(std::string hex);

// repo name : acceptConnectionAndGetRequestData
// file name : multiTypes.cpp
// fun  name :
void	multiTypes(std::string buffer, struct client& clt);
void	getFilename(std::string buffer, int file_index, struct uploadFiles& upload_files, int find, int fd);
int		isADerective(std::string buffer, int find, int size);
void	getFileSize(struct uploadFiles& file);

// repo name : acceptConnectionAndGetRequestData
// file name : splitBody.cpp
// fun  name :
void	addHeaders(std::string buffer, struct client& clt, int pos);
void	splitBody(std::string buffer, struct client& clt);

// repo name : acceptConnectionAndGetRequestData
// file name : getFilesLength.cpp
// fun  name :
void    getFilesLength(struct client& clt);
/*     ***********************************************     */


/*     ***********************************************     */
// repo name : parseConfigFile
// file name : parseConfigFile.cpp 
// fun  name :
int		parseConfigData(std::vector<std::string>& file);
void    parseConfigFile(struct webserv& web, int ac, char **av);

// repo name : parseConfigFile
// file name : getConfigData.cpp 
// fun  name :
std::string 	getDirectiveKey(std::string line);
int 			nbrOfServerBlock(std::vector<std::string> file);
void    		getKeyandValue(std::string& key, std::string& value, std::string line);
void    		getMultivalue(std::vector<std::string>& holder, std::string values);
void    		getPairValue(std::vector<std::pair<std::string, std::string> >& holder, std::string value    );
void    		fillServerBlock(struct webserv& web, std::vector<std::string> serv);
void			getConfigData(struct webserv& web, std::vector<std::string> file);

// repo name : parseConfigFile
// file name : parseConfigFun1.cpp  
// fun  name :
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

// repo name : parseConfigFile
// file name : moveBracketsToNextLine.cpp  
// fun  name :
void	moveBracketsToNextLine(std::vector<std::string>& file);
int		isaBlockWithOpenBrackets(std::string& line);
int		isDirectiveWithCloseBrackets(std::string& line);

// repo name : parseConfigFile
// file name : splitSemicolons.cpp 
// fun  name :
void    removeDoubleSemiColons(std::string& line);
void	splitSemiColons(std::vector<std::string>& file);
int		isAloneSemiColon(std::string  line);
void    removeEmptySemiColons(std::vector<std::string>& file);

// repo name : parseConfigFile
// file name : valideServersBlock.cpp  
// fun  name :
int		valideServerName(std::string line);
int		isAclosedServer(std::vector<std::string> file, int line_index);
int		valideServersBlock(std::vector<std::string> file);

// repo name : parseConfigFile
// file name : checkConfigData.cpp  
// fun  name :
int		DuplicatePort(std::string listen, std::vector<std::string> port);
int		valideListenDirective(std::vector<std::string>& listen, std::vector<std::string>& port);
int		validHostDirective(std::string& host);
int		checkConfigData(struct webserv& web);

// repo name : parseConfigFile
// file name : displayServerFile.cpp
// fun  name :
void	displayServerFile(std::vector<struct serverfile> conf);

// repo name : parseConfigFile
// file name : valides.cpp
// fun  name :
int		getNbr(std::string line);
int		validePort(std::string line);
int		alphaDigit(int c);
int		valideDomainName(std::string line);
int		valideFile(std::string line);
int		valideUnit(std::string line);
int		valideOnOff(std::string line);
int		valideExtension(std::string line);
int		valideScript(std::string line);
int		validePath(std::string line);
/*     ***********************************************     */


/*     ***********************************************     */
// repo name : init
// file name : initWebStructAndFree.cpp
// fun  name :
void	getServerConfig(struct webserv& web);
void	initWebStrcut(struct webserv& web);
void	freedWeb(struct webserv& web);

// repo name : init
// file name : displayHostPort.cpp
// fun  name :
void    displayHostPort(struct webserv& web);

// repo name : init
// file name : activeSocket.cpp
// fun  name :
void	activeReadSocket(struct webserv& web);
void	activeWriteSocket(struct webserv& web);
void	activeSocket(struct webserv& web);

// repo name : init
// file name : initServer.cpp
// fun  name :
int 	initServer(struct webserv& web);

/*     ***********************************************     */


/* ************************** utils.cpp ********************************************* */
unsigned long	stringToInt(std::string str);
std::vector<int>	search2(struct client &clt, struct webserv &web);
int				search(struct client &clt, struct webserv &web);
std::string		replaceLocation(std::string uri, std::string pattern, std::string root);
std::string		intToString(int n);
void			fillMapContentTypes(std::map<std::string, std::string> &contentTypes);
std::string		host(std::string host);
std::string		getStatusMessage(int statusCode);
int error(struct client &clt,int statusCode);

/* ************************** parseRequest ****************************************** */

int				parseRequest(struct webserv& web, struct client& clt);
void			fillRequestData(struct client& clt);
int				isRequestWellFormed(struct client &clt, struct webserv &web);

/* ************************* sendResponse ****************************************** */

int				sendResponse(struct client &clt, struct webserv &web, int statusCode);
std::string		readFileContent(std::string &filePath, int statusCode);
void			getResponse(struct client &clt, int statusCode,
					std::string &response, std::string responseContent, std::string filePath);
void			getResponseHeader(struct client &clt, int statusCode, std::string filePath);
/* ************************** responseUtils *************************************** */

std::string 	getFilePath(struct client& clt, struct webserv &web, int statusCode);
std::string 	getContentType(std::string filePath);
std::string 	decimalToHex(int decimalNumber);
void			initData(struct client &clt, std::string filePath, std::ifstream &file);


/* ************************** seneErrorResponse *********************************** */

void	readeErrorFile(struct client &clt, int statusCode);
void	fillErrorResponse(struct client &clt, struct webserv &web, int statusCode);
void	getResponseHeaderError(struct client &clt, int statusCode);

/* ************************** sendRedirecResponse ********************************* */

void	fillRedirectResponse(struct client &clt, struct webserv &web, int statusCode);

/* ************************** cgi ************************************************* */
int cgi(struct webserv &web, struct client &clt);

/**************************************************************************************/


int		get(struct webserv& web, struct client& clt);
void	post(struct webserv& web, struct client& clt);
void	deleteResponse(struct webserv& web, struct client& clt);


#endif
