#include "NPuzzle.hpp"

using namespace NPuzzle;

size_t*		CopyPtrToPtr(const size_t* field)
{
	size_t		length = NPuzzle::State::GetPuzzleLen() * NPuzzle::State::GetPuzzleLen();
	size_t*		new_field = new size_t[length];
	
	if (!new_field)
		throw new std::string("Couldn't create copy of field");
	for (size_t i = 0; i < length; ++i)
		new_field[i] = field[i];
	return (new_field);
}

void				printUsage()
{
	std::cout << "Usage: ./n_puzzle [-dot, -s, -i, -h] -f puzzle_file.txt" << std::endl;
	std::cout << "\t-dot - generating dot file with steps passed to solution state" << std::endl;
	std::cout << "\t-s - (min: 2, max: 100, default: 3): size for a random generated puzzle" << std::endl;
	std::cout << "\t-i - (default: 1000): number of iterations for a random generated puzzle" << std::endl;
	std::cout << "\t-f - specify file, which would serve as a source of puzzle for a program" << std::endl;
}

void		FreeStorage(std::vector<NPuzzle::State*>& v)
{
	for (size_t i = 0; i < v.size(); ++i)
		delete v[i];
}

void		FreeStorage(NPuzzle::State* v)
{
	if (v->GetParent() == nullptr)
		return ;
	delete v;	
}

NPuzzle::State*				copyState(NPuzzle::State* st)
{
	if (!(st))
		throw new std::string("Empty state encountered");
	NPuzzle::State*		temp = new NPuzzle::State(CopyPtrToPtr(st->GetField()), st->GetFCost(),
														st->GetGCost(), st->GetParent());
	return (temp);
}

static void		MainController(std::pair<Solver*, State*> parsed, bool isDot=false)
{
	Solver* 	solv = parsed.first;
	State* 		st = parsed.second;

	if (isDot)
		solv->SetVisStr("digraph a {\n");
		
	if (solv->IsSolvable(st))
	{
		std::vector<State*> 	closed;
		std::vector<State*> 	opened;
		State* 					fin_state;
		
		opened.push_back(st);
		fin_state = solv->SolveWithA(opened, closed);
		if (fin_state)
			solv->VisitStates(fin_state, isDot);
		else
			std::cout << "Have not found an answer" << std::endl;
		FreeStorage(opened);
		FreeStorage(closed);
	}
	else
	{
		std::cerr << "Puzzle is unsolvable" << std::endl;
		delete st;
	}

	delete solv;
}

std::pair<Solver*, State*>		GenerateData(char const* puzzle_size="3", char const* iterations="1000")
{
	if (!puzzle_size && !(std::atoi(puzzle_size) > 1 && std::atoi(puzzle_size) < 101))
		throw std::string("Invalid size parameter for puzzle generation");
	if (!iterations && std::atoi(iterations) < 0)
		throw std::string("Invalid iterations number parameter for puzzle generation");
	
	size_t 	const 	size = static_cast<size_t>(std::atoi(puzzle_size));
	int 			iterNum = std::atoi(iterations);
	Solver* const	solv = new Solver(size, NPuzzle::Solver::GenerateFinalState(size), "", MANHATTAN, true);
	
	size_t* const	new_field = new size_t[size * size];
	for (size_t i = 0; i < size * size; ++i)
		new_field[i] = solv->GetFinalState().first[i];

	auto findTile = [&](const size_t* const field, const size_t len, const int num)
	{
		int  length = static_cast<int>(len * len);
		if (num < (length - 1))
		{
			for (int i = 0; i < length; ++i)
			{
				if (field[i] == static_cast<size_t>(num))
					return (i);
			}
		}
		return (-1);
	};

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

void		HandleCmdArgs(const int ac, char const* argv[])
{
	int 	puzzle_size;
	int 	iterations;
	bool	isDot;
	int		puzzleFile;

	puzzle_size = 0;
	iterations = 0;
	isDot = false;
	puzzleFile = 0;

	if (ac > 1)
	{
		for (int i = 1; i < ac; ++i)
		{
			if (!std::strcmp(argv[i], "-h"))
				printUsage();
			else if (!std::strcmp(argv[i], "-s"))
			{
				if (i + 1 < ac)
					puzzle_size = ++i;
				else
					throw std::string("Size for puzzle generation isn't specified");
			}
			else if (!std::strcmp(argv[i], "-i"))
			{
				if (i + 1 < ac)
					iterations = ++i;
				else
					throw std::string("Iterations number for puzzle generation isn't specified");
			}
			else if (!std::strcmp(argv[i], "-dot"))
				isDot = true;
			else if (!std::strcmp(argv[i], "-f"))
			{
				if (i + 1 < ac)
					puzzleFile = ++i;
				else
					throw std::string("File with puzzle isn't specified");
			}
			else
			{
				printUsage();
				throw std::string("Unkown flag is specified");
			}
		}

		if (puzzleFile)
			MainController(ReadData(argv[puzzleFile]), isDot);
		else
		{
			if (puzzle_size && iterations)
				MainController(GenerateData(argv[puzzle_size], argv[iterations]), isDot);
			else if (puzzle_size && !iterations)
				MainController(GenerateData(argv[puzzle_size]), isDot);
			else
				MainController(GenerateData(), isDot); 
		}
	}
	else
		MainController(GenerateData());
}

int 		main(int argc, char const *argv[])
{
	try
	{
		HandleCmdArgs(argc, argv);
	}	
	catch (std::string msg)
	{
		std::cerr << msg << std::endl;
	}
	return (0);
}
