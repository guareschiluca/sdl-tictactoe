#include "Drawing.h"

#pragma region C++ Includes
#include <vector>
#pragma endregion


using namespace std;

#pragma region Constant Parameters
//	Color palette
#define COL_CHAR 200, 200, 200, 255
#define COL_CLEAR 32, 32, 32, 255
#define COL_CROSS 12, 52, 243, 255
#define COL_CIRCLE 243, 32, 12, 255

//	Geometry
#define CIRCLE_POINTS 32

//	Trygonometry
#define PI 3.1415293f
#define PI2 (2 * PI)
#define HPI (PI / 2)
#pragma endregion

/*
 * Cross and Circle glyphs are drawn in a really simple way,
 * line by line.
 * Not a clever choice, but a style choice.
 * Style choices are rarely clever.
 */

void DrawCross(SDL_Renderer * r, int x, int y, int radius)
{
	SDL_SetRenderDrawColor(r, COL_CROSS);
	SDL_RenderDrawLine(r, x - radius, y - radius, x + radius, y + radius);
	SDL_RenderDrawLine(r, x + radius, y - radius, x - radius, y + radius);
}

void DrawCircle(SDL_Renderer * r, int x, int y, int radius)
{
	SDL_SetRenderDrawColor(r, COL_CIRCLE);
	float angleStep = PI2 / CIRCLE_POINTS;
	vector<SDL_Point> points;
	for(int i = 0; i <= CIRCLE_POINTS; i++)
	{
		float angle = angleStep * i;
		points.push_back({ x + (int)(cos(angle) * radius), y + (int)(sin(angle) * radius) });
	}
	SDL_RenderDrawLines(r, points.data(), CIRCLE_POINTS + 1);
}

/*
 * A flexible function to dynamically draw the correct symbol.
 * Better than writing an if or switch statement each time we
 * need to draw a glyph.
 */

void DrawGlyph(SDL_Renderer * r, FactionGlyph glyph, int x, int y, int radius)
{
	switch(glyph)
	{
		case FG_Cross:
			DrawCross(r, x, y, radius);
			break;
		case FG_Circle:
			DrawCircle(r, x, y, radius);
			break;
		default:
			break;
	}
}

/*
 * This was a style choice, we wanted to make this project as
 * simple as possible also by referencing as few libraries as
 * possible, so, instead of referencing and initializing
 * SDL_ttf, we decide to draw characters in a more old-fashion
 * way.
 */

void DrawChar(SDL_Renderer * r, const SDL_Rect * area, const char chr, int padding)
{
	//	Shift bounds by the requested padidng, to avoid touching the borders
	SDL_Rect bounds = {
		area->x + padding,
		area->y + padding,
		area->w - padding * 2,
		area->h - padding * 2
	};

	//	Prepare a set of points to be drawn as lines
	vector<SDL_Point> points;

	//	Fill points with vertices according to the character requested to draw
	switch(chr)
	{
		case 'a':
		case 'A':
			points.push_back({bounds.x, bounds.y + bounds.h});
			points.push_back({bounds.x, bounds.y});
			points.push_back({bounds.x + bounds.w, bounds.y});
			points.push_back({bounds.x + bounds.w, bounds.y + bounds.h});
			points.push_back({bounds.x + bounds.w, bounds.y + bounds.h / 2});
			points.push_back({bounds.x, bounds.y + bounds.h / 2});
			break;
		case 'd':
		case 'D':
			points.push_back({bounds.x, bounds.y + bounds.h});
			points.push_back({bounds.x, bounds.y});
			points.push_back({bounds.x + bounds.w, bounds.y + bounds.h / 2});
			points.push_back({bounds.x + bounds.w, bounds.y + bounds.h});
			points.push_back(points[0]);	//	Close loop
			break;
		case 'i':
		case 'I':
			points.push_back({bounds.x + bounds.w / 2, bounds.y});
			points.push_back({bounds.x + bounds.w / 2, bounds.y + bounds.h});
			break;
		case 'n':
		case 'N':
			points.push_back({bounds.x, bounds.y + bounds.h});
			points.push_back({bounds.x, bounds.y});
			points.push_back({bounds.x + bounds.w, bounds.y + bounds.h});
			points.push_back({bounds.x + bounds.w, bounds.y});
			break;
		case 'r':
		case 'R':
			points.push_back({bounds.x, bounds.y + bounds.h});
			points.push_back({bounds.x, bounds.y});
			points.push_back({bounds.x + bounds.w, bounds.y + bounds.h / 3});
			points.push_back({bounds.x, bounds.y + (bounds.h / 3) * 2});
			points.push_back({bounds.x + bounds.w, bounds.y + bounds.h});
			break;
		case 's':
		case 'S':
			points.push_back({bounds.x + bounds.w, bounds.y});
			points.push_back({bounds.x, bounds.y});
			points.push_back({bounds.x, bounds.y + bounds.h / 2});
			points.push_back({bounds.x + bounds.w, bounds.y + bounds.h / 2});
			points.push_back({bounds.x + bounds.w, bounds.y + bounds.h});
			points.push_back({bounds.x, bounds.y + bounds.h});
			break;
		case 'w':
		case 'W':
			points.push_back({bounds.x, bounds.y});
			points.push_back({bounds.x, bounds.y + bounds.h});
			points.push_back({bounds.x + bounds.w / 2, bounds.y + bounds.h / 2});
			points.push_back({bounds.x + bounds.w, bounds.y + bounds.h});
			points.push_back({bounds.x + bounds.w, bounds.y});
			break;
	}

	//	Render the lines between the pairs of vertices
	SDL_SetRenderDrawColor(r, COL_CHAR);
	SDL_RenderDrawLines(r, points.data(), (int)points.size());
}
