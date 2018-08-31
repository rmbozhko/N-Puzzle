#include "Solver.hpp"

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

std::vector<size_t> 	ConvertToVector(std::vector<std::vector<size_t>>& f_state)
{
	std::vector<size_t> 		v;

	for (size_t i = 0; i < f_state.size(); i++)
		for (size_t j = 0; j < f_state.size(); j++)
			v.push_back(f_state[i][j]);
	return (v);
}

// int increasing()
// { 
//     static size_t i = 1;
//     return i++;
// }

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

		std::vector<State> 	childs = temp.GetChilds(this->GetPuzzleSize());
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