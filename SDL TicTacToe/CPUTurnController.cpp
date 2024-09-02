#include "CPUTurnController.h"

#pragma region C++ Include
#include <cassert>
#pragma endregion

#pragma region Engine Includes
#include "Random.h"
#pragma endregion

CPUTurnController::CPUTurnController(Difficulty initialDifficulty, Field & gameField, FactionGlyph factionGlyph) :
	ATurnController(factionGlyph),
	gameField(gameField)
{
	SetDifficulty(initialDifficulty);
}

void CPUTurnController::SetDifficulty(Difficulty newDifficulty)
{
	//	Update difficulty
	difficulty = newDifficulty;

	/*
	 * Put it simple, based on the difficulty, the AI
	 * prefers more thoughtful moves or more random moves.
	 */

	//	Calculate AI parameters based on the new difficulty
	switch(difficulty)
	{
		case Difficulty::Easy:
			defenseChance = 0.25f;
			offenseChance = 0.25f;
			prioritizeWinningMove = false;
			break;
		case Difficulty::Medium:
			defenseChance = 0.75f;
			offenseChance = 0.5f;
			prioritizeWinningMove = true;
			break;
		case Difficulty::Hard:
			defenseChance = 1.0f;
			offenseChance = 1.0f;
			prioritizeWinningMove = true;
			break;
		default:
			assert(false);	//	Shouldn't happen unless a new difficulty is introduced and not handled
			defenseChance = 0.25f;
			offenseChance = 0.25f;
			prioritizeWinningMove = false;
			break;
	}
}

Uint32 CPUTurnController::GetTurnDuration(Difficulty difficulty)
{
	/*
	 * Just giving random numbers to simulate that
	 * the AI needs time to find out a move.
	 */
	switch(difficulty)
	{
		case Difficulty::Easy:
			return (Uint32)Random::Range(350, 650);
		case Difficulty::Medium:
			return (Uint32)Random::Range(650, 1050);
		case Difficulty::Hard:
			return (Uint32)Random::Range(1000, 1600);
		default:
			assert(false);	//	Shouldn't happen unless a new difficulty is introduced and not handled
			return 100u;
	}
}

void CPUTurnController::TurnOpeningOperations()
{
	turnEndTime = SDL_GetTicks64() + GetTurnDuration(difficulty);
}

void CPUTurnController::TurnUpdateOperations()
{
	/*
	 * A good implementation of the AI for this game
	 * would have been the mnimax algorithm.
	 * This one is a simplicistic AI algorith, based
	 * on a concept of priorities:
	 * - Can the opponent win with a move? Block it!
	 * - Can I close with a move? Do it!
	 * - None of those? Make a move that enhances my
	 *		current position
	 * Actually not so far from a minimax iplementation
	 * but this is a bit more dumb (no recursion, no dpth).
	 */

	//	Wait until the turn ends (faking the AI's speculations)
	if(SDL_GetTicks64() < turnEndTime)
		return;

	//	Storing opponent's glyph to evaluate its best move and find a defensive move
	const FactionGlyph opponentFactionGlyph = GetFactionGlyph() == FG_Cross ? FG_Circle : FG_Cross;

	//	Find a defensive move
	bool opponentConclusiveMove;	//	Actually not used...
	const int defensiveMove = gameField.FindBestMove(opponentFactionGlyph, &opponentConclusiveMove);

	//	Find an offensive move
	bool myConclusiveMove;
	const int offensiveMove = gameField.FindBestMove(GetFactionGlyph(), &myConclusiveMove);

	//	Find fallback move
	const int fallbackMove = gameField.GetRandomEmptyCell();

	//	Make decision based on difficulty
	int chosenMove;
	if(
		defensiveMove > -1 &&
		Random::GetChance(defenseChance) &&
		(!myConclusiveMove || !prioritizeWinningMove) //	Don't make a defensive move if you can conclude
		)
		chosenMove = defensiveMove;
	else if(
		offensiveMove > -1 &&
		Random::GetChance(offenseChance)
		)
		chosenMove = offensiveMove;
	else if(fallbackMove > -1)
		chosenMove = fallbackMove;
	else
	{
		assert(false);	//	Shouldn't ever happen, unless there's at least one empty cell
	}

	//	Perform move
	gameField.MakeMove(chosenMove / 3, chosenMove % 3, GetFactionGlyph());

	//	Conclude turn
	Conclude();
}
