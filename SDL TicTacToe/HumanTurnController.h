#pragma once

#pragma region Game Includes
#include "ATurnController.h"
#include "Field.h"
#pragma endregion

/*
 * Controller responsible for human interaction with the
 * game. It implements ATurnController, which in turn
 * implements ITurnsReceiver, so its lifecycle is entirely
 * scheduled by the TurnsScheduler.
 * All this class does is responding to the turn's update
 * message and handle the player's input to commit a move.
 * There's no need to make checks if this is the correct
 * moment to take actions, because the turns scheduler
 * already sends messages only to the relevant receiver
 * during its turn.
 */
class HumanTurnController : public ATurnController
{
	// Fields
public:
protected:
private:
	Field & gameField;
	// Constructors
public:
	HumanTurnController(Field & gameField, FactionGlyph factionGlyph);
protected:
private:
	// Methods
public:
protected:
private:
	//	ATurnController implementation
	void TurnOpeningOperations() { }
	void TurnUpdateOperations();
	void TurnClosingOperations() { }
};

