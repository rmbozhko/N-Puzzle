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
		std::deque<State>	GetChilds() const;

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
			// if (child.isInArray(closed))
				// continue ;
			// if in closed and needed to be renewed we need to extract node from closed_list
			if ((child.isInArray(opened) || child.isInArray(closed) && child.f_cost > ft_get_f_cost())
				|| (!child.isInArray(opened) && !child.isInArray(closed)))
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

State 		Solver::StatesDeepening(State& temp, const size_t threshold)
{
	std::deque<State>		children;
	State 					probState(nullptr, MAX_UINT);

	if (threshold > temp.f_cost)
		return temp;
	children = temp.GetChilds();
	for (State& child : children)
	{
		StatesDeepening(child, threshold);
		if (child.f_cost < probState.f_cost)
			probState = child;
	}
	return (probState);
}

State 		Solver::SolveWithIDA(State& start)
{
	State 		temp;
	size_t		threshold;

	threshold = start.f_cost;
	while (1)
	{
		if (temp.curr_state == Solver::final_state)
			return (temp);
		temp = Solver::StatesDeepening(start, threshold);
		threshold = temp.f_cost;
	}
}

std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return str;
}

void		ft_handle_square(std::vector<std::vector<size_t>>& arr, size_t row, size_t col, std::vector<size_t>::const_iterator& iter)
{
	arr[row][col] = (*iter)++;
	arr[row][col + 1] = (*iter)++;
	arr[row + 1][col + 1] = (*iter)++;
	arr[row + 1][col] = *iter;
}

void		Solver::GenerateFinalState(std::vector<std::vector<size_t>>& arr,
										size_t row, size_t col,	const size_t lvl,
										std::vector<size_t>::const_iterator& iter)
{
	if (lvl - (row * 2) == 1)
		arr[row][col] = *iter;
	else if (lvl - (row * 2) == 2)
		ft_handle_square(arr, row, col, iter);
	else
	{
		for (size_t i = row, j = col; j < lvl - col; ++j)
			arr[i][j] = (*iter)++;
		for (size_t i = row + 1, j = lvl - col - 1; i < lvl - row; ++i)
			arr[i][j] = (*iter)++;
		for (size_t i = lvl - row - 1, j = lvl - col - 2; j > col; --j)
			arr[i][j] = (*iter)++;
		for (size_t i = lvl - row - 1, j = col; i > row; --i)
			arr[i][j] = (*iter)++;
		Solver::GenerateFinalState(arr, row + 1, col + 1, lvl, iter);
	}
} 

template<class ForwardIt>
void 	generate(ForwardIt first, ForwardIt last)
{
	size_t		i;

	i = 0;
    while (first != last) {
        *first++ = i++;
    }
}

std::vector<size_t> 	ConvertToVector(std::vector<std::vector<size_t>>& f_state)
{
	std::vector<size_t> 		v;

	for (size_t i = 0; i < f_state.size(); i++)
		for (size_t j = 0; j < f_state.size(); j++)
			v.push_back(f_state[i][j]);
	return (v);
}

std::vector<size_t> 	GenerateFinalState()
{
	std::vector<size_t> 					seq(this.puzzle_len);
	std::vector<std::vector<size_t>>		final_state;

    std::generate(seq.begin(), seq.end());
    std::vector<size_t>::const_iterator i = seq.begin();
    GenerateFinalState(final_state, 0, 0, this.puzzle_len, i);
    ConvertToVector(final_state);
    return (final_state);
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

size_t		ft_calc_horiz_conflicts()
{
	size_t		conflicts;

	conflicts = 0;
	for (int i = 0; i < this.puzzle_len; ++i)
	{
		size_t		temp;

		temp = 0;
		for (int j = 0; j < this.puzzle_len; ++j)
		{
			const size_t		position = i * this.puzzle_len + j;
			const size_t		num = this.curr_state[position];

			if (num != 0 && static_cast<size_t>(position / this.puzzle_len) == i)
			{
				if (temp < num)
					temp = num;
				else
					conflicts += 2;
			}
		}
	}
	return (conflicts);
}

size_t		ft_calc_vertic_conflicts()
{
	size_t		conflicts;

	conflicts = 0;
	for (int i = 0; i < this.puzzle_len; ++i)
	{
		size_t		temp;

		temp = 0;
		for (int j = 0; j < this.puzzle_len; ++j)
		{
			const size_t		position = j * this.puzzle_len + i;
			const size_t		num = this.curr_state[position];

			if (num != 0 && static_cast<size_t>(position / this.puzzle_len) == j)
			{
				if (temp < num)
					temp = num;
				else
					conflicts += 2;
			}
		}
	}
	return (conflicts);
}

inline size_t		ft_add_lin_conf()
{
	return (ft_calc_horiz_conflicts() + ft_calc_vertic_conflicts());
}

float		Solver::ManhattanDist(const int* final_state, const int* curr_state)
{
	float		dist;

	dist = 0;
	for (int i = 0; i < (puzzle_len * puzzle_len); ++i)
	{
		if (curr_state[i] != 0)
		{
			std::pair<size_t, size_t> goal;	
			goal = ft_find_elem(final_state, curr_state[i]);
			dist += std::abs((i % puzzle_len) - goal.x) + std::abs((i / puzzle_len) - goal.y);
		}
	}
	dist += (LinearConflict) ? ft_add_lin_conf() : 0;
	return (dist);
}

float		Solver::MisplacedTiles(const int* final_state, const int* curr_state)
{
	float		num;

	num = 0;
	for (int i = 0; i < (puzzle_len * puzzle_len); ++i)
	{
		if (curr_state[i] != 0)
		{
			std::pair<size_t, size_t> goal;	
			goal = ft_find_elem(final_state, curr_state[i]);
			if (!std::abs((i % puzzle_len) - goal.x) && !std::abs((i / puzzle_len) - goal.y))
				num += 1.0;
		}
	}
	return (num);
}

float		Solver::EuclideanDist(const int* final_state, const int* curr_state)
{
	float		dist;

	dist = 0;
	for (int i = 0; i < (this.puzzle_len * this.puzzle_len); ++i)
	{
		if (curr_state[i] != 0)
		{
			std::pair<size_t, size_t> 	goal;
			float						result;

			goal = ft_find_elem(final_state, curr_state[i]);
			result = std::pow(std::abs((i % this.puzzle_len) - goal.x), 2.0) + std::pow(std::abs((i / this.puzzle_len) - goal.y), 2.0);
			if (result != 0.0)
				dist += std::sqrt(result);
		}
	}
	return (dist);
}



void		Solver::SetFCost(const NPuzzle::State& parent)
{
	this->f_cost = parent.g_cost + 1 + Solver::CalcHCost();
	this->f_cost += (Solver::GreedyMode) ? 0 : (parent.f_cost - parent.g_cost);
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