#include "NPuzzle.hpp"

NPuzzle::State*		NPuzzle::Solver::SolveWithA(State* start, std::vector<State*>& opened, std::vector<State*>& closed)
{
	// std::vector<State*> 	opened;
	// std::vector<State*> 	closed;

	opened.push_back(start);
	while (!opened.empty())
	{
		std::sort(opened.begin(), opened.end(), [](State* a, State* b) { return (a->GetFCost() < b->GetFCost()); });
		State*		temp = opened.front();

		if (*temp == Solver::GetFinalState())
		{
			// FreeStorage(opened);
			// FreeStorage(closed);
			return (temp);
		}
		opened.erase(opened.begin());
		closed.push_back(temp);

		std::vector<State*> 	children = temp->GetChildren(GetPuzzleSize(), *this);
		for (size_t i = 0; i < children.size(); ++i)
		{
			// if in closed and needed to be renewed we need to extract node from closed_list
			if (!children[i]->isInArray(opened) && !children[i]->isInArray(closed))
			{
				IncreaseTotalNumOfStates();
				SetMaxNumOfStates(opened.size());
				opened.push_back(children[i]);	
			}
			else if (children[i]->isInArray(opened))
			{
				int		pos;
				
				// do we renewed states already moved to closed list?
				// pos = children[i]->GetDuplicate(closed);
				// if (pos != -1)
				// {
				// 	if (children[i]->GetFCost() > closed[pos]->GetFCost())
				// 	{
				// 		*(children[i]) = closed[pos]; // overload '=' and delete all allocated memory
				// 		opened.push_back(children[i]);
				// 		delete closed[pos];
				// 		closed.erase(closed.begin() + pos);
				// 	}
				// }
				
				pos = children[i]->GetDuplicate(opened);
				if (pos != -1)
				{
					if (children[i]->GetFCost() > opened[pos]->GetFCost())
					{
						*(children[i]) = opened[pos];
						opened.push_back(children[i]);
						delete opened[pos];
						opened.erase(opened.begin() + pos);
					}
				}
			}
		}
	}
	return (nullptr);
}

NPuzzle::State* 		NPuzzle::Solver::StatesDeepening(NPuzzle::State* temp, const size_t threshold) const
{
	std::vector<NPuzzle::State*>		children;
	float								tempFCost = std::numeric_limits<float>::max();
	size_t								pos = 0;

	if (threshold < temp->GetFCost() || *(temp) == Solver::GetFinalState())
	{
		// if (*(temp) == Solver::GetFinalState())
			// std::cout << "@@@@@@@@@@@@Found\n" << temp << std::endl;	
		return temp;
	}
	// std::cout << "?????????? Next node\n" << temp << std::endl;
	children = temp->GetChildren(GetPuzzleSize(), *this);
	for (size_t i = 0; i < children.size(); ++i)
	{
		// std::cout << "---------------" << std::endl;
		// std::cout << children[i] << std::endl;
		// std::cout << "###############" << std::endl;
		State*		st = StatesDeepening(children[i], threshold);
		if (*(st) == Solver::GetFinalState())
		{
			// std::cout << "$$$$$$$$$$$$$$$$$$$$$Found\n" << st << std::endl;
			return (st);//(children[i]);
		}
		if (children[i]->GetFCost() < tempFCost)
		{
			// std::cout << "Cost is less for child #" << i << std::endl << children[i];
			tempFCost = children[i]->GetFCost();
			pos = i;
		}
	}
	// std::cout << "Choosed child #" << pos << std::endl << children[pos];
	return (children[pos]);
}

NPuzzle::State* 		NPuzzle::Solver::SolveWithIDA(NPuzzle::State* start)
{
	NPuzzle::State* 		temp;
	float					threshold;

	threshold = start->GetFCost();
	SetUnicostMode(false);
	if (*(start) == Solver::GetFinalState())
			return (start);
	while (1)
	{
		// std::cout << threshold << std::endl;
		// std::cout << "Start:\n" << start << std::endl;
		temp = NPuzzle::Solver::StatesDeepening(start, threshold);
		if (*(temp) == Solver::GetFinalState())
			return (temp);
		// std::cout << "Moving along" << std::endl;
		threshold = temp->GetFCost();
		// if (threshold )
			// exit(0);
	}
}