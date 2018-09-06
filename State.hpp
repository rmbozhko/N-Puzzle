#ifndef STATE_HPP
# define STATE_HPP

#include "NPuzzle.hpp"

namespace NPuzzle
{
	class Solver;
	class State
	{
	public:
		State() {}
		State(size_t* field) : field_(field), f_cost_(0) {};
		State(size_t* field, float f_cost, size_t g_cost, State* parent)
				: field_(field), f_cost_(f_cost), g_cost_(g_cost), parent_(parent) {};
		~State();
		std::vector<State*>	GetChildren(const size_t puzzle_len, const Solver& solv);
		const size_t*		GetField() const { return (field_); }
		const size_t		TileAt(const size_t pos) const { return (field_[pos]); }
		bool				isInArray(std::vector<State*>& v) const;
		size_t				findTile(const size_t num) const;
		State*				GetFromArray(std::vector<State*>& v) const;
		float				GetFCost() const { return (f_cost_); }
		void				SetFCost(float f_cost) { f_cost_ = f_cost; }
		float				calcFCost(const float h_cost, const size_t g_cost, bool is_unicost) const;	
		int					GetDuplicate(std::vector<State*>& v) const;
		void 				SetParent(State* parent) { parent_ = parent; }
		State* 				GetParent() const { return (parent_); }
		static size_t		GetPuzzleLen() { return(puzzle_len_); }
		static void			SetPuzzleLen(size_t puzzle_len) { puzzle_len_ = puzzle_len; }
		size_t				GetGCost() const { return (g_cost_); }
		void				SetGCost(size_t g_cost) { g_cost_ = g_cost; }
		bool				operator==(const std::pair<size_t*, size_t> final_state) const;
		bool				operator==(const size_t* state) const;
		State*				operator=(const State* st);
	private:
		State*		parent_;		
		size_t*				field_;
		float				f_cost_;
		size_t				g_cost_;
		static size_t		puzzle_len_;
	};
	std::ostream&				operator<<(std::ostream&, const State&);
	std::ostream&				operator<<(std::ostream &os, const State*);
}

#endif