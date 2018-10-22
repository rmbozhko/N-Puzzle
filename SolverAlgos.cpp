#include "NPuzzle.hpp"

void				FreeStorage(std::vector<NPuzzle::State*>& v);

NPuzzle::State*				copyState(NPuzzle::State* st);

NPuzzle::State*		NPuzzle::Solver::SolveWithA(std::vector<State*>& opened, std::vector<State*>& closed)
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
			// while (1);
			std::cout << "Here" << std::endl;
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
				opened.push_back(copyState(children[i]));	
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
						opened.push_back(copyState(children[i]));
						delete opened[pos];
						opened.erase(opened.begin() + pos);
					}
				}
			}
		}
		FreeStorage(children);
	}
	return (nullptr);
}
