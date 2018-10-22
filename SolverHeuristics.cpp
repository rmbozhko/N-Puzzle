#include "NPuzzle.hpp"

float		NPuzzle::Solver::calcHeuristic(const size_t* curr_state) const
{
	float		res;
	HEURISTIC 	func_num;

	func_num = GetHeuristic();
	switch(func_num)
	{
		case MANHATTAN:
			res = NPuzzle::Solver::ManhattanDist(GetFinalState().first, curr_state, false);
			break ;
		case EUCLIDE:
			res = NPuzzle::Solver::EuclideanDist(GetFinalState().first, curr_state);
			break ;
		case MIS_TILES:
			res = NPuzzle::Solver::MisplacedTiles(GetFinalState().first, curr_state);
			break ;
		case LIN_CONF:
			res = NPuzzle::Solver::ManhattanDist(GetFinalState().first, curr_state, true);
			break ;
		default:
			throw std::string("Wrong defined heuristic function");
	}
	return (res);
}

std::pair<size_t, size_t>		findArrElem(const size_t* final_state, const size_t size, const size_t elem)
{
	std::pair<size_t, size_t> 	goal;
	size_t 						i;

	for (i = 0; i < (size * size); ++i)
		if (final_state[i] == elem)
			break ;
	return (std::make_pair(i % size, static_cast<size_t>(i / size)));
}


size_t		ft_calc_horiz_conflicts(const size_t* curr_state, const size_t puzzle_len)
{
	size_t		conflicts;

	conflicts = 0;
	for (size_t i = 0; i < puzzle_len; ++i)
	{
		size_t		temp;

		temp = 0;
		for (size_t j = 0; j < puzzle_len; ++j)
		{
			const size_t		position = i * puzzle_len + j;
			const size_t		num = curr_state[position];

			if (num != 0 && static_cast<size_t>(position / puzzle_len) == i)
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

size_t		ft_calc_vertic_conflicts(const size_t* curr_state, const size_t puzzle_len)
{
	size_t		conflicts;

	conflicts = 0;
	for (size_t i = 0; i < puzzle_len; ++i)
	{
		size_t		temp;

		temp = 0;
		for (size_t j = 0; j < puzzle_len; ++j)
		{
			const size_t		position = j * puzzle_len + i;
			const size_t		num = curr_state[position];

			if (num != 0 && static_cast<size_t>(position / puzzle_len) == j)
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

inline size_t		ft_add_lin_conf(const size_t* curr_state, const size_t puzzle_len)
{
	return (ft_calc_horiz_conflicts(curr_state, puzzle_len) + ft_calc_vertic_conflicts(curr_state, puzzle_len));
}

float		NPuzzle::Solver::ManhattanDist(const size_t* final_state, const size_t* curr_state, bool LinearConflict = false) const
{
	float		dist;

	dist = 0;
	for (size_t i = 0; i < (GetPuzzleSize() * GetPuzzleSize()); ++i)
	{
		if (curr_state[i] != 0)
		{
			std::pair<size_t, size_t> goal;	
			goal = findArrElem(final_state, GetPuzzleSize(), curr_state[i]);
			dist += std::abs(static_cast<int>((i % GetPuzzleSize()) - goal.first))
						+ std::abs(static_cast<int>((i / GetPuzzleSize()) - goal.second));
		}
	}
	if (LinearConflict)
		dist += static_cast<int>(ft_add_lin_conf(curr_state, GetPuzzleSize()));
	return (dist);
}

float		NPuzzle::Solver::MisplacedTiles(const size_t* final_state, const size_t* curr_state) const
{
	float		num;

	num = 0;
	for (size_t i = 0; i < (GetPuzzleSize() * GetPuzzleSize()); ++i)
	{
		if (curr_state[i] != 0)
		{
			std::pair<size_t, size_t> goal;	
			goal = findArrElem(final_state, GetPuzzleSize(), curr_state[i]);
			if (!std::abs(static_cast<int>((i % GetPuzzleSize()) - goal.first))
					&& !std::abs(static_cast<int>(i / GetPuzzleSize())) - goal.second)
				num += 1.0;
		}
	}
	return (num);
}

float		NPuzzle::Solver::EuclideanDist(const size_t* final_state, const size_t* curr_state) const
{
	float		dist;

	dist = 0;
	for (size_t i = 0; i < (GetPuzzleSize() * GetPuzzleSize()); ++i)
	{
		if (curr_state[i] != 0)
		{
			std::pair<size_t, size_t> 	goal;
			float						result;

			goal = findArrElem(final_state, GetPuzzleSize(), curr_state[i]);
			result = std::pow(std::abs(static_cast<int>((i % GetPuzzleSize()) - goal.first)), 2.0)
						+ std::pow(std::abs(static_cast<int>((i / GetPuzzleSize()) - goal.second)), 2.0);
			if (result != 0.0)
				dist += std::sqrt(result);
		}
	}
	return (dist);
}