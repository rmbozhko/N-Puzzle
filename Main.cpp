#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>

namespace NPuzzle {

	class State
	{
	public:
		State() {}
		State(size_t* field) : field_(field)  {};
		~State() {};
		std::deque<State>	GetChilds(const State& st);
		const size_t*		GetField() const { return (field_); }

	private:
		size_t*		field_;
	};

	class Solver
	{
	public:
		// final_state_(BuildFinalState(puzzle_len))
		Solver(size_t puzzle_len=0, size_t* final_state=nullptr, std::string viz_str="", State st=NULL) : puzzle_len_(puzzle_len), final_state_(final_state), viz_str_(viz_str), start_state_(st) {};
		~Solver() {};
		bool		SolveWithA(State& start);
		bool		CheckIfSolvable(const NPuzzle::State& st) const;
		
		const size_t*		GetStartState() { return (start_state_.GetField()); }
		const size_t		GetPuzzleSize() const { return (puzzle_len_); }
		const std::string&	GetVisStr() const { return (viz_str_); }

		void			SetVisStr(std::string& viz_str)
		{
			if (viz_str.size() > 0)
				viz_str_ = viz_str;
			else
				throw std::string("Wrongly formatted string for XDoT");
		}

		void				SetPuzzleSize(const size_t len)
		{
			if (len > 20)
				throw std::string("Passed puzzle size is too big to be handled");
			else
				puzzle_len_ = len;
		}
	private:
		size_t					puzzle_len_;
		size_t*					final_state_; // = {1, 2, 3, 8, 0, 4, 7, 6, 5};
		std::string 			viz_str_;
		State					start_state_;
	};

	static	std::string 		VisitStates(const State& st);
	static 	Solver		 		ReadData(const char* filename);
	static 	int*				BuildFinalState(const size_t puzzle_len);
}

// bool		Solver::SolveWithA(State& start)
// {
// 	std::deque<State> 	opened;
// 	std::deque<State> 	closed;

// 	opened.push_back(start);
// 	while (!opened.empty())
// 	{
// 		ft_sort_by_f_cost(opened);
// 		State		temp = opened.pop_front();
// 		closed.add(temp);

// 		if (temp == final_state) // implement == overloading, define final_state
// 			return (true);

// 		std::deque<State> 	childs = State::GetChilds(temp);
// 		for (State& child : childs)
// 		{
// 			if (child.isInArray(closed))
// 				continue ;
// 			if ((child.isInArray(opened) && child.f_cost > ft_get_f_cost()) || !child.isInArray(opened))
// 			{
// 				child.f_cost = ft_get_f_cost();
// 				child.parent = temp;
// 				if (!child.isInArray(opened))
// 					opened.push_back(child);
// 			}
// 		} 
// 	}
// 	return (false);
// }

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

NPuzzle::Solver 		NPuzzle::ReadData(const char* filename)
{
	std::ifstream 				infile(filename);
	std::string 				line;
	size_t						i;
	std::smatch					m;
	
	std::vector<size_t> 		fileField;
	size_t						size;

	i = 1;
	size = 0;
	while (std::getline(infile, line))
	{
		line = rtrim(ltrim(line));
		/* pass from file useful flags:
		*	1. %manhattan
		*	2. %lin_conf
		*	3. %smth_else
		*	4. %greddy
		*	4. %unicost
		*/
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
				std::cout << "Distance: " << std::distance(start, end) << std::endl;
				if (std::regex_match(m[0].str(), m, std::regex("^[0-9]+")) && std::distance(start, end) == 1)
				{
					if (size == 0)
						size = static_cast<size_t>(atoi(m[0].str().c_str()));
					else
						throw std::string("Error: Puzzle size overwriting on line #") + std::to_string(i); 
					std::cout << "PuzzleSize:" << size << std::endl;
				}
				else if (std::distance(start, end) == size)
					for (; start != std::sregex_iterator(); start++ )
						fileField.push_back(static_cast<size_t>(atoi((*start).str().c_str())));
				else
					throw std::string("Error: Puzzle data doesn't correspond to size on the line #") + std::to_string(i);
			}
		}
		else if (std::regex_match(line, m, std::regex("^[#].+$"))) // plain line with comments
			std::cout << "Line of comment: " << m[0].str() << std::endl;
		else if (std::regex_match(line, m, std::regex("^[ \t\n]*$")) || line[0] == '\n')
			std::cout << "Line with newlines chars" << std::endl;
		else
		{
			line = std::string("Error: Invalid syntax on line #") + std::to_string(i); 
			throw line;
		}
		i++;
	}
	if (fileField.size() && (fileField.size() == (size * size)))
		return (NPuzzle::Solver(size, static_cast<size_t*>(&fileField[0]), "", NPuzzle::State(static_cast<size_t*>(&fileField[0]))));
	else
		throw std::string("Passed data is not sufficient or exceeds");
}

// bool		Solver::CheckIfSolvable(const NPuzzle::State& st) const
// {
// 	size_t		inv_count = 0;

//     for (size_t i = 0; i < (this.puzzle_len * this.puzzle_len) - 1; i++)
//         for (size_t j = i + 1; j < (this.puzzle_len * this.puzzle_len); j++)
//         	inv_count += (st->field[j] && st->field[i] &&  st->field[i] > st->field[j]) ? 1 : 0;

//     return ((inv_count % 2) == 0);
// }

int 		main(int argc, char const *argv[])
{

	if (argc == 2)
	{
		try
		{
			NPuzzle::Solver solv = NPuzzle::ReadData(argv[1]);
			auto	temp = solv.GetStartState();
			for (int i = 0; i < solv.GetPuzzleSize() * solv.GetPuzzleSize(); ++i)
			{
				std::cout << temp[i] << std::endl;
			}
			// if (solv.CheckIfSolvable(solv.GetStartState()))
			// {
			// 	NPuzzle::State 		st;

			// 	if (solv.puzzle_len == 3) // what to do if len == 2
			// 		st = solv.SolveWithA(solv.start_state);
			// 	else
			// 		st = solv.SolveWithIDA(solv.start_state);
			// 	solv.viz_str = NPuzzle::VisitStates(st); // combine states into xdotted string
			// }

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