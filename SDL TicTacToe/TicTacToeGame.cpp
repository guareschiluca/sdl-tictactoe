#include "TicTacToeGame.h"

#pragma region C++ Includes
#include <cassert>
#pragma endregion

#pragma region Engine Includes
#include "Input.h"
#pragma endregion

#pragma region Constant Parameters
#define TOP_MARGIN 64
#define BOTTOM_MARGIN 32
#define MONITOR_MARGIN 6
#pragma endregion

TicTacToeGame::TicTacToeGame(const SDL_Rect & viewport, ControlType crossControlType, ControlType circleControlType) :
	viewport(viewport),
	turnMonitor{turnMonitorArea},
	gameField{gameFieldArea},
	crossController(CreateTurnControllerForControlType(crossControlType, FG_Cross)),
	circleController(CreateTurnControllerForControlType(circleControlType, FG_Circle))
{
	//	Create controllers
	turnsScheduler.AddTurn(crossController);
	turnsScheduler.AddTurn(circleController);

	//	Make first areas calculation
	RefreshViewportAreas();
}

TicTacToeGame::~TicTacToeGame()
{
	if(crossController)
	{
		delete crossController;
		crossController = nullptr;
	}
	if(circleController)
	{
		delete circleController;
		circleController = nullptr;
	}
}

void TicTacToeGame::Update()
{
	if(gameField.IsGameOn())
	{
		//	Broadcast update to relevant components
		turnsScheduler.Update();

		//	Define current turn
		const ATurnController * currentTurnController = dynamic_cast<const ATurnController *>(turnsScheduler.GetCurrentTurn());
		if(currentTurnController)
			turnMonitor.SetGlyph(currentTurnController->GetFactionGlyph());
		else
			turnMonitor.ClearGlyph();
	}
	else if(Input::Get().GetMouseButtonPressed(1))
	{	//	LMB (or touch emulation) pressed during this frame during game over.
		/*
		 * Here we're handling an input outside of a controller class.
		 * This is far from ideal, instead it should be better to listen
		 * to controllers but this would require a refactoring as the
		 * controllers are now bound to their turn.
		 * Ok for this project, not for a real world scenario.
		 */
		turnsScheduler.StartOver();
		gameField.Reset();
	}
}

void TicTacToeGame::PreRender(SDL_Renderer * r)
{
	/*
	 * Recalculating everything every time is not wise. In
	 * this trivial project this is not a problem but in a
	 * real world scenario it would be wise to recalculate
	 * only when the viewport actually changed.
	 */

	//	Update all viewport elements to accomodate viewport size
	RefreshViewportAreas();

	//	Fnally, broadcast pre-render event to relevant components
	turnMonitor.PreRender(r);
	gameField.PreRender(r);
}

void TicTacToeGame::Render(SDL_Renderer * r) const
{
	//	Render the turn monitor only when the game is on
	if(gameField.IsGameOn())
		turnMonitor.Render(r);

	//	Broadcast render to relevant components
	gameField.Render(r);
}

void TicTacToeGame::RefreshViewportAreas()
{
	//	Define the turns monitor's area
	turnMonitorArea.w = viewport.w;
	turnMonitorArea.h = TOP_MARGIN - MONITOR_MARGIN;
	turnMonitorArea.x = viewport.x;
	turnMonitorArea.y = viewport.y + MONITOR_MARGIN;

	//	Define the field's area
	gameFieldArea.w = viewport.w;
	gameFieldArea.h = viewport.h - (TOP_MARGIN + BOTTOM_MARGIN);
	gameFieldArea.x = viewport.x;
	gameFieldArea.y = viewport.y + TOP_MARGIN;
}

ATurnController * TicTacToeGame::CreateTurnControllerForControlType(ControlType controlType, FactionGlyph factionGlyph)
{
	switch(controlType)
	{
		case CT_Human:
			return new HumanTurnController(gameField, factionGlyph);
		case CT_CPU_Easy:
			return new CPUTurnController(Difficulty::Easy, gameField, factionGlyph);
		case CT_CPU:	//	This shouldn't ever be set but let's default it to CT_CPU_Medium
		case CT_CPU_Medium:
			return new CPUTurnController(Difficulty::Medium, gameField, factionGlyph);
		case CT_CPU_Hard:
			return new CPUTurnController(Difficulty::Hard, gameField, factionGlyph);
		default:
			assert(false);	//	This shouldn't happen
			return nullptr;
	}
}
