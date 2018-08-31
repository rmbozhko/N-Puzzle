#include "NPuzzle.hpp"

using namespace NPuzzle;

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
// 	this->f_cost_ = parent.g_cost + 1 + Solver::CalcHCost();
// 	this->f_cost_ += (Solver::GreedyMode) ? 0 : (parent.f_cost - parent.g_cost);
// }

int 		main(int argc, char const *argv[])
{

	if (argc == 2)
	{
		try
		{
			std::pair<Solver, State>parsed = ReadData(argv[1]);
			Solver solv = parsed.first;
			State st = parsed.second;

			// if (solv.CheckIfSolvable(st))
			// {
				if (solv.GetPuzzleSize() == 3) // what to do if len == 2?
					std::cout << solv.SolveWithA(st) << std::endl;
				// else
					// solv.SolveWithIDA(st);
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