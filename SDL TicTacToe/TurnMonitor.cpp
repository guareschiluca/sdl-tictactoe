#include "TurnMonitor.h"

#pragma region C++ Includes
#include <algorithm>
#pragma endregion

#pragma region Game Includes
#include "Drawing.h"
#pragma endregion


using namespace std;

TurnMonitor::TurnMonitor(const SDL_Rect & area, FactionGlyph initialGlyph) :
	area(area),
	currentGlyph(initialGlyph)
{
	CalculateMoitorMetrics();
}

void TurnMonitor::PreRender(SDL_Renderer * r)
{
	CalculateMoitorMetrics();
}

void TurnMonitor::Render(SDL_Renderer * r) const
{
	DrawGlyph(
		r,
		currentGlyph,
		monitorArea.x + monitorArea.w / 2,
		monitorArea.y + monitorArea.h / 2,
		monitorArea.h / 2
	);
}

void TurnMonitor::CalculateMoitorMetrics()
{
	/*
	 * For a matter of coherence here we're calculating
	 * an inscribed square in the arbitrary rect area so
	 * the monitor will always be squared and centered in
	 * the assigned area.
	 * This is actually a useless operation as the render
	 * function only calls the DrawGlyph function which
	 * doesn't require a rect, instead it requires only
	 * the center of a rect to draw the glyph around.
	 */

	//	Calculate squared monitor area
	const int axis = min(area.w, area.h);
	monitorArea.w = axis;
	monitorArea.h = axis;
	monitorArea.x = area.x + area.w / 2 - monitorArea.w / 2;
	monitorArea.y = area.y + area.h / 2 - monitorArea.h / 2;
}
