#include "TurnsScheduler.h"

#pragma region C++ Includes
#include <cassert>
#include <algorithm>
#pragma endregion


void TurnsScheduler::AddTurn(ITurnsReceiver * turnToAdd)
{
	//	Do not add the same turn twice!!
	assert(find(turns.begin(), turns.end(), turnToAdd) == turns.end());

	//	Add the new turn
	turns.push_back(turnToAdd);

	//	First turn also becomes the current turn
	if(!currentTurn)
	{
		currentTurn = turnToAdd;
		currentTurn->OnTurnBegan();
	}
}

void TurnsScheduler::RemoveTurn(ITurnsReceiver * turnToRemove)
{
	//	Do not remove a turn if not in list!!
	assert(find(turns.begin(), turns.end(), turnToRemove) != turns.end());

	//	Remove the requested turn
	turns.erase(remove(turns.begin(), turns.end(), turnToRemove));
}

void TurnsScheduler::StartOver()
{
	//	Conclude the current turn, if any, if on
	if(currentTurn)
	{
		assert(currentTurn->IsOnTurn());
		currentTurn->OnTurnFinished();
	}

	//	Start over from first turn
	if(turns.size() < 1)
		currentTurn = nullptr;
	else
		currentTurn = turns[0];

	//	Begin the first turn
	currentTurn->OnTurnBegan();
}

void TurnsScheduler::Update()
{
	//	Nothing to update if no turn is ongoing
	if(!currentTurn)
		return;	//	alternatively we could set the first turn as current turn, but it would require a check to be sure there are enough turns

	//	Broadcast the turn update message to the current turn receiver
	currentTurn->OnTurnUpdate();

	//	Check if the turn is concluded, in case advance to next turn
	if(currentTurn->ConsumeConcluded())
		AdvanceTurn();
}

void TurnsScheduler::AdvanceTurn()
{
	//	No turns, do nothing
	if(turns.empty())
		return;

	//	Conclude current turn
	if(currentTurn)
		currentTurn->OnTurnFinished();

	//	Find current turn's iterator within the vector
	auto turnIterator = find(turns.begin(), turns.end(), currentTurn);

	//	We know that the current element is in the collection, so we asusme that the find()... found!
	assert(turnIterator != turns.end());	//	we know that

	//	Advance to next turn
	/*
	 * Here we have a scenario where now it's the turn #, but in the meantime
	 * someone has removed that turn (API allows this).
	 * In such a case the turnIterator will be turns.end() since it won't be
	 * found.
	 * Handling this scenario is as easy as this:
	 * if(turnIterator != turns.end())
	 *		//... what follows
	 * The point here is that this game doesn't need this feature at all, the
	 * turns scheduler itself overshoots the needs of this process and has
	 * been implemented just to give some sort of structure to the code.
	 * So, yeah, wanna port this class to another porject which will use it
	 * in a more complete way? Uncomment the above example and remove the assert.
	 * In this limited project we just assert that this scenario must not
	 * happen.
	 */
	turnIterator++;

	//	If turn reached the end of the vector, start over from the first turn
	if(turnIterator == turns.end())
		turnIterator = turns.begin();

	//	Store the advanced iterator into the current turn
	currentTurn = *turnIterator;

	//	Notify the current turn that its turn just began
	assert(currentTurn);	//	This shouldn't ever trigger, it would mean that an element in the vector became nullptr and it's unlikely to happen
	currentTurn->OnTurnBegan();
}
