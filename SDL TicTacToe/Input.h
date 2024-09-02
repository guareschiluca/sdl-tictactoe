#pragma once

#pragma region C++ Includes
#include <map>
#pragma endregion

#pragma region SDL Includes
#include <SDL.h>
#pragma endregion

#pragma region Engine Includes
#include "SourceState.h"
#pragma endregion

using namespace std;

/*
 * Centralized (singleton) input management class.
 * Takes care of polling events and updating the input
 * state.
 * Exposes methods to query that state.
 * 
 * It's designed to just call its PollEvents method from
 * the main loop and then call query method from the game's
 * business logic.
 */
class Input
{
	// Fields
public:
protected:
private:
	ButtonsState mouseButtonsState;
	AxesState mousePositionsState;	//	Would support multi-touch as positions are indexed
	ButtonsState buttonsState;

	bool quitRequested = false;
	// Constructors
public:
	// Delete copy constructor and assignment operator (singleton protection)
	Input(const Input &) = delete;
	Input & operator=(const Input &) = delete;
protected:
private:
	Input() { }
	// Methods
public:
	static Input & Get()
	{
		//	Singleton implementation
		static Input instance;
		return instance;
	}

	/*
	 * Moving the events loop entirely here in the Input class
	 * is not ideal at all. In a real world scenario, we would
	 * want to handle not only input events but also, for
	 * instance, window events such as resize, minimize or
	 * restore.
	 * 
	 * The best approach would be to leave the events loop in
	 * the main loop and, from there, dispatch events to relevant
	 * managers, such as Input or Screen (referring to the above
	 * example).
	 * 
	 * Another approach would be to keep event loops in specialized
	 * classes but filtering events for each class based on their
	 * responsibilities:
	 * see https://wiki.libsdl.org/SDL2/SDL_EventFilter
	 * 
	 * In such a small project, without specific needs, the
	 * decision to make the main loop even simpler may be good,
	 * as there's no will to continue and expand this project.
	 */

	void PollEvents();

	__inline bool WasQuitRequested() const { return quitRequested; }

	/*
	 * The following functions are used to query the state of
	 * the different devices:
	 * - Get*: the current state, regardless of when it became active
	 * - Get*Pressed: whether the state changed to active during this iteration
	 * - Get*Released: wheter the state changed to inactive during this iteration
	 * 
	 * All require an id to identify what you're querying from that group:
	 * - Mouse: an integer identifies the id of the button (SDL noation)
	 * - Keyboard/Gamepad: an SDL_Keycode to identify the key
	 * 
	 * An exception is the mouse position. Despite internal support for
	 * multiple mouse or multi-touch, mouse position and mouse delta do not
	 * need an id when querying.
	 */

	bool GetMouseButton(int buttonId) const;
	bool GetMouseButtonPressed(int buttonId) const;
	bool GetMouseButtonReleased(int buttonId) const;

	const SDL_Point GetMousePosition() const;
	const SDL_Point GetMouseDelta() const;

	bool GetButton(SDL_Keycode buttonId) const;
	bool GetButtonPressed(SDL_Keycode buttonId) const;
	bool GetButtonReleased(SDL_Keycode buttonId) const;
protected:
private:
	/*
	 * The following are internal functions used by the PollEvents
	 * function to update the input states.
	 * They're pretty simple and have been added only to
	 * improve readability, they will most likely be inlined by the
	 * compiler.
	 */

	__inline void NotifyMouseButtonPressed(int buttonId);
	__inline void NotifyMouseButtonReleased(int buttonId);
	__inline void NotifyMousePosition(SDL_Point newMousePosition);
	__inline void NotifyButtonPressed(SDL_Keycode buttonID);
	__inline void NotifyButtonReleased(SDL_Keycode buttonID);
};

