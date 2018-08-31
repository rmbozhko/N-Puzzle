#include "State.hpp"

size_t		NPuzzle::State::findBetterFCost(std::vector<State>& v) const
{
	size_t		res;

	res = UINT_MAX;
	for (std::vector<State>::iterator i = v.begin(); i != v.end(); ++i)
		if (this->GetField() == (*i).GetField())
			res = (res > (*i).GetFCost()) ? (*i).GetFCost() : res; 
	return (res);
}

int			find_in_arr(const size_t* arr, const size_t size, const size_t elem)
{
	for (int i = 0; i < size; ++i)
		if (arr[i] == elem)
			return (i);
	return (-1);
}

size_t*		prepareField(const size_t* field, const size_t x, const size_t y, size_t pos, size_t puzzle_len)
{
	std::vector<size_t> 	new_field(field, field + puzzle_len);

	if (pos == 0 && y > 0)
		std::swap(new_field[y * puzzle_len + x], new_field[(y - 1) * puzzle_len + x]);
	else if (pos == 1 && y < puzzle_len - 1)
		std::swap(new_field[y * puzzle_len + x], new_field[(y - 1) * puzzle_len + x]);
	else if (pos == 2 && x > 0)
		std::swap(new_field[y * puzzle_len + x], new_field[y * puzzle_len + (x + 1)]);
	else if (pos == 3 && x < puzzle_len - 1)
		std::swap(new_field[y * puzzle_len + x], new_field[y * puzzle_len + (x + 1)]);
	return (&new_field[0]);
}

std::vector<NPuzzle::State> 	NPuzzle::State::GetChilds(const size_t puzzle_len) const
{
	std::vector<State> 		v;
	size_t					pos = static_cast<size_t>(find_in_arr(this->GetField(), puzzle_len, 0));
	size_t					x = pos % puzzle_len;
	size_t					y = pos / puzzle_len;

	for (size_t i = 0; i < 4; ++i)
	{
		size_t* updated_field = prepareField(this->GetField(), x, y, i, puzzle_len);
		if (updated_field != nullptr)
			v.push_back(State(updated_field));
	}
	return (v);
}

bool		NPuzzle::State::operator==(const std::pair<size_t*, size_t> final_state) const
{
	const size_t* 	field_state = this->GetField();
	const size_t	field_lenght = (final_state.second * final_state.second);
	for (size_t i = 0; i < field_lenght; ++i)
		if (field_state[i] != final_state.first[i])
			return (false);
		
	return (true);
}

bool				NPuzzle::State::isInArray(std::vector<State>& v) const
{
	for (std::vector<State>::iterator i = v.begin(); i != v.end(); ++i)
		if (this->GetField() == (*i).GetField())
			return (true);
	return (false);
}