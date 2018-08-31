#ifndef STATE_HPP
# define STATE_HPP

#include "NPuzzle.hpp"

namespace NPuzzle
{
	class State
	{
	public:
		State() {}
		State(size_t* field) : field_(field), f_cost_(0) {};
		State(size_t* field, size_t f_cost, size_t g_cost) : field_(field), f_cost_(f_cost), g_cost_(g_cost) {};
		~State() {};
		std::vector<State>	GetChilds(const size_t puzzle_len) const;
		const size_t*		GetField() const { return (field_); }
		const size_t		TileAt(const size_t pos) const { return (field_[pos]); }
		bool				isInArray(std::vector<State>& v) const;
		size_t				GetFCost() const { return (f_cost_); }
		void				SetFCost(size_t f_cost) { f_cost_ = f_cost; }
		size_t				findBetterFCost(std::vector<State>& v) const;
		// void 				SetParent(State& parent) { parent_ = parent; }
		// State 				GetParent() const { return (parent_); }
		size_t				GetGCost() const { return (f_cost_); }
		void				SetGCost(size_t f_cost) { f_cost_ = f_cost; }
		bool				operator==(const std::pair<size_t*, size_t> final_state) const;

	private:
		size_t*		field_;
		size_t		f_cost_;
		size_t		g_cost_;
	};
	static	std::string 		VisitStates(const State& st);
}

#endif