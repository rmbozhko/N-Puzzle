#ifndef SOLVER_HPP
# define SOLVER_HPP
#include "NPuzzle.hpp"

namespace NPuzzle
{
	enum HEURISTIC
	{
		MANHATTAN=0,
		EUCLIDE,
		MIS_TILES,
		LIN_CONF,		
	};
	
	class State;
	
	class Solver
		{
		public:
			Solver(size_t puzzle_len=0, size_t* final_state=nullptr, std::string viz_str="", HEURISTIC heuristic=MANHATTAN, bool is_unicost=true)
						: puzzle_len_(puzzle_len), final_state_(final_state), viz_str_(viz_str), heuristic_(heuristic), is_unicost_(is_unicost)
						{};
			~Solver() {};
			bool		SolveWithA(State& start);
			bool		SolveWithIDA(State& start);
			State 		StatesDeepening(State& temp, const size_t threshold);
			bool		CheckIfSolvable(const NPuzzle::State& st) const;
			float		ManhattanDist(const int* final_state, const int* curr_state);
			float		EuclideanDist(const int* final_state, const int* curr_state);
			float		MisplacedTiles(const int* final_state, const int* curr_state);
			static size_t*		GenerateFinalState(const size_t puzzle_len);
			// const size_t*		GetStartState() { return (start_state_.GetField()); }
			const std::pair<size_t*, size_t>		GetFinalState() { return (std::make_pair(final_state_, this->GetPuzzleSize())); }
			const size_t		GetPuzzleSize() const { return (puzzle_len_); }
			const std::string&	GetVisStr() const { return (viz_str_); }

			void			SetVisStr(std::string& viz_str)
			{
				if (viz_str.size() > 0)
					viz_str_ = viz_str;
				else
					throw std::string("Wrongly formatted string for XDoT");
			}

			void				SetPuzzleSize(const size_t len)
			{
				if (len > 20)
					throw std::string("Passed puzzle size is too big to be handled");
				else
					puzzle_len_ = len;
			}
		private:
			size_t					puzzle_len_;
			size_t*					final_state_;
			std::string 			viz_str_;
			HEURISTIC 				heuristic_;
			bool					is_unicost_;
		};
		
		
		std::pair<Solver, State> 		ReadData(const char* filename);
}

#endif