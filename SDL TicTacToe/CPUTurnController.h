#pragma once

#pragma region SDL_Includes
#include <SDL_timer.h>
#pragma endregion

#pragma region Game Includes
#include "ATurnController.h"
#include "Field.h"
#pragma endregion

/*
 * A helper enum used by this class only to make decisions
 * based on the chosen difficulty level. This is mostly
 * used internally by this class, it's visible outside
 * because it needs to be passed as a parameter to the
 * constructor or to the CPUTurnController::SetDifficulty()
 * funciton.
 */

enum class Difficulty
{
	Easy,
	Medium,
	Hard
};

/*
 * Controller responsible for CPU interaction with the
 * game. It implements ATurnController, which in turn
 * implements ITurnsReceiver, so its lifecycle is entirely
 * scheduled by the TurnsScheduler.
 * All this class does is simulating a "thinking..." delay
 * and then calculates a move and performs it on the field.
 * To calculate the move, takes into account different
 * possibilities and makes a choice, ,that can be better or
 * worse,based on the difficulty.
 * There's no need to make checks if this is the correct
 * moment to take actions, because the turns scheduler
 * already sends messages only to the relevant receiver
 * during its turn.
 */
class CPUTurnController : public ATurnController
{
	// Fields
public:
protected:
private:
	Difficulty difficulty;
	Field & gameField;
	Uint64 turnEndTime;
	float defenseChance;
	float offenseChance;
	bool prioritizeWinningMove;
	// Constructors
public:
	CPUTurnController(Difficulty initialDifficulty, Field & gameField, FactionGlyph factionGlyph);
protected:
private:
	// Methods
public:
	void SetDifficulty(Difficulty newDifficulty);
protected:
private:
	static Uint32 GetTurnDuration(Difficulty difficulty);

	//	ATurnController implementation
	void TurnOpeningOperations();
	void TurnUpdateOperations();
	void TurnClosingOperations() { }
};

