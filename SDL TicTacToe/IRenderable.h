#pragma once

#include "SDL.h"

/*
 * Common interface for all objects that can be
 * displayed on a render target.
 */
class IRenderable
{
public:
	virtual const SDL_Rect & GetRect() const = 0;
	virtual void PreRender(SDL_Renderer * r) { }
	virtual void Render(SDL_Renderer * r) const = 0;
};

