#include "NPuzzle.hpp"

bool		NPuzzle::Solver::SolveWithA(State& start)
{
	std::vector<State> 	opened;
	std::vector<State> 	closed;

	opened.push_back(start);

	while (!opened.empty())
	{
		std::sort(opened.begin(), opened.end(), [](State a, State b) { return (a.GetFCost() < b.GetFCost()); });
		State		temp = opened.front();
		opened.pop_back();		
		closed.push_back(temp);

		if (temp == Solver::GetFinalState())
			return (true);

		std::vector<State> 	childs = temp.GetChildren(GetPuzzleSize(), *this);
		for (State& child : childs)
		{
			// if (child.isInArray(closed))
				// continue ;
			// if in closed and needed to be renewed we need to extract node from closed_list
			if (!child.isInArray(opened) && !child.isInArray(closed))
				opened.push_back(child);	
			else if (child.isInArray(opened) || child.isInArray(closed))
			{
				size_t		f_cost;
				f_cost = child.findBetterFCost(opened);
				child.SetFCost(child.GetFCost() > f_cost ? f_cost : child.GetFCost());
				f_cost = child.findBetterFCost(closed);
				child.SetFCost(child.GetFCost() > f_cost ? f_cost : child.GetFCost());
				opened.push_back(child);
			}
		} 
	}
	return (false);
}

NPuzzle::State 		NPuzzle::Solver::StatesDeepening(NPuzzle::State& temp, const size_t threshold) const
{
	std::vector<NPuzzle::State>		children;
	float							tempFCost = std::numeric_limits<float>::max();
	size_t							pos = 0;
	
	if (threshold > temp.GetFCost())
		return temp;
	children = temp.GetChildren(GetPuzzleSize(), *this);
	for (size_t i = 0; i < children.size(); ++i)
	{
		StatesDeepening(children[i], threshold);
		if (children[i].GetFCost() < tempFCost)
		{
			tempFCost = children[i].GetFCost();
			pos = i;
		}
	}
	return (children[pos]);
}

NPuzzle::State 		NPuzzle::Solver::SolveWithIDA(NPuzzle::State& start) const
{
	NPuzzle::State 		temp;
	float				threshold;

	threshold = start.GetFCost();
	while (1)
	{
		if (temp == Solver::GetFinalState())
			return (temp);
		temp = NPuzzle::Solver::StatesDeepening(start, threshold);
		threshold = temp.GetFCost();
	}
}