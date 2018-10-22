#include "NPuzzle.hpp"

size_t*		CopyPtrToPtr(const size_t* field);

NPuzzle::State::~State()
{
	delete[] field_; 
}

int		NPuzzle::State::GetDuplicate(std::vector<State*>& v) const
{
	for (int i = 0; static_cast<size_t>(i) < v.size(); ++i)
		if (*this == v[i]->GetField())
			return (i);

	return (-1);
}

size_t				NPuzzle::State::findTile(const size_t num) const
{
	const size_t*		arr = GetField();
	const size_t		puzzle_len = State::GetPuzzleLen() * State::GetPuzzleLen();
	
	if (num > puzzle_len - 1)
		throw std::string("Impossible to find passed tile in current field");
	for (size_t i = 0; i < puzzle_len; ++i)
		if (arr[i] == num)
			return (i);
	return (0);
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
	if (is_unicost)
	{
		res += (GetFCost() - GetGCost());
	}
	return (res);
}

size_t*		NPuzzle::prepareField(size_t* field, const size_t x, const size_t y, const size_t pos, const size_t puzzle_len)
{
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
	return (field);
}

std::vector<NPuzzle::State*> 	NPuzzle::State::GetChildren(const size_t puzzle_len, const Solver& solv)
{
	std::vector<State*> 		v;
	size_t					pos = findTile(0);
	size_t					x = pos % puzzle_len;
	size_t					y = pos / puzzle_len;

	for (size_t i = 0; i < 4; ++i)
	{
		size_t*		new_field = new size_t[puzzle_len * puzzle_len];
		for (size_t i = 0; i < puzzle_len * puzzle_len; ++i)
			new_field[i] = TileAt(i);
		if (prepareField(new_field, x, y, i, puzzle_len) != nullptr)
		{
			State* temp = new State(new_field,
				calcFCost(solv.calcHeuristic(new_field), GetGCost() + 1, solv.IsUnicost()),
				GetGCost() + 1, this);
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
		if (*this == (*i)->GetField())
			return (true);
	}
	return (false);
}

NPuzzle::State*				NPuzzle::State::GetFromArray(std::vector<State*>& v) const
{
	for (size_t i = 0; i < v.size(); ++i)
		if (*this == v[i]->GetField())
			return (v[i]);
	return (nullptr);
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


NPuzzle::State*		NPuzzle::State::operator=(const State* st)
{
	SetField(CopyPtrToPtr(st->GetField()));
	SetFCost(st->GetFCost());
	SetGCost(st->GetGCost());
	SetParent(st->GetParent());
	return (this);
}

bool		NPuzzle::State::operator==(const size_t* state) const
{
	const size_t* 	field_state = GetField();
	const size_t	field_lenght = (GetPuzzleLen() * GetPuzzleLen());
	for (size_t i = 0; i < field_lenght; ++i)
		if (field_state[i] != state[i])
			return (false);
	return (true);
}

size_t NPuzzle::State::puzzle_len_ = 0;