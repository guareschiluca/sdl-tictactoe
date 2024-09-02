#include "Input.h"

void Input::NotifyMouseButtonPressed(int buttonId)
{
	mouseButtonsState.Set(buttonId, true);
}

void Input::NotifyMouseButtonReleased(int buttonId)
{
	mouseButtonsState.Set(buttonId, false);
}

void Input::NotifyMousePosition(SDL_Point newMousePosition)
{
	mousePositionsState.Set(0, newMousePosition);

}

void Input::NotifyButtonPressed(SDL_Keycode buttonID)
{
	buttonsState.Set((int)buttonID, true);
}

void Input::NotifyButtonReleased(SDL_Keycode buttonID)
{
	buttonsState.Set((int)buttonID, false);
}

void Input::PollEvents()
{
	//	Step to next iteration
	mouseButtonsState.Step();
	mousePositionsState.Step();
	buttonsState.Step();

	//	Build new state
	static SDL_Event ev;
	while(SDL_PollEvent(&ev))
	{
		switch(ev.type)
		{
#ifndef __EMSCRIPTEN__
			/*
			 * Not handling quit event when targetting
			 * webgl, for a more platform-specific UX
			 */
			//	System events
			case SDL_QUIT:	//	Borderline event, but it actually comes from input so we're handling it here
				quitRequested = true;
				return;
#endif
			//	Mouse events
			case SDL_MOUSEBUTTONDOWN:
				NotifyMouseButtonPressed(ev.button.button);
				NotifyMousePosition({ev.button.x, ev.button.y});
				break;
			case SDL_MOUSEBUTTONUP:
				NotifyMouseButtonReleased(ev.button.button);
				NotifyMousePosition({ev.button.x, ev.button.y});
				break;
			case SDL_MOUSEMOTION:
				NotifyMousePosition({ev.motion.x, ev.motion.y});
				break;
			//	Keyboard events
			case SDL_KEYDOWN:
				NotifyButtonPressed(ev.key.keysym.sym);
				break;
			case SDL_KEYUP:
				NotifyButtonReleased(ev.key.keysym.sym);
				break;
			default:
				break;
			//	FEATURE: Gamepad events
			//	FEATURE: Touch events (first touch emulates the mouse)
		}
	}
}

bool Input::GetMouseButton(int buttonId) const
{
	return mouseButtonsState.Get(buttonId);
}

bool Input::GetMouseButtonPressed(int buttonId) const
{
	return mouseButtonsState.GetPressed(buttonId);
}

bool Input::GetMouseButtonReleased(int buttonId) const
{
	return mouseButtonsState.GetReleased(buttonId);
}

const SDL_Point Input::GetMousePosition() const
{
	return mousePositionsState.Get(0);
}

const SDL_Point Input::GetMouseDelta() const
{
	return mousePositionsState.Delta(0);
}

bool Input::GetButton(SDL_Keycode buttonId) const
{
	return buttonsState.Get(buttonId);
}

bool Input::GetButtonPressed(SDL_Keycode buttonId) const
{
	return buttonsState.GetPressed(buttonId);
}

bool Input::GetButtonReleased(SDL_Keycode buttonId) const
{
	return buttonsState.GetReleased(buttonId);
}
