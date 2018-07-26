#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <boost/algorithm/string.hpp>
#include <vector>

std::vector<std::string> 		ft_read_data(const char* filename)
{
	std::ifstream 				infile(filename);
	std::string 				line;
	size_t						i;
	std::vector<std::string> 	v;

	i = 0;
	while (std::getline(infile, line))
	{
		line = boost::trim_left(line);
		if (regex_match(line, "^[0-9 \t]+$")) // plain line with numbers
			std::cout << "Line with digits: " << line << std::endl;
		else if (regex_match(line, "^[#].+$")) // plain line with comments
			std::cout << "Line of comment: " << line << std::endl;
		else if (regex_match(line, "^[0-9 \t]+[#].+$")) // line with comments and numbers
			std::cout << "Line with digits and comments: " << line << std::endl;
		else if (regex_match(line, "[\n]+") || line[0] == '\n')
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

int main(int argc, char const *argv[])
{
	if (argc == 2)
	{
		try
		{
			ft_val_n_init(ft_read_data(argv[1]));
		}
		catch (std::string msg)
		{
			std::cerr << msg << std::endl;
		}
	}
	else
		std::cout << "Generating pazzle ..." << std::endl;
	return (0);
}