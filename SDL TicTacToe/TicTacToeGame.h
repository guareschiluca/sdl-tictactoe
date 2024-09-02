#pragma once

#pragma region Engine Includes
#include "IUpdatable.h"
#include "IRenderable.h"
#pragma endregion

#pragma region Game Includes
#include "Tokens.h"
#include "TurnsScheduler.h"
#include "TurnMonitor.h"
#include "Field.h"
#include "HumanTurnController.h"
#include "CPUTurnController.h"
#pragma endregion

/*
 * Tic-Ttac-Toe game implementation. This class handles the game flow
 * and all its components both for update and for render, directly or
 * through its components.
 */
class TicTacToeGame : public IUpdatable, public IRenderable
{
	// Fields
public:
protected:
private:
	const SDL_Rect & viewport;
	SDL_Rect turnMonitorArea;
	SDL_Rect gameFieldArea;
	TurnsScheduler turnsScheduler;
	TurnMonitor turnMonitor;
	Field gameField;
	ATurnController * crossController;
	ATurnController * circleController;
	// Constructors
public:
	TicTacToeGame(const SDL_Rect & viewport, ControlType crossControlType, ControlType circleControlType);
	~TicTacToeGame();
protected:
private:
	// Methods
public:
	//	IUpdatable implementation
	void Update() override;

	//	IRenderable implementation
	const SDL_Rect & GetRect() const override { return viewport; }
	void PreRender(SDL_Renderer * r) override;
	void Render(SDL_Renderer * r) const override;
protected:
private:
	void RefreshViewportAreas();
	ATurnController * CreateTurnControllerForControlType(ControlType controlType, FactionGlyph factionGlyph);
};

