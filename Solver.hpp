#ifndef SOLVER_HPP
# define SOLVER_HPP
#include "NPuzzle.hpp"

namespace NPuzzle
{
	typedef float (*Heuristic)(const size_t*, const size_t*);

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
			Solver(size_t puzzle_len=0, size_t* final_state=nullptr, std::string viz_str="", HEURISTIC heuristic=MANHATTAN, bool is_unicost=true, size_t maxNumOfStates=0, size_t totalNumOfStates_=0)
						: puzzle_len_(puzzle_len), final_state_(final_state), viz_str_(viz_str), heuristic_(heuristic), is_unicost_(is_unicost), maxNumOfStates_(maxNumOfStates), totalNumOfStates_(totalNumOfStates_)
						{};
			~Solver() {};
			State*		SolveWithA(State* start, std::vector<State*>& opened, std::vector<State*>& closed);
			State* 		StatesDeepening(State* temp, const size_t threshold) const;
			State* 		SolveWithIDA(State* start) const;
			bool		IsSolvable(const NPuzzle::State* st) const;
			float		calcHeuristic(const size_t* curr_state) const;
			float		ManhattanDist(const size_t* final_state, const size_t* curr_state, bool LinearConflict) const;
			float		EuclideanDist(const size_t* final_state, const size_t* curr_state) const;
			float		MisplacedTiles(const size_t* final_state, const size_t* curr_state) const;
			static size_t*		GenerateFinalState(const size_t puzzle_len);
			const std::pair<size_t*, size_t>		GetFinalState() const { return (std::make_pair(final_state_, this->GetPuzzleSize())); }
			const size_t		GetPuzzleSize() const { return (puzzle_len_); }
			HEURISTIC 	GetHeuristic() const { return (heuristic_); }
			bool		IsUnicost() const { return (is_unicost_); }
			const std::string&	GetVisStr() const { return (viz_str_); }
			void			SetVisStr(std::string viz_str)
			{
				if (viz_str.size() > 0)
					viz_str_ += viz_str;
				else
					throw std::string("Wrongly formatted string for XDoT");
			}
			size_t		GetMaxNumOfStates() const { return(maxNumOfStates_); }
			void		SetMaxNumOfStates(size_t num) { maxNumOfStates_ = (num > maxNumOfStates_) ? num : maxNumOfStates_; }
			size_t		GetTotalNumOfStates() const { return (totalNumOfStates_); }
			void		SetTotalNumOfStates(size_t num) { totalNumOfStates_ = num; }
			void		IncreaseTotalNumOfStates() { totalNumOfStates_++; }
			void		VisitStates(State* st, bool isDot=false);

		private:
			size_t					puzzle_len_;
			size_t*					final_state_;
			std::string 			viz_str_;
			HEURISTIC 				heuristic_;
			bool					is_unicost_;
			size_t					maxNumOfStates_;
			size_t					totalNumOfStates_;
		};
		

		std::pair<Solver*, State*> 		ReadData(const char* filename);
}

#endif