#include "NPuzzle.hpp"

static void		ft_handle_square(std::vector<std::vector<size_t>>& arr, size_t row, size_t col, std::vector<size_t>::const_iterator& iter)
{
	arr[row][col] = (*iter);
	iter++;
	arr[row][col + 1] = (*iter);
	iter++;
	arr[row + 1][col + 1] = (*iter);
	iter++;
	arr[row + 1][col] = (*iter);
}

static void		ConstructFinalState(std::vector<std::vector<size_t>>& arr,
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

static size_t* 	ConvertToVector(std::vector<std::vector<size_t>>& f_state)
{
	size_t* 		v;

	v = new size_t[f_state.size() * f_state.size()];
	for (size_t i = 0; i < f_state.size(); i++)
		for (size_t j = 0; j < f_state.size(); j++)
			v[(i * f_state.size()) + j] = f_state[i][j];
	return (v);
}

void		NPuzzle::ft_print_arr(std::vector<size_t> arr, const size_t len)
{
	for (size_t i = 0; i < arr.size(); ++i)
	{
		if (i && i % len == 0)
			std::cout << std::endl;
		std::cout << arr[i] << " ";
	}
	std::cout << std::endl;
}

void		NPuzzle::ft_print_arr(const size_t* arr, const size_t len)
{
	for (size_t i = 0; i < (len * len); ++i)
	{
		if (i && i % len == 0)
			std::cout << std::endl;
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

bool		NPuzzle::Solver::CheckIfSolvable(const NPuzzle::State* st) const
{
	size_t		inv_count = 0;

    for (size_t i = 0; i < (GetPuzzleSize() * GetPuzzleSize()) - 1; i++)
        for (size_t j = i + 1; j < (GetPuzzleSize() * GetPuzzleSize()); j++)
        	inv_count += (st->TileAt(j) && st->TileAt(i) &&  st->TileAt(i) > st->TileAt(j)) ? 1 : 0;
    if ((inv_count % 2) != 0)
    	throw std::string("Passed puzzle set is unsolvable");
    return (true);
}