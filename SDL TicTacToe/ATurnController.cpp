#include "ATurnController.h"

#pragma region C++ Includes
#include <cassert>
#pragma endregion


ATurnController::ATurnController(FactionGlyph factionGlyph) :
	factionGlyph(factionGlyph)
{ }

void ATurnController::OnTurnBegan()
{
	onTurn = true;
	TurnOpeningOperations();
}

void ATurnController::OnTurnUpdate()
{
	assert(IsOnTurn());

	TurnUpdateOperations();
}

void ATurnController::OnTurnFinished()
{
	TurnClosingOperations();
	onTurn = false;
}
