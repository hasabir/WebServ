#include "webserv.hpp"

int	main()
{
	std::fstream 	s;
	char			line[100];
	std::string		reads;

	s.open("req11.txt");
	while (1)
	{
		s.read(line, 99);
		reads.assign(line);
		if (reads.size() == 0)
			break;
		int i = 0;
		while (i < reads.size())
		{
			if (reads[i] == '\r')
				std::cout << "\\r";
			else 
			{
				if (reads[i] == '\n')
					std::cout << "\\n";
				std::cout << reads[i];
			}
			i++;
		}
		std::string temp = reads.substr(reads.size() - 5, 5);
		if (temp.size() >= 5 && temp == "0\r\n\r\n")
		{
			std::cout << "end of file  : " << temp.size() <<  std::endl;
			std::cout << "start [" << temp[0] << "end [" << temp[4] << "]" << std::endl;
			temp.assign("");
		}
		//else
		//	std::cout << "Not valid ." << std::endl;
		reads.assign("");
		memset(line, 0 , 100);
	}
}
