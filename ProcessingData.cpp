#include "NPuzzle.hpp"

namespace NPuzzle
{
	std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
	{
	    str.erase(0, str.find_first_not_of(chars));
	    return (str);
	}

	std::string& rtrim(std::string& s, const std::string& delimiters = " \f\n\r\t\v" )
	{
	   s.erase( s.find_last_not_of( delimiters ) + 1 );
	   return (s);
	}

	size_t*		CopyVectorToPtr(std::vector<size_t>& v)
	{
		size_t*		field = new size_t [v.size() * v.size()];
			for (size_t i = 0; i < v.size() * v.size(); ++i)
				field[i] = v[i];
		return (field);
	}

	std::pair<Solver*, State*> 		ReadData(const char* filename)
	{
		std::ifstream 				infile(filename);
		std::string 				line;
		size_t						i;
		std::smatch					m;
		HEURISTIC					found = MANHATTAN;
		bool						is_unicost = true;
		
		std::vector<size_t> 		fileField;
		size_t						size;
		Heuristic 				h_func_;

		i = 1;
		size = 0;
		while (std::getline(infile, line))
		{
			line = rtrim(ltrim(line));
			if (std::regex_match(line, m, std::regex("^[0-9 \t]+$")) ||
				(std::regex_match(line, m, std::regex("^[0-9 \t]+[#].+$")))) // plain line with numbers
			{
				// extracting comment line of present
				if (line.find("#") != std::string::npos)
					rtrim(line.erase(line.find("#")));
				// looking for puzzle size
				if (std::regex_search(line, m, std::regex("^[0-9 \t]+")))
				{
					std::regex 		temp("[0-9]+");
					auto start = std::sregex_iterator(line.begin(), line.end(), temp);
					auto end = std::sregex_iterator(); 
					if (std::regex_match(m[0].str(), m, std::regex("^[0-9]+")) && std::distance(start, end) == 1)
					{
						if (size == 0)
							size = static_cast<size_t>(atoi(m[0].str().c_str()));
						else
							throw std::string("Error: Puzzle size overwriting on line #") + std::to_string(i); 
					}
					else if (std::distance(start, end) == size)
					{
						for (; start != std::sregex_iterator(); start++ )
						{
							size_t		elem = static_cast<size_t>(atoi((*start).str().c_str()));
							if (elem >= (size * size))
								throw std::string("Error: Puzzle data is greater than upper bound on the line #") + std::to_string(i);
							else if ((std::find(fileField.begin(), fileField.end(), elem) != fileField.end()))
								throw std::string("Error: Puzzle data is repeated on the line #") + std::to_string(i);
							else
								fileField.push_back(elem);
						}
					}
					else
						throw std::string("Error: Puzzle data doesn't correspond to size on the line #") + std::to_string(i);
				}
			}
			else if (std::regex_match(line, m, std::regex("^[#].+$"))) // plain line with comments
				std::cout << "Line of comment: " << m[0].str() << std::endl;
			else if (std::regex_match(line, m, std::regex("^[ \t\n]*$")) || line[0] == '\n')
				std::cout << "Line with newlines chars" << std::endl;
			else if (std::regex_match(line, m, std::regex("^%euclide$")))
				found = EUCLIDE;
			else if (std::regex_match(line, m, std::regex("^%lin_conf$")))
				found = LIN_CONF;
			else if (std::regex_match(line, m, std::regex("^%mis_tiles$")))
				found = MIS_TILES;
			else if (std::regex_match(line, m, std::regex("^%greddy$")))
				is_unicost = false;
			else
			{
				line = std::string("Error: Invalid syntax on line #") + std::to_string(i);
				throw line;
			}
			i++;
		}
		if (fileField.size() && (fileField.size() == (size * size)) && (std::find(fileField.begin(), fileField.end(), 0) != fileField.end()))
		{
			Solver* 	solv = new Solver(size, NPuzzle::Solver::GenerateFinalState(size), "digraph a {\n", found, is_unicost);
			State* 		st = new State(CopyVectorToPtr(fileField));
			State::SetPuzzleLen(size);
			st->SetGCost(0);
			st->SetFCost(st->calcFCost(solv->calcHeuristic(st->GetField()), 0, is_unicost));
			st->SetParent(nullptr);
			return (std::make_pair(solv, st));
		}
		else
			throw std::string("Passed data is not sufficient or exceeds");
	}
}
