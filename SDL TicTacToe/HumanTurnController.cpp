#include "HumanTurnController.h"

#pragma region Engine Includes
#include "Input.h"
#pragma endregion


HumanTurnController::HumanTurnController(Field & gameField, FactionGlyph factionGlyph) :
	ATurnController(factionGlyph),
	gameField(gameField)
{ }

void HumanTurnController::TurnUpdateOperations()
{
	//	Check a click happened this frame
	if(!Input::Get().GetMouseButtonPressed(1))
		return;

	//	Check the click happened in a cell
	int row, col;
	if(!gameField.TestCell(Input::Get().GetMousePosition(), row, col))
		return;

	//	Check it was an empty cell
	if(gameField.GetCell(row, col) != FG_None)
		return;

	//	Perform move
	gameField.MakeMove(row, col, GetFactionGlyph());

	//	Conclude turn
	Conclude();
}
