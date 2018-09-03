#include "NPuzzle.hpp"

using namespace NPuzzle;

int 		main(int argc, char const *argv[])
{
	if (argc == 2)
	{
		try
		{
			std::pair<Solver*, State*>parsed = ReadData(argv[1]);
			Solver* solv = parsed.first;
			State* st = parsed.second;

			std::string			filename = "rules/sequence.dot";
			std::ofstream		file(filename);

			if (!file.is_open())
				throw std::string("Couldn't open file for visualization");

			// if (solv->CheckIfSolvable(st))
			// {
				State* fin_state = (solv->GetPuzzleSize() == 3) ? solv->SolveWithA(st) : solv->SolveWithIDA(st);  
				if (fin_state)
					std::cout << "Solved" << std::endl;
				else
					std::cout << "Have not found an answer" << std::endl;
				std::cout << fin_state << std::endl;
				solv->SetVisStr(VisitStates(fin_state));
			// }
			solv->SetVisStr(std::string("}\n"));
			file << solv->GetVisStr();
			file.close();
			
			delete st;
			delete solv;
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