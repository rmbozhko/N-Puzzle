#include "NPuzzle.hpp"

NPuzzle::State*		NPuzzle::Solver::SolveWithA(State* start, std::vector<State*>& opened, std::vector<State*>& closed)
{
	// std::vector<State*> 	opened;
	// std::vector<State*> 	closed;

	// opened.push_back(start);
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
	State*								stmt;
	State*								st;

	// std::cout << "Passed temp:\n" << temp << "+++++++++++++" << std::endl;
	if (threshold < temp->GetFCost() || *(temp) == Solver::GetFinalState()) {
		return temp;
	}
	// std::cout << "Yo" << std::endl;
	children = temp->GetChildren(GetPuzzleSize(), *this);
	
	for (size_t i = 0; i < children.size(); ++i) {
		
		st = StatesDeepening(children[i], threshold);
		
		if (*(st) == Solver::GetFinalState()) {
			return (st);
		}
		
		if (st->GetFCost() < tempFCost) {
			tempFCost = st->GetFCost();
			stmt = st;
		}
	}
	for (size_t i = 0; i < children.size(); ++i) {
		if (!(children[i] == stmt))
			delete children[i];
	}
	return (stmt);
}

NPuzzle::State* 		NPuzzle::Solver::SolveWithIDA(NPuzzle::State* start)
{
	NPuzzle::State* 		temp;
	NPuzzle::State*			d_ptr;
	float					threshold;

	threshold = start->GetFCost();
	temp = nullptr;
	d_ptr = nullptr;
	if (*(start) == Solver::GetFinalState())
			return (start);
	// std::cout << start << std::endl;
	while (1)
	{
		// std::cout << "THRESHOLD: " << threshold << std::endl;
		temp = NPuzzle::Solver::StatesDeepening(start, threshold);
		if (d_ptr && !(d_ptr == temp))
			delete d_ptr;
		d_ptr = temp;
		// std::cout << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n" << temp << "$$$$$$$$$$$$$$$$$$$$$$$$$$$$\n";
		if (*(temp) == Solver::GetFinalState())
		{
			while (1);
			return (temp);
		}
		threshold = temp->GetFCost();
	}
	while (1);
}