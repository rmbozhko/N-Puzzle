#include "NPuzzle.hpp"

using namespace NPuzzle;

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