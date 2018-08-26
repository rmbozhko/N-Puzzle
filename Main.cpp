#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include <climits>

namespace NPuzzle {

	class State
	{
	public:
		State() {}
		State(size_t* field) : field_(field), f_cost_(0) {};
		State(size_t* field, size_t f_cost) : field_(field), f_cost_(f_cost) {};
		~State() {};
		std::vector<State>	GetChilds(const size_t puzzle_len) const;
		const size_t*		GetField() const { return (field_); }
		const size_t		TileAt(const size_t pos) const { return (field_[pos]); }
		bool				isInArray(std::vector<State>& v) const;
		size_t				GetFCost() const { return (f_cost_); }
		void				SetFCost(size_t f_cost) { f_cost_ = f_cost; }
		size_t				findBetterFCost(std::vector<State>& v) const;
		// void 				SetParent(State& parent) { parent_ = parent; }
		// State 				GetParent() const { return (parent_); }

	private:
		size_t*		field_;
		size_t		f_cost_;
		size_t		g_cost_;
	};

	class Solver
	{
	public:
		// final_state_(BuildFinalState(puzzle_len))
		Solver(size_t puzzle_len=0, size_t* final_state=nullptr, std::string viz_str="")
					: puzzle_len_(puzzle_len), final_state_(final_state), viz_str_(viz_str)
					{};
		~Solver() {};
		bool		SolveWithA(State& start);
		bool		SolveWithIDA(State& start);
		State 		StatesDeepening(State& temp, const size_t threshold);
		bool		CheckIfSolvable(const NPuzzle::State& st) const;
		float		ManhattanDist(const int* final_state, const int* curr_state);
		float		EuclideanDist(const int* final_state, const int* curr_state);
		float		MisplacedTiles(const int* final_state, const int* curr_state);
		
		// const size_t*		GetStartState() { return (start_state_.GetField()); }
		const size_t*		GetFinalState() { return (final_state_); }
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
		// State					start_state_;
	};

	static	std::string 					VisitStates(const State& st);
	static 	std::pair<Solver, State> 		ReadData(const char* filename);
	static 	int*							BuildFinalState(const size_t puzzle_len);
}
using namespace NPuzzle;

size_t		State::findBetterFCost(std::vector<State>& v) const
{
	size_t		res;

	res = UINT_MAX;
	for (std::vector<State>::iterator i = v.begin(); i != v.end(); ++i)
		if (this->GetField() == (*i).GetField())
			res = (res > (*i).GetFCost()) ? (*i).GetFCost() : res; 
	return (res);
}

int			find_in_arr(const size_t* arr, const size_t size, const size_t elem)
{
	for (int i = 0; i < size; ++i)
		if (arr[i] == elem)
			return (i);
	return (-1);
}

size_t*		prepareField(const size_t* field, const size_t x, const size_t y, size_t pos, size_t puzzle_len)
{
	std::vector<size_t> 	new_field(field, field + puzzle_len);

	if (pos == 0 && y > 0)
		std::swap(new_field[y * puzzle_len + x], new_field[(y - 1) * puzzle_len + x]);
	else if (pos == 1 && y < puzzle_len - 1)
		std::swap(new_field[y * puzzle_len + x], new_field[(y - 1) * puzzle_len + x]);
	else if (pos == 2 && x > 0)
		std::swap(new_field[y * puzzle_len + x], new_field[y * puzzle_len + (x + 1)]);
	else if (pos == 3 && x < puzzle_len - 1)
		std::swap(new_field[y * puzzle_len + x], new_field[y * puzzle_len + (x + 1)]);
	return (&new_field[0]);
}

std::vector<State> 	State::GetChilds(const size_t puzzle_len) const
{
	std::vector<State> 		v;
	size_t					pos = static_cast<size_t>(find_in_arr(this->GetField(), puzzle_len, 0));
	size_t					x = pos % puzzle_len;
	size_t					y = pos / puzzle_len;

	for (size_t i = 0; i < 4; ++i)
	{
		size_t* updated_field = prepareField(this->GetField(), x, y, i, puzzle_len);
		if (updated_field != nullptr)
			v.push_back(State(updated_field));
	}
	return (v);
}

bool				State::isInArray(std::vector<State>& v) const
{
	for (std::vector<State>::iterator i = v.begin(); i != v.end(); ++i)
		if (this->GetField() == (*i).GetField())
			return (true);
	return (false);
}

bool		Solver::SolveWithA(State& start)
{
	std::vector<State> 	opened;
	std::vector<State> 	closed;

	opened.push_back(start);
	while (!opened.empty())
	{
		std::sort(opened.begin(), opened.end(), [](State a, State b) { return (a.GetFCost() < b.GetFCost()); });
		State		temp = opened.front();
		opened.pop_back();		
		closed.push_back(temp);

		if (temp.GetField() == Solver::GetFinalState()) // implement == overloading, define final_state
			return (true);

		std::vector<State> 	childs = temp.GetChilds(this->GetPuzzleSize());
		for (State& child : childs)
		{
			// if (child.isInArray(closed))
				// continue ;
			// if in closed and needed to be renewed we need to extract node from closed_list
			if (!child.isInArray(opened) && !child.isInArray(closed))
				opened.push_back(child);	
			else if (child.isInArray(opened) || child.isInArray(closed))
			{
				size_t		f_cost;
				f_cost = child.findBetterFCost(opened);
				child.SetFCost(child.GetFCost() > f_cost ? f_cost : child.GetFCost());
				f_cost = child.findBetterFCost(closed);
				child.SetFCost(child.GetFCost() > f_cost ? f_cost : child.GetFCost());
				opened.push_back(child);
			}
		} 
	}
	return (false);
}

// State 		Solver::StatesDeepening(State& temp, const size_t threshold)
// {
// 	std::deque<State>		children;
// 	State 					probState(nullptr, UINT_MAX);

// 	if (threshold > temp.f_cost)
// 		return temp;
// 	children = temp.GetChilds();
// 	for (State& child : children)
// 	{
// 		StatesDeepening(child, threshold);
// 		if (child.f_cost < probState.f_cost)
// 			probState = child;
// 	}
// 	return (probState);
// }

// State 		Solver::SolveWithIDA(State& start)
// {
// 	State 		temp;
// 	size_t		threshold;

// 	threshold = start.f_cost;
// 	while (1)
// 	{
// 		if (temp.curr_state == Solver::final_state_)
// 			return (temp);
// 		temp = Solver::StatesDeepening(start, threshold);
// 		threshold = temp.f_cost;
// 	}
// }

std::string& ltrim(std::string& str, const std::string& chars = "\t\n\v\f\r ")
{
    str.erase(0, str.find_first_not_of(chars));
    return (str);
}

// void		ft_handle_square(std::vector<std::vector<size_t>>& arr, size_t row, size_t col, std::vector<size_t>::const_iterator& iter)
// {
// 	arr[row][col] = (*iter)++;
// 	arr[row][col + 1] = (*iter)++;
// 	arr[row + 1][col + 1] = (*iter)++;
// 	arr[row + 1][col] = *iter;
// }

// void		Solver::GenerateFinalState(std::vector<std::vector<size_t>>& arr,
// 										size_t row, size_t col,	const size_t lvl,
// 										std::vector<size_t>::const_iterator& iter)
// {
// 	if (lvl - (row * 2) == 1)
// 		arr[row][col] = *iter;
// 	else if (lvl - (row * 2) == 2)
// 		ft_handle_square(arr, row, col, iter);
// 	else
// 	{
// 		for (size_t i = row, j = col; j < lvl - col; ++j)
// 			arr[i][j] = (*iter)++;
// 		for (size_t i = row + 1, j = lvl - col - 1; i < lvl - row; ++i)
// 			arr[i][j] = (*iter)++;
// 		for (size_t i = lvl - row - 1, j = lvl - col - 2; j > col; --j)
// 			arr[i][j] = (*iter)++;
// 		for (size_t i = lvl - row - 1, j = col; i > row; --i)
// 			arr[i][j] = (*iter)++;
// 		Solver::GenerateFinalState(arr, row + 1, col + 1, lvl, iter);
// 	}
// } 

// template<class ForwardIt>
// void 	generate(ForwardIt first, ForwardIt last)
// {
// 	size_t		i;

// 	i = 0;
//     while (first != last) {
//         *first++ = i++;
//     }
// }

// std::vector<size_t> 	ConvertToVector(std::vector<std::vector<size_t>>& f_state)
// {
// 	std::vector<size_t> 		v;

// 	for (size_t i = 0; i < f_state.size(); i++)
// 		for (size_t j = 0; j < f_state.size(); j++)
// 			v.push_back(f_state[i][j]);
// 	return (v);
// }

// std::vector<size_t> 	GenerateFinalState()
// {
// 	std::vector<size_t> 					seq(this.puzzle_len);
// 	std::vector<std::vector<size_t>>		final_state;

//     std::generate(seq.begin(), seq.end());
//     std::vector<size_t>::const_iterator i = seq.begin();
//     GenerateFinalState(final_state, 0, 0, this.puzzle_len, i);
//     ConvertToVector(final_state);
//     return (final_state);
// }

std::string& rtrim(std::string& s, const std::string& delimiters = " \f\n\r\t\v" )
{
   s.erase( s.find_last_not_of( delimiters ) + 1 );
   return (s);
}

std::pair<Solver, State> 		NPuzzle::ReadData(const char* filename)
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
		else
		{
			line = std::string("Error: Invalid syntax on line #") + std::to_string(i);
			throw line;
		}
		i++;
	}
	if (fileField.size() && (fileField.size() == (size * size)) && (std::find(fileField.begin(), fileField.end(), 0) != fileField.end()))
		return std::make_pair<Solver, State>(Solver(size, static_cast<size_t*>(&fileField[0]), ""), State(static_cast<size_t*>(&fileField[0])));
	else
		throw std::string("Passed data is not sufficient or exceeds");
}

// size_t		ft_calc_horiz_conflicts()
// {
// 	size_t		conflicts;

// 	conflicts = 0;
// 	for (int i = 0; i < this.puzzle_len; ++i)
// 	{
// 		size_t		temp;

// 		temp = 0;
// 		for (int j = 0; j < this.puzzle_len; ++j)
// 		{
// 			const size_t		position = i * this.puzzle_len + j;
// 			const size_t		num = this.curr_state[position];

// 			if (num != 0 && static_cast<size_t>(position / this.puzzle_len) == i)
// 			{
// 				if (temp < num)
// 					temp = num;
// 				else
// 					conflicts += 2;
// 			}
// 		}
// 	}
// 	return (conflicts);
// }

// size_t		ft_calc_vertic_conflicts()
// {
// 	size_t		conflicts;

// 	conflicts = 0;
// 	for (int i = 0; i < this.puzzle_len; ++i)
// 	{
// 		size_t		temp;

// 		temp = 0;
// 		for (int j = 0; j < this.puzzle_len; ++j)
// 		{
// 			const size_t		position = j * this.puzzle_len + i;
// 			const size_t		num = this.curr_state[position];

// 			if (num != 0 && static_cast<size_t>(position / this.puzzle_len) == j)
// 			{
// 				if (temp < num)
// 					temp = num;
// 				else
// 					conflicts += 2;
// 			}
// 		}
// 	}
// 	return (conflicts);
// }

// inline size_t		ft_add_lin_conf()
// {
// 	return (ft_calc_horiz_conflicts() + ft_calc_vertic_conflicts());
// }

// float		Solver::ManhattanDist(const int* final_state, const int* curr_state)
// {
// 	float		dist;

// 	dist = 0;
// 	for (int i = 0; i < (this.puzzle_len * this.puzzle_len); ++i)
// 	{
// 		if (curr_state[i] != 0)
// 		{
// 			std::pair<size_t, size_t> goal;	
// 			goal = ft_find_elem(final_state, curr_state[i]);
// 			dist += std::abs((i % puzzle_len) - goal.x) + std::abs((i / puzzle_len) - goal.y);
// 		}
// 	}
// 	dist += (LinearConflict) ? ft_add_lin_conf() : 0;
// 	return (dist);
// }

// float		Solver::MisplacedTiles(const int* final_state, const int* curr_state)
// {
// 	float		num;

// 	num = 0;
// 	for (int i = 0; i < (puzzle_len * puzzle_len); ++i)
// 	{
// 		if (curr_state[i] != 0)
// 		{
// 			std::pair<size_t, size_t> goal;	
// 			goal = ft_find_elem(final_state, curr_state[i]);
// 			if (!std::abs((i % puzzle_len) - goal.x) && !std::abs((i / puzzle_len) - goal.y))
// 				num += 1.0;
// 		}
// 	}
// 	return (num);
// }

// float		Solver::EuclideanDist(const int* final_state, const int* curr_state)
// {
// 	float		dist;

// 	dist = 0;
// 	for (int i = 0; i < (this.puzzle_len * this.puzzle_len); ++i)
// 	{
// 		if (curr_state[i] != 0)
// 		{
// 			std::pair<size_t, size_t> 	goal;
// 			float						result;

// 			goal = ft_find_elem(final_state, curr_state[i]);
// 			result = std::pow(std::abs((i % this.puzzle_len) - goal.x), 2.0) + std::pow(std::abs((i / this.puzzle_len) - goal.y), 2.0);
// 			if (result != 0.0)
// 				dist += std::sqrt(result);
// 		}
// 	}
// 	return (dist);
// }

// void		Solver::SetFCost(const NPuzzle::State& parent)
// {
// 	this->f_cost = parent.g_cost + 1 + Solver::CalcHCost();
// 	this->f_cost += (Solver::GreedyMode) ? 0 : (parent.f_cost - parent.g_cost);
// }

bool		Solver::CheckIfSolvable(const NPuzzle::State& st) const
{
	size_t		inv_count = 0;

    for (size_t i = 0; i < (this->GetPuzzleSize() * this->GetPuzzleSize()) - 1; i++)
        for (size_t j = i + 1; j < (this->GetPuzzleSize() * this->GetPuzzleSize()); j++)
        	inv_count += (st.TileAt(j) && st.TileAt(i) &&  st.TileAt(i) > st.TileAt(j)) ? 1 : 0;
    std::cout << "inv_count:" << inv_count << std::endl;
    if ((inv_count % 2) != 0)
    	throw std::string("Passed puzzle set is unsolvable");
    return (true);
}

int 		main(int argc, char const *argv[])
{

	if (argc == 2)
	{
		try
		{
			std::pair<Solver, State>parsed = NPuzzle::ReadData(argv[1]);
			Solver solv = parsed.first;
			State st = parsed.second;
			if (solv.CheckIfSolvable(st))
			{
				std::cout << "Solvable" << std::endl;

				if (solv.GetPuzzleSize() == 3) // what to do if len == 2?
					std::cout << solv.SolveWithA(st) << std::endl;
				// else
					// solv.SolveWithIDA(st);
			// 	solv.viz_str = NPuzzle::VisitStates(st); // combine states into xdotted string
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