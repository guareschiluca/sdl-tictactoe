#pragma once

#include <map>

#include <SDL.h>

using namespace std;

/*
 * =============================================================
 * Classes defiend in this header are designed to be mainly used
 * by an input manager.
 * =============================================================
 */

/*
 * Template class for hardware states, implements a double buffer
 * based on maps where the key identifies the specific source and
 * the value identifies the state of that specific source.
 * Set operations are made on the curretn buffer, read operations
 * take into account either buffers and the buffers transition is
 * triggered by the State::Step funciton, to be called at the
 * beginning of each iteration.
 */
template<typename ValueType>
class State
{
private:
	map<int, ValueType> previous;
	map<int, ValueType> current;
public:
	void Set(int id, ValueType newValue) { current[id] = newValue; }
	void Step()
	{
		for(const auto & row : current)
			previous[row.first] = row.second;
	}
	ValueType Get(int id) const { return GetValue(id, current); }
protected:
	ValueType GetPrevious(int id) const { return GetValue(id, previous); }
private:
	ValueType GetValue(int id, const std::map<int, ValueType> & pool) const
	{
		auto it = pool.find(id);
		if(it == pool.end())
			return ValueType();
		return it->second;
	}
};

/*
 * Specialized state class for axes where the value type is set to
 * SDL_Point. Useful for mouse or joystick axes.
 * 1D axes must be mapped to SDL_Points so just one component,
 * typically the X component, will be used.
 */
class AxesState : public State<SDL_Point>
{
public:
	SDL_Point Delta(int id) const;
	/*
	 * Performance can be optimized by caching the delta
	 * value each time Set is called, and then just returning
	 * a copy or a reference to that value.
	 * This qould prevent to calculate the same value multiple
	 * times if the delta is queried more than once during
	 * the same iteration, but this is a small inefficiency
	 * (we're talking about two integer subtractions and the
	 * creation of a struct) and would require a tiny refactoring
	 * of the template class to allow post-set operations.
	 * This project doesn't need that level of optimization.
	 * 
	 * Also, such an optimization should be profiled since the
	 * time spent making two subtractions and an allocation on
	 * the stack, with subsequent copy, may not be more efficient
	 * than the virtual table for the virtual method called at
	 * every Set and then implemented in the sub-class.
	 */
};

/*
 * Specialized state class for buttons where the value type is set to
 * bool. Useful for mouse, keyboard or gamepad buttons.
 */
class ButtonsState : public State<bool>
{
public:
	bool GetPressed(int id) const;
	bool GetReleased(int id) const;
};
