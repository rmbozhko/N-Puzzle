#ifndef NPUZZLE_HPP
# define NPUZZLE_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <vector>
#include "Solver.hpp"
#include "State.hpp"
#include <cmath>
#include <limits>
#include <cstdlib>

namespace NPuzzle {
	void		ft_print_arr(std::vector<size_t> arr, const size_t len);
	void		ft_print_arr(const size_t* arr, const size_t len);
	size_t*		CopyVectorToPtr(std::vector<size_t>& v);
	// size_t*		CopyPtrToPtr(const size_t* field);
}

#endif