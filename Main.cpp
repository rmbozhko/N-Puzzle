#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

std::vector<std::string>& 		ft_read_data(const char* filename, std::vector<std::string>& v)
{
	std::ifstream 				infile(filename);
	std::string 				line;
	size_t						i;

	i = 1;
	while (std::getline(infile, line))
	{
		ltrim(line);
		if (std::regex_match(line, std::regex("^[0-9 \t]+$"))) // plain line with numbers
			std::cout << "Line with digits: " << line << std::endl;
		else if (std::regex_match(line, std::regex("^[#].+$"))) // plain line with comments
			std::cout << "Line of comment: " << line << std::endl;
		else if (std::regex_match(line, std::regex("^[0-9 \t]+[#].+$"))) // line with comments and numbers
			std::cout << "Line with digits and comments: " << line << std::endl;
		else if (std::regex_match(line, std::regex("^[ \t\n]*$")) || line[0] == '\n')
			std::cout << "Line with newlines chars" << std::endl;
		else
		{
			line = std::string("Error: Invalid syntax on line #") + std::to_string(i); 
			throw line;
		}
		i++;
		v.push_back(line);
	}

	return (v);
}

bool						ft_val_n_init(std::vector<std::string>& data)
{
	std::cout << "Starting initialization..." << std::endl;
	return (true);
}

int main(int argc, char const *argv[])
{
	if (argc == 2)
	{
		std::vector<std::string> 		v;
		try
		{
			ft_val_n_init(ft_read_data(argv[1], v));
		}
		catch (std::string msg)
		{
			std::cerr << msg << std::endl;
		}
	}
	else
		std::cout << "Generating puzzle ..." << std::endl;
	return (0);
}