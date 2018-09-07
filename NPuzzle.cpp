#include "NPuzzle.hpp"

using namespace NPuzzle;

void				printUsage()
{
	std::cerr << "Usage: ./n_puzzle [-dot, -s, -i, -h] puzzle_file.txt" << std::endl;
	std::cerr << "\t-dot - generating dot file with steps passed to solution state" << std::endl;
	std::cerr << "\t-s - (min: 2, max: 100, default: 3): size for a random generated puzzle" << std::endl;
	std::cerr << "\t-i - (default: 1000): number of iterations for a random generated puzzle" << std::endl;
}

void				FreeStorage(std::vector<NPuzzle::State*>& v)
{
	for (int i = 0; i < v.size(); ++i)
		delete v[i];
}

static void		MainController(std::pair<Solver*, State*>parsed, bool isDot=false)
{
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

static int					findTile(const size_t* const field, const size_t len, const size_t num)
{
	if (num < (len * len) - 1)
	{
		for (int i = 0; i < len * len; ++i)
		{
			if (field[i] == num)
				return (i);
		}
	}
	return (-1);
}

std::pair<Solver*, State*>		GenerateData(char const* puzzle_size="3", char const* iterations="1000")
{
	if (!(std::atoi(puzzle_size) > 1 && std::atoi(puzzle_size) < 101))
		throw std::string("Invalid size parameter for puzzle generation");
	if (std::atoi(iterations) < 0)
		throw std::string("Invalid iterations number parameter for puzzle generation");
	
	size_t 	const 	size = static_cast<size_t>(std::atoi(puzzle_size));
	int 			iterNum = std::atoi(iterations);
	Solver* const	solv = new Solver(size, NPuzzle::Solver::GenerateFinalState(size), "", MANHATTAN, true);
	
	size_t* const	new_field = new size_t[size * size];
	for (size_t i = 0; i < size * size; ++i)
		new_field[i] = solv->GetFinalState().first[i];

	while (iterNum)
	{
		int const 	pos = findTile(new_field, size, 0);
		if (pos == -1)
			throw std::string("Couldn't find passed element");
		if ((prepareField(new_field, pos % size, pos / size,
													rand() % 4, size)) != nullptr)
			--iterNum;

	}

	State* 	const	st = new State(new_field);
	State::SetPuzzleLen(size);
	st->SetGCost(0);
	st->SetFCost(st->calcFCost(solv->calcHeuristic(st->GetField()), 0, true));
	st->SetParent(nullptr);
	
	return (std::make_pair(solv, st));
}

int 		main(int argc, char const *argv[])
{
	try
	{
		switch (argc)
		{
			case 1:
				MainController(GenerateData());
				break ;
			case 2:
				if (!std::strcmp(argv[1], "-h"))
					printUsage();
				else
					MainController(ReadData(argv[1]));
				break ;
			case 3:
				if (!std::strcmp(argv[1], "-dot"))
					MainController(ReadData(argv[2]), true);
				else if (!std::strcmp(argv[1], "-h"))
					printUsage();
				else if (!std::strcmp(argv[1], "-s"))
					MainController(GenerateData(argv[2]));
				else if (!std::strcmp(argv[1], "-i"))
					MainController(GenerateData("3", argv[2]));
				break ;
			case 4:
				if (!std::strcmp(argv[1], "-dot") && !std::strcmp(argv[2], "-s"))
					MainController(GenerateData(argv[3]), true);
				else if (!std::strcmp(argv[3], "-dot") && !std::strcmp(argv[1], "-s"))
					MainController(GenerateData(argv[2]), true);
				else if (!std::strcmp(argv[1], "-dot") && !std::strcmp(argv[2], "-i"))
					MainController(GenerateData("3", argv[3]), true);
				else if (!std::strcmp(argv[3], "-dot") && !std::strcmp(argv[1], "-i"))
					MainController(GenerateData("3", argv[2]), true);
				else
					printUsage();
			case 5:
				if (!std::strcmp(argv[1], "-s") && !std::strcmp(argv[3], "-i"))
					MainController(GenerateData(argv[2], argv[4]));
				else if (!std::strcmp(argv[3], "-s") && !std::strcmp(argv[1], "-i"))
					MainController(GenerateData(argv[4], argv[2]));
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
