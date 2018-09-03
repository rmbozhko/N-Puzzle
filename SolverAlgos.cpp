#include "NPuzzle.hpp"

NPuzzle::State*		NPuzzle::Solver::SolveWithA(State* start)
{
	std::vector<State*> 	opened;
	std::vector<State*> 	closed;

	opened.push_back(start);
	while (!opened.empty())
	{
		std::sort(opened.begin(), opened.end(), [](State* a, State* b) { return (a->GetFCost() < b->GetFCost()); });
		if (opened.size())
		{
			std::cout << "????????????????????????????" << std::endl;
			for (std::vector<State*>::iterator i = opened.begin(); i != opened.end(); ++i)
				std::cout << (*i);
			// exit(0);
		}
		State*		temp = opened.front();

		if (*temp == Solver::GetFinalState())
		{
			std::cout << temp;
			return (temp);
		}
		opened.erase(opened.begin());
		closed.push_back(temp);

		std::vector<State*> 	children = temp->GetChildren(GetPuzzleSize(), *this);
		std::cout << temp;
		std::cout << opened.size() << std::endl;
		std::cout << children.size() << std::endl;
		for (size_t i = 0; i < children.size(); ++i)
		{
			// if (children[i]->isInArray(closed))
				// continue ;
			// if in closed and needed to be renewed we need to extract node from closed_list
			if (!children[i]->isInArray(opened) && !children[i]->isInArray(closed))
				opened.push_back(children[i]);	
			else if (children[i]->isInArray(opened) || children[i]->isInArray(closed))
			{
				size_t		f_cost;
				f_cost = children[i]->findBetterFCost(opened);
				children[i]->SetFCost(children[i]->GetFCost() > f_cost ? f_cost : children[i]->GetFCost());
				f_cost = children[i]->findBetterFCost(closed);
				children[i]->SetFCost(children[i]->GetFCost() > f_cost ? f_cost : children[i]->GetFCost());
				opened.push_back(children[i]);
			}
		}
		if (children.size())
		{
			std::cout << "###########################" << std::endl;
			for (std::vector<State*>::iterator i = opened.begin(); i != opened.end(); ++i)
				std::cout << (*i);
			// exit(0);
		}
		// if (opened.size() > 6)
			// exit(0);
	}
	return (nullptr);
}

NPuzzle::State* 		NPuzzle::Solver::StatesDeepening(NPuzzle::State* temp, const size_t threshold) const
{
	std::vector<NPuzzle::State*>		children;
	float								tempFCost = std::numeric_limits<float>::max();
	size_t								pos = 0;
	
	if (threshold > temp->GetFCost())
		return temp;
	children = temp->GetChildren(GetPuzzleSize(), *this);
	for (size_t i = 0; i < children.size(); ++i)
	{
		StatesDeepening(children[i], threshold);
		if (children[i]->GetFCost() < tempFCost)
		{
			tempFCost = children[i]->GetFCost();
			pos = i;
		}
	}
	return (children[pos]);
}

NPuzzle::State* 		NPuzzle::Solver::SolveWithIDA(NPuzzle::State* start) const
{
	NPuzzle::State* 		temp;
	float					threshold;

	threshold = start->GetFCost();
	while (1)
	{
		if (*temp == Solver::GetFinalState())
			return (temp);
		temp = NPuzzle::Solver::StatesDeepening(start, threshold);
		threshold = temp->GetFCost();
	}
}