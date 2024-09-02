#include "SourceState.h"

//	AxesState class
SDL_Point AxesState::Delta(int id) const
{
	const SDL_Point previousPoint = GetPrevious(id);
	const SDL_Point currentPoint = Get(id);
	return {currentPoint.x - previousPoint.x, currentPoint.y - previousPoint.y};
}

//	ButtonsState class
bool ButtonsState::GetPressed(int id) const { return !GetPrevious(id) && Get(id); }

bool ButtonsState::GetReleased(int id) const { return GetPrevious(id) && !Get(id); }
