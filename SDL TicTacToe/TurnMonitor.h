#pragma once

#pragma region Engine Includes
#include "IRenderable.h"
#pragma endregion

#pragma region Game Includes
#include "Tokens.h"
#pragma endregion

/*
 * Simple widget which displays the glyph for the current turn
 * so that players know it's their time to make a move.
 * It has nearly no internal logic: other than get/set and
 * display the current glyph, it only ensures that the glyph
 * gets rendered in a squared area incribed in the given
 * arbitrary rect that has been assigned to it.
 */
class TurnMonitor : public IRenderable
{
	// Fields
public:
protected:
private:
	const SDL_Rect & area;
	SDL_Rect monitorArea;
	FactionGlyph currentGlyph;
	// Constructors
public:
	TurnMonitor(const SDL_Rect & area, FactionGlyph initialGlyph = FG_None);
protected:
private:
	// Methods
public:
	FactionGlyph GetGlyph() const { return currentGlyph; }
	void SetGlyph(FactionGlyph newGlyph) { currentGlyph = newGlyph; }
	void ClearGlyph() { SetGlyph(FG_None); }

	//	IRenderable implementation
	const SDL_Rect & GetRect() const override { return monitorArea; }
	void PreRender(SDL_Renderer * r) override;
	void Render(SDL_Renderer * r) const override;
protected:
private:
	void CalculateMoitorMetrics();
};

