#pragma once

#pragma region SDL Includes
#include <SDL.h>
#pragma endregion

#pragma region Game Includes
#include "Tokens.h"
#pragma endregion

/*
 * Cross and Circle glyphs are drawn in a really simple way,
 * line by line.
 * Not a clever choice, but a style choice.
 * Style choices are rarely clever.
 */

void DrawCross(SDL_Renderer * r, int x, int y, int radius);

void DrawCircle(SDL_Renderer * r, int x, int y, int radius);

/*
 * A flexible function to dynamically draw the correct symbol.
 * Better than writing an if or switch statement each time we
 * need to draw a glyph.
 */

void DrawGlyph(SDL_Renderer * r, FactionGlyph glyph, int x, int y, int radius);

/*
 * This was a style choice, we wanted to make this project as
 * simple as possible also by referencing as few libraries as
 * possible, so, instead of referencing and initializing
 * SDL_ttf, we decide to draw characters in a more old-fashion
 * way.
 */

void DrawChar(SDL_Renderer * r, const SDL_Rect * area, const char chr, int padding = 10);
