#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

namespace NPuzzle {

	class State
	{
	public:
		State();
		~State();
		std::deque<State>	GetChilds(const State& st);

	private:
		const int*		field;
	};

	class Solver
	{
	public:
		Solver();
		~Solver();
		bool		SolveWithA();
		bool		CheckIfSolvable() const;
		void 		ReadData(const char* filename);
	
	private:
		size_t					puzzle_len;
		static const int*		final_state; // = {1, 2, 3, 8, 0, 4, 7, 6, 5};
		static string 			viz_str;
	};

	static	std::string 		VisitStates(const State& st);
}

bool		Solver::SolveWithA(State& start)
{
	std::deque<State> 	opened;
	std::deque<State> 	closed;

	opened.push_back(start);
	while (!opened.empty())
	{
		ft_sort_by_f_cost(opened);
		State		temp = opened.pop_front();
		close.add(temp);

		if (temp == final_state) // implement == overloading, define final_state
			return (true);

		std::deque<State> 	childs = State::GetChilds(temp);
		for (State& child : childs)
		{
			if (child.isInArray(closed))
				continue ;
			if ((child.isInArray(opened) && child.f_cost > ft_get_f_cost()) || !child.isInArray(opened))
			{
				child.f_cost = ft_get_f_cost();
				child.parent = temp;
				if (!child.isInArray(opened))
					opened.push_back(child);
			}
		} 
	}
	return (false);
}

std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

void 		Solver::ReadData(const char* filename)
{
	std::ifstream 				infile(filename);
	std::string 				line;
	size_t						i;

	i = 1;
	while (std::getline(infile, line))
	{
		ltrim(line);
		// instead regex_match -- use regex_search
		/* pass from file useful flags:
		*	1. %manhattan
		*	2. %lin_conf
		*	3. %smth_else
		*	4. %greddy
		*	4. %unicost
		*/
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
	}
}

bool		Solver::CheckIfSolvable(const NPuzzle::State& st) const
{
	size_t		inv_count = 0;

    for (size_t i = 0; i < (this.puzzle_len * this.puzzle_len) - 1; i++)
        for (size_t j = i + 1; j < (this.puzzle_len * this.puzzle_len); j++)
        	inv_count += (st.field[j] && st.field[i] &&  st.field[i] > st.field[j]) ? 1 : 0;

    return ((inv_count % 2) == 0);
}

int 		main(int argc, char const *argv[])
{
	if (argc == 2)
	{
		NPuzzle::Solver		solv;
		try
		{
			solv.ReadData(argv[1]);
			if (solv.CheckIfSolvable(solv.start_state))
			{
				NPuzzle::State 		st;

				if (solv.puzzle_len == 3) // what to do if len == 2
					st = solv.SolveWithA(solv.start_state);
				else
					st = solv.SolveWithIDA(solv.start_state);
				solv.viz_str = NPuzzle::VisitStates(st); // combine states into xdotted string
			}

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