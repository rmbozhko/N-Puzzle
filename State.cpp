#include "NPuzzle.hpp"

NPuzzle::State::~State()
{
	delete[] field_; 
}

std::string		NPuzzle::VisitStates(State* st)
{
	State*			temp = st;
	std::string		res;
	int				id = -1;

	while (temp != nullptr)
	{
		const size_t*	state_field = temp->GetField();
		res += std::to_string(++id); 
		res += " [\nshape=plaintext\nlabel=<<table color='black'>\n";
		for (size_t i = 0; i < State::GetPuzzleLen() * State::GetPuzzleLen(); ++i)
		{
			if (i % State::GetPuzzleLen() == 0)
				res += "<tr>";
			res += "<td>" + std::to_string(state_field[i]) + "</td>";
			if ((i + 1) % State::GetPuzzleLen() == 0)
				res += "</tr>\n";
		}
		res += "<tr><td colspan='" + std::to_string(static_cast<size_t>(State::GetPuzzleLen()))
				+ "'>State's f cost: " + std::to_string(static_cast<size_t>(temp->GetFCost())) + "</td></tr>";
		res += "</table>>];\n";
		temp = temp->GetParent();
	}
	for (; id > -1; --id)
	{
		res += "\n" + std::to_string(id);
		if (id - 1 > -1)
			res += "->" + std::to_string(id - 1) + ";";
		else
			res += ";\n";
	}
	std::cout << "res: " << res << std::endl;
	return (res);
}

size_t		NPuzzle::State::findBetterFCost(std::vector<State*>& v) const
{
	size_t		res;

	res = UINT_MAX;
	for (std::vector<State*>::iterator i = v.begin(); i != v.end(); ++i)
		if (GetField() == (*i)->GetField())
			res = (res > (*i)->GetFCost()) ? (*i)->GetFCost() : res; 
	return (res);
}

int			find_in_arr(const size_t* arr, const size_t size, const size_t elem)
{
	for (int i = 0; i < size; ++i)
		if (arr[i] == elem)
			return (i);
	return (-1);
}

std::ostream&		NPuzzle::operator<<(std::ostream &os, const NPuzzle::State& temp)
{
	os << "f_cost: " << temp.GetFCost() << std::endl;
	for (size_t i = 0; i < State::GetPuzzleLen(); ++i)
	{
		for (size_t j = 0; j < State::GetPuzzleLen(); ++j)
			os << temp.TileAt((i * State::GetPuzzleLen()) + j) << " ";
		os << std::endl;
	}
	return (os);
}

std::ostream&		NPuzzle::operator<<(std::ostream &os, const NPuzzle::State* temp)
{
	os << "f_cost: " << temp->GetFCost() << std::endl;
	for (size_t i = 0; i < State::GetPuzzleLen(); ++i)
	{
		for (size_t j = 0; j < State::GetPuzzleLen(); ++j)
			os << temp->TileAt((i * State::GetPuzzleLen()) + j) << " ";
		os << std::endl;
	}
	return (os);
}

float		NPuzzle::State::calcFCost(const float h_cost, const size_t g_cost, bool is_unicost = false) const
{
	float		res;

	res = h_cost + g_cost;
	std::cout << "calcFCost: " << h_cost << ", " << g_cost << std::endl;
	if (is_unicost)
	{
		std::cout << "F_COST_PART: " << GetFCost() << ", G_COST_PART:" << GetGCost() << std::endl;
		res += (GetFCost() - GetGCost());
	}
	return (res);
}

size_t*		prepareField(size_t* field, const size_t x, const size_t y, const size_t pos, const size_t puzzle_len)
{
	//std::cout << "prepareField|Before" << std::endl;
	//NPuzzle::ft_print_arr(field, puzzle_len);
	//std::cout << "X:" << x << "|Y:" << y << "|pos:" << pos << std::endl;
	//std::cout << "-------------------" << std::endl;
	if (pos == 0 && y > 0)
		std::swap(field[y * puzzle_len + x], field[(y - 1) * puzzle_len + x]);
	else if (pos == 1 && y < puzzle_len - 1)
		std::swap(field[y * puzzle_len + x], field[(y + 1) * puzzle_len + x]);
	else if (pos == 2 && x > 0)
		std::swap(field[y * puzzle_len + x], field[y * puzzle_len + (x - 1)]);
	else if (pos == 3 && x < puzzle_len - 1)
		std::swap(field[y * puzzle_len + x], field[y * puzzle_len + (x + 1)]);
	else
		return (nullptr);
	// std::cout << "prepareField|After" << std::endl;
	// NPuzzle::ft_print_arr(field, puzzle_len);
	// std::cout << "---------------------" << std::endl;
	return (field);
}

std::vector<NPuzzle::State*> 	NPuzzle::State::GetChildren(const size_t puzzle_len, const Solver& solv)
{
	std::vector<State*> 		v;
	size_t					pos = static_cast<size_t>(find_in_arr(GetField(), puzzle_len, 0));
	size_t					x = pos % puzzle_len;
	size_t					y = pos / puzzle_len;

	for (size_t i = 0; i < 4; ++i)
	{
		//std::cout << "Child #" << i << std::endl;
		size_t*		new_field = new size_t[puzzle_len * puzzle_len];
		for (size_t i = 0; i < puzzle_len * puzzle_len; ++i)
			new_field[i] = TileAt(i);
		if (prepareField(new_field, x, y, i, puzzle_len) != nullptr)
		{
			//std::cout << "G_COST:" << GetGCost() << std::endl;
			State* temp = new State(new_field,
				calcFCost(solv.calcHeuristic(new_field), GetGCost() + 1, solv.IsUnicost()),
				GetGCost() + 1, this);
			//std::cout << "F_COST:" << temp->GetFCost() << std::endl;
			v.push_back(temp);
		}
		else
			delete[] new_field;
	}
	return (v);
}

bool				NPuzzle::State::isInArray(std::vector<State*>& v) const
{
	for (std::vector<State*>::iterator i = v.begin(); i != v.end(); ++i)
	{
		std::cout << " -- -- - - - - - - - - - - -- " << std::endl;
		if (*this == (*i)->GetField())
			return (true);
	}
	return (false);
}

bool		NPuzzle::State::operator==(const std::pair<size_t*, size_t> final_state) const
{
	const size_t* 	field_state = GetField();
	const size_t	field_lenght = (final_state.second * final_state.second);
	for (size_t i = 0; i < field_lenght; ++i)
		if (field_state[i] != final_state.first[i])
			return (false);
		
	return (true);
}

bool		NPuzzle::State::operator==(const size_t* state) const
{
	const size_t* 	field_state = GetField();
	const size_t	field_lenght = (GetPuzzleLen() * GetPuzzleLen());
	std::cout << "%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%% " << std::endl;
	for (size_t i = 0; i < field_lenght; ++i)
		if (field_state[i] != state[i])
			return (false);
	return (true);
}

size_t NPuzzle::State::puzzle_len_ = 0;