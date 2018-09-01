#include "NPuzzle.hpp"

using namespace NPuzzle;

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