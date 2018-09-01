#include "NPuzzle.hpp"

void		ft_handle_square(std::vector<std::vector<size_t>>& arr, size_t row, size_t col, std::vector<size_t>::const_iterator& iter)
{
	arr[row][col] = (*iter);
	iter++;
	arr[row][col + 1] = (*iter);
	iter++;
	arr[row + 1][col + 1] = (*iter);
	iter++;
	arr[row + 1][col] = (*iter);
}

void		ConstructFinalState(std::vector<std::vector<size_t>>& arr,
										size_t row, size_t col,	const size_t lvl,
										std::vector<size_t>::const_iterator& iter)
{
	if (lvl - (row * 2) == 1)
		arr[row][col] = (*iter);
	else if (lvl - (row * 2) == 2)
		ft_handle_square(arr, row, col, iter);
	else
	{
		for (size_t i = row, j = col; j < lvl - col; ++j, ++iter)
			arr[i][j] = (*iter);
		for (size_t i = row + 1, j = lvl - col - 1; i < lvl - row; ++i, ++iter)
			arr[i][j] = (*iter);
		for (size_t i = lvl - row - 1, j = lvl - col - 2; j > col; --j, ++iter)
			arr[i][j] = (*iter);
		for (size_t i = lvl - row - 1, j = col; i > row; --i, ++iter)	
			arr[i][j] = (*iter);
		ConstructFinalState(arr, row + 1, col + 1, lvl, iter);
	}
}

float		NPuzzle::Solver::calcHeuristic(const size_t* curr_state) const
{
	float		res;
	HEURISTIC 	func_num;

	func_num = GetHeuristic(); 
	switch(func_num)
	{
		case MANHATTAN:
			res = NPuzzle::Solver::ManhattanDist(GetFinalState().first, curr_state, false);
			break ;
		case EUCLIDE:
			res = NPuzzle::Solver::EuclideanDist(GetFinalState().first, curr_state);
			break ;
		case MIS_TILES:
			res = NPuzzle::Solver::MisplacedTiles(GetFinalState().first, curr_state);
			break ;
		case LIN_CONF:
			res = NPuzzle::Solver::ManhattanDist(GetFinalState().first, curr_state, true);
			break ;
		default:
			throw std::string("Wrong defined heuristic function");
	}

	return (res);
}

std::pair<size_t, size_t>		findArrElem(const size_t* final_state, const size_t size, const size_t elem)
{
	std::pair<size_t, size_t> 	goal;
	size_t 						i;

	for (i = 0; i < (size * size); ++i)
		if (final_state[i] == elem)
			break ;
	return (std::make_pair(i % size, static_cast<size_t>(i / size)));
}

float		NPuzzle::Solver::ManhattanDist(const size_t* final_state, const size_t* curr_state, bool LinearConflict = false) const
{
	float		dist;

	dist = 0;
	for (size_t i = 0; i < (this->GetPuzzleSize() * this->GetPuzzleSize()); ++i)
	{
		if (curr_state[i] != 0)
		{
			std::pair<size_t, size_t> goal;	
			goal = findArrElem(final_state, this->GetPuzzleSize(), curr_state[i]);
			dist += std::abs(static_cast<double>((i % this->GetPuzzleSize()) - goal.first))
						+ std::abs(static_cast<double>((i / this->GetPuzzleSize()) - goal.second));
		}
	}
	// if (LinearConflict)
		// dist += ft_add_lin_conf();
	return (dist);
}

float		NPuzzle::Solver::MisplacedTiles(const size_t* final_state, const size_t* curr_state) const
{
	float		num;

	num = 0;
	for (size_t i = 0; i < (this->GetPuzzleSize() * this->GetPuzzleSize()); ++i)
	{
		if (curr_state[i] != 0)
		{
			std::pair<size_t, size_t> goal;	
			goal = findArrElem(final_state, this->GetPuzzleSize(), curr_state[i]);
			if (!std::abs(static_cast<double>((i % this->GetPuzzleSize()) - goal.first))
					&& !std::abs(static_cast<double>(i / this->GetPuzzleSize())) - goal.second)
				num += 1.0;
		}
	}
	return (num);
}

float		NPuzzle::Solver::EuclideanDist(const size_t* final_state, const size_t* curr_state) const
{
	float		dist;

	dist = 0;
	for (size_t i = 0; i < (this->GetPuzzleSize() * this->GetPuzzleSize()); ++i)
	{
		if (curr_state[i] != 0)
		{
			std::pair<size_t, size_t> 	goal;
			float						result;

			goal = findArrElem(final_state, this->GetPuzzleSize(), curr_state[i]);
			result = std::pow(std::abs(static_cast<double>((i % this->GetPuzzleSize()) - goal.first)), 2.0)
						+ std::pow(std::abs(static_cast<double>((i / this->GetPuzzleSize()) - goal.second)), 2.0);
			if (result != 0.0)
				dist += std::sqrt(result);
		}
	}
	return (dist);
} 

std::vector<size_t> 	ConvertToVector(std::vector<std::vector<size_t>>& f_state)
{
	std::vector<size_t> 		v;

	for (size_t i = 0; i < f_state.size(); i++)
		for (size_t j = 0; j < f_state.size(); j++)
			v.push_back(f_state[i][j]);
	return (v);
}

void		ft_print_arr(std::vector<size_t> arr, const size_t len)
{
	for (size_t i = 0; i < arr.size(); ++i)
	{
		if (i && i % len == 0)
			std::cout << std::endl;
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}

void		ft_print_arr(const size_t* arr, const size_t len)
{
	for (size_t i = 0; i < (len * len); ++i)
	{
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}

size_t*			 	NPuzzle::Solver::GenerateFinalState(const size_t puzzle_len)
{
	std::vector<size_t> 					seq(puzzle_len * puzzle_len);
	std::vector<std::vector<size_t>>		final_state;
	final_state.resize(puzzle_len, std::vector<size_t>(puzzle_len, 0));

    std::generate(seq.begin(), seq.end(), []() { static size_t i = 1; return (i++); });
    seq[seq.size() - 1] = 0;
    ft_print_arr(seq, puzzle_len * puzzle_len);
    std::vector<size_t>::const_iterator i = seq.begin();
    ConstructFinalState(final_state, 0, 0, puzzle_len, i);
    for (int i = 0; i < final_state.size(); ++i)
    	ft_print_arr(final_state[i], puzzle_len);  
    return (&ConvertToVector(final_state)[0]);
}

bool		NPuzzle::Solver::CheckIfSolvable(const NPuzzle::State& st) const
{
	size_t		inv_count = 0;

    for (size_t i = 0; i < (this->GetPuzzleSize() * this->GetPuzzleSize()) - 1; i++)
        for (size_t j = i + 1; j < (this->GetPuzzleSize() * this->GetPuzzleSize()); j++)
        	inv_count += (st.TileAt(j) && st.TileAt(i) &&  st.TileAt(i) > st.TileAt(j)) ? 1 : 0;
    if ((inv_count % 2) != 0)
    	throw std::string("Passed puzzle set is unsolvable");
    return (true);
}

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