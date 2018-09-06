#include "NPuzzle.hpp"

using namespace NPuzzle;

void				printUsage()
{
	std::cerr << "Usage: ./n_puzzle [-dot, -h] puzzle_file.txt" << std::endl;
	std::cerr << "\t-dot - generating dot file with steps passed to solution state" << std::endl;
}

void				FreeStorage(std::vector<NPuzzle::State*>& v)
{
	for (int i = 0; i < v.size(); ++i)
		delete v[i];
}

void		MainController(const char* filename, bool isDot=false)
{
	std::pair<Solver*, State*>parsed = ReadData(filename);
	Solver* solv = parsed.first;
	State* st = parsed.second;

	if (isDot)
		solv->SetVisStr("digraph a {\n");

	if (solv->IsSolvable(st))
	{
		std::vector<State*> 	closed;
		std::vector<State*> 	opened;
		State* fin_state = (solv->GetPuzzleSize() == 3) ? solv->SolveWithA(st, opened, closed)
														: solv->SolveWithIDA(st);
		if (fin_state)
			solv->VisitStates(fin_state, isDot);
		else
			std::cout << "Have not found an answer" << std::endl;
		FreeStorage(opened);
		FreeStorage(closed);
	}
	else
		std::cerr << "Puzzle is unsolvable" << std::endl;
	
	// should be deleted, cause IDA doesn't need opened || closed
	// so st never gets into closed, so won't be freed above
	// delete st; 
	delete solv;
}

int 		main(int argc, char const *argv[])
{
	try
	{
		switch (argc)
		{
			case 1:
				std::cout << "Generating puzzle" << std::endl;
				break ;
			case 2:
				if (!std::strcmp(argv[1], "-h"))
					printUsage();
				else
					MainController(argv[1]);
				break ;
			case 3:
				if (!std::strcmp(argv[1], "-dot"))
					MainController(argv[2], true);
				else if (!std::strcmp(argv[1], "-h"))
					printUsage();
				else
					throw std::string("Invallid command line parameter");
				break ;
			default:
				printUsage();
		}
	}
	catch (std::string msg)
	{
		std::cerr << msg << std::endl;
	}
	return (0);
}
