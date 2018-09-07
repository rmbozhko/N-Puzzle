#include "NPuzzle.hpp"

static void 	HandleCOUT(const NPuzzle::State* temp, int& id, const size_t& len)
{
	std::cout << "H-cost: " << (static_cast<size_t>(temp->GetFCost() - temp->GetGCost()))
						<< ", G-Cost: " << temp->GetGCost() << "\n\t";
	for (size_t i = 0; i < len * len; ++i)
	{
		if (temp->TileAt(i) == 0)
			std::cout << "\033[1;31m" << temp->TileAt(i) << "\033[0m"; 
		else
			std::cout << temp->TileAt(i);
		std::cout << (((i + 1) % len == 0) ? "\n\t" : " ");
	}
	std::cout << std::endl;
	++id;
}

static int	GetMovedTilePosition(const NPuzzle::State* const st, const NPuzzle::State* const parent)
{
	if (parent == nullptr) {
		return (-1);
	}
	else
	{
		const size_t*		curr_field = st->GetField();
		const size_t*		parent_field = parent->GetField();
		const int		 	puzzle_len = NPuzzle::State::GetPuzzleLen() * NPuzzle::State::GetPuzzleLen();
		
		for (int i = 0; i < puzzle_len; ++i) {
			if ((curr_field[i]) && curr_field[i] != parent_field[i]) {
				return (i);
			}
		}
	}

	return (-1);
}

static void 	HandleXdot(const NPuzzle::State* temp, const size_t* state_field, int& id, std::string& res, const size_t& len)
{
	res += std::to_string(++id); 
	res += " [\nshape=plaintext\nlabel=<<table color='black'>\n";
	const int	moved_tile_pos = GetMovedTilePosition(temp, temp->GetParent());
	for (size_t i = 0; i < len * len; ++i)
	{
		if (i % len == 0)
			res += "<tr>";
		res += "<td ";
		if (state_field[i] == 0)
			res += "bgcolor='red'";
		else if (moved_tile_pos != -1 && i == moved_tile_pos)
			res += "bgcolor='green'";
		res += ">" + std::to_string(state_field[i]) + "</td>";
		if ((i + 1) % len == 0)
			res += "</tr>\n";
	}
	res += "<tr><td colspan='" + std::to_string(static_cast<size_t>(len))
				+ "'>State's f cost: " + std::to_string(static_cast<size_t>(temp->GetFCost())) + "</td></tr>";
	res += "</table>>];\n";
}

void		NPuzzle::Solver::VisitStates(State* st, bool isDot)
{
	State*			temp = st;
	std::string		res;
	int				id = -1;
	size_t			len = State::GetPuzzleLen();
	
	while (temp != nullptr)
	{
		if (isDot)
			HandleXdot(temp, temp->GetField(), id, res, len);
		else
			HandleCOUT(temp, id, len);
		temp = temp->GetParent();
	}
	
	if (isDot)
	{
		for (; id > -1; --id)
		{
			res += "\n" + std::to_string(id);
			if (id - 1 > -1)
				res += "->" + std::to_string(id - 1) + ";";
			else
				res += ";\n";
		}
		SetVisStr(res);
		std::ofstream		file("solution.dot");
		if (!file.is_open())
			throw std::string("Couldn't open file for visualization");
		SetVisStr(std::string("}\n"));
		file << GetVisStr();
		file.close();
	}
	else
	{
		std::cout << "Total number of states: "  << GetTotalNumOfStates() << std::endl;
		std::cout << "Max number of states: " << GetMaxNumOfStates() << std::endl;
		std::cout << "Number of moves: " << id << std::endl;
	}
}

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

static void		ConstructState(std::vector<std::vector<size_t>>& arr,
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
		ConstructState(arr, row + 1, col + 1, lvl, iter);
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
    // ft_print_arr(seq, puzzle_len * puzzle_len);
    std::vector<size_t>::const_iterator i = seq.begin();
    ConstructState(final_state, 0, 0, puzzle_len, i);
    // for (int i = 0; i < final_state.size(); ++i)
    	// ft_print_arr(final_state[i], puzzle_len);  
    return (&ConvertToVector(final_state)[0]);
}

static std::vector<std::vector<size_t>> 	ConvertTo2DVector(const size_t* state, const size_t len)
{	
	std::vector<std::vector<size_t>>	v;
	v.resize(len, std::vector<size_t>(len, 0));

	for (size_t i = 0; i < len; ++i)
		for (size_t j = 0; j < len; ++j)
			v[i][j] = state[i * len + j];

    return (v);	
}

static void		ft_handle_square(const std::vector<std::vector<size_t>>& arr, size_t row, size_t col,
									std::vector<size_t>& v)
{
	v.push_back(arr[row][col]);
	v.push_back(arr[row][col + 1]);
	v.push_back(arr[row + 1][col + 1]);
	v.push_back(arr[row + 1][col]);
}

static void		GetCurrStateAsSnail(const std::vector<std::vector<size_t>>& arr,
										size_t row, size_t col,	const size_t lvl,
										std::vector<size_t>& v)
{
	if (lvl - (row * 2) == 1)
		v.push_back(arr[row][col]);
	else if (lvl - (row * 2) == 2)
		ft_handle_square(arr, row, col, v);
	else
	{
		for (size_t i = row, j = col; j < lvl - col; ++j)
			v.push_back(arr[i][j]);
		for (size_t i = row + 1, j = lvl - col - 1; i < lvl - row; ++i)
			v.push_back(arr[i][j]);
		for (size_t i = lvl - row - 1, j = lvl - col - 2; j > col; --j)
			v.push_back(arr[i][j]);
		for (size_t i = lvl - row - 1, j = col; i > row; --i)	
			v.push_back(arr[i][j]);
		GetCurrStateAsSnail(arr, row + 1, col + 1, lvl, v);
	}
}

static size_t	CalcInversions(const std::vector<std::vector<size_t>>& arr, const size_t len)
{
	std::vector<size_t> v;
	size_t				inv_count = 0;

	GetCurrStateAsSnail(arr, 0, 0, len, v);
	for (size_t i = 0; i < (len * len); i++)
	{
        for (size_t j = i + 1; j < (len * len); j++)
        {
        	inv_count += (v[j] && v[i] &&  v[i] > v[j]) ? 1 : 0;
        }
	}
	return (inv_count);
}

bool		NPuzzle::Solver::IsSolvable(const NPuzzle::State* st) const
{
	size_t const		len = State::GetPuzzleLen();
	auto const			v = ConvertTo2DVector(st->GetField(), len);
	size_t const		inv_num = CalcInversions(v, len);
	
	return (((inv_num % 2) == 0) ? true : false);
 }