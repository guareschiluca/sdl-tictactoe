#include "Field.h"

#pragma region C++ includes
#include <algorithm>
#include <cassert>
#pragma endregion

#pragma region Engine Includes
#include "Random.h"
#pragma endregion

#pragma region Game Includes
#include "Drawing.h"
#pragma endregion

using namespace std;

#pragma region Constant Parameters
#define COL_FIELD 200, 200, 200, 255
#pragma endregion

/*
 * Solutions to the Tic-Tac-Toe game are few and fixed
 * so it's a good idea to store them statically so
 * they're available to any function which may need
 * them and are not duplicated in case more than one
 * field is instantiated.
 */

int Field::winCombos[8][3] =
	{
		{0, 1, 2},
		{3, 4, 5},
		{6, 7, 8},
		{0, 3, 6},
		{1, 4, 7},
		{2, 5, 8},
		{0, 4, 8},
		{2, 4, 6}
	};

Field::Field(const SDL_Rect & area) :
	area(area)
{
	//	Make sure to initialize the class in a consistent state
	CalculateFieldMetrics();
	Reset();
}

void Field::Reset()
{
	//	Fill the field with empty cells
	fill(begin(cells), end(cells), FG_None);
	
	/*
	 * Instead of iterating through the field, checking
	 * empty cells and adding them to a vector every time
	 * anybody needs to know what are empty cells, we
	 * preferred storing a vector of empty cells and
	 * modifying it any time the field changes.
	 * 
	 * In this case we're clearing any possible value
	 * previously stored in the vector and adding back
	 * all cells, since as we restart the game the field
	 * gets cleared and all cells are empty.
	 * 
	 * See Field::MakeMove() for more details
	 */

	//	Empty cells are all cells
	emptyCells.clear();
	for(int c = 0; c < sizeof(cells) / sizeof(cells[0]); c++)
		emptyCells.push_back(c);
}

bool Field::TestCell(SDL_Point point, int & row, int & col) const
{
	//	Iterate cells to check if point is in any of them (using reference variable so they're read at return)
	for(row = 0; row < 3; row++)
		for(col = 0; col < 3; col++)
			if(SDL_PointInRect(&point, &cellsAreas[row][col]))
				return true;

	//	Point not in any cell, unset coords and reurn failure
	row = -1;
	col = -1;
	return false;
}

int Field::GetRandomEmptyCell() const
{
	if(emptyCells.size() < 1)
		return -1;
	
	if(emptyCells.size() == 1)
		return emptyCells[0];
	
	return emptyCells[Random::Range(0, (int)emptyCells.size())];
}

int Field::GetMoveScore(FactionGlyph glyph, int row, int col) const
{
	//	Convert from matrix to linear index
	int cellIndex = row * 3 + col;

	if(cells[cellIndex] != FG_None)
		return -5;

	//	Init best score to a neutral value
	int bestComboScore = -5;

	for(auto winCombo : winCombos)
	{
		int comboIndex = -1;
		for(int c = 0; c < 3; c++)
		{
			//	Find placement of cell in combo
			if(winCombo[c] == cellIndex)
			{
				comboIndex = c;
				break;
			}
		}

		//	Cell not in this combo? Skip
		if(comboIndex < 0)
			continue;

		//	Check if winning combo
		int score = -5;
		for(int c = 0; c < 3; c++)
		{
			if(c == comboIndex)
				continue;
			if(cells[winCombo[c]] == FG_None)
				score += 1;
			else if(cells[winCombo[c]] == glyph)
				score += 2;
			else
				score -= 3;
		}

		//	If move makes a combo, return a high score
		if(score > bestComboScore)
			bestComboScore = score;
	}

	return bestComboScore;
}

int Field::FindBestMove(FactionGlyph glyph, bool * isConclusiveMove) const
{
	/*
	 * To find the best move here we iterate empty cells,
	 * which are the available moves, and for each we check
	 * whether that cell contiìributes to a winning combo.
	 * The higher the number of the same symbol in a combo,
	 * the hgher the score of that move.
	 * 
	 * Optionaly, we can fill a bool pointer that tells whether
	 * the chosen move is a winning move; this is useful to
	 * the AI that can decide for a winning move over a
	 * defensive move.
	 */

	//	Prepare decision making
	int bestScore = -10;
	int bestMove = -1;

	//	Find the best among the availabe moves
	for(const int & move : emptyCells)
	{
		const int row = move / 3;
		const int col = move % 3;

		const int moveScore = GetMoveScore(glyph, row, col);
		if(moveScore > bestScore)
		{
			bestScore = moveScore;
			bestMove = move;
		}
	}

	//	Check if is a conclusive move
	if(isConclusiveMove)
		*isConclusiveMove = bestScore >= 4;

	//	Return the best move
	assert(bestMove > -1);
	return bestMove;
}

bool Field::MakeMove(int row, int col, FactionGlyph glyph)
{
	//	Calculate linear index
	int index = row * 3 + col;

	//	Check that the cell is not already taken
	if(cells[index] != FG_None)
		return false;
	
	//	Fill the cell with the move's glyph
	cells[index] = glyph;

	/*
	 * Instead of iterating through the field, checking
	 * empty cells and adding them to a vector every time
	 * anybody needs to know what are empty cells, we
	 * preferred storing a vector of empty cells and
	 * modifying it any time the field changes.
	 *
	 * In this case we're taking the cell interested by the
	 * current move out of the vector of empty cells.
	 * This minimizes the interactions with the vector,
	 * which may be heavy operations.
	 *
	 * See Field::Reset() for more details
	 */

	//	Remove cell from empty cells
	emptyCells.erase(remove(emptyCells.begin(), emptyCells.end(), index));

	return true;
}

bool Field::IsFull() const
{
	for(int i = 0; i < 9; i++)
		if(cells[i] == FG_None)
			return false;
	
	return true;
}

FactionGlyph Field::GetWinner() const
{
	for(int combo = 0; combo < 8; combo++)
		if(
			cells[winCombos[combo][0]] != FG_None &&
			cells[winCombos[combo][0]] == cells[winCombos[combo][1]] &&
			cells[winCombos[combo][0]] == cells[winCombos[combo][2]]
		)
			return cells[winCombos[combo][0]];
	
	return FG_None;
}

FactionGlyph Field::GetCell(int row, int col) const
{
	return cells[row * 3 + col];
}

void Field::PreRender(SDL_Renderer * r)
{
	//	Refresh metrics
	CalculateFieldMetrics();
}

void Field::Render(SDL_Renderer * r) const
{
	//	Render the current screen, based on the game's state
	if(IsGameDraw())
		RenderGameDrawScreen(r);
	else if(IsGameWon())
		RenderGameWonScreen(r);
	else
		RenderGameScreen(r);
}

void Field::CalculateFieldMetrics()
{
	//	Calculate squared field area
	const int axis = min(area.w, area.h);
	fieldArea.w = axis;
	fieldArea.h = axis;
	fieldArea.x = area.x + area.w / 2 - fieldArea.w / 2;
	fieldArea.y = area.y + area.h / 2 - fieldArea.h / 2;

	//	Calculate cells areas
	const int cellSize = fieldArea.w / 3;

	for(int row = 0; row < 3; row++)
		for(int col = 0; col < 3; col++)
			cellsAreas[row][col] = {
				fieldArea.x + col * cellSize,
				fieldArea.y + row * cellSize,
				cellSize,
				cellSize
		};

	//	Calculate glyph radius
	glyphRadius = (int)((cellSize - 10) / 2);
}

void Field::RenderGameScreen(SDL_Renderer * r) const
{
	for(int row = 0; row < 3; row++)
		for(int col = 0; col < 3; col++)
		{
			SDL_SetRenderDrawColor(r, COL_FIELD);
			SDL_RenderDrawRect(r, &cellsAreas[row][col]);
			DrawGlyph(
				r,
				GetCell(row, col),
				cellsAreas[row][col].x + cellsAreas[row][col].w / 2,
				cellsAreas[row][col].y + cellsAreas[row][col].h / 2,
				glyphRadius
			);
		}
}

void Field::RenderGameDrawScreen(SDL_Renderer * r) const
{
	assert(IsFull());	//	Shouldn't render draw screen if not actually draw

	//	Draw the "draw" word scattered among the cells
	DrawChar(r, &cellsAreas[0][0], 'd');
	DrawChar(r, &cellsAreas[0][1], 'r');
	DrawChar(r, &cellsAreas[1][1], 'a');
	DrawChar(r, &cellsAreas[1][2], 'w');
}

void Field::RenderGameWonScreen(SDL_Renderer * r) const
{
	assert(GetWinner() != FG_None);	//	Shouldn't render win screen if there's no winner

	//	Draw the winner's glyph
	DrawGlyph(
		r,
		GetWinner(),
		cellsAreas[0][1].x + cellsAreas[0][1].w / 2,
		cellsAreas[0][1].y + cellsAreas[0][1].h / 2,
		glyphRadius
	);

	//	Draw the "wins" word scattered among the cells
	DrawChar(r, &cellsAreas[1][0], 'w');
	DrawChar(r, &cellsAreas[1][1], 'i');
	DrawChar(r, &cellsAreas[2][1], 'n');
	DrawChar(r, &cellsAreas[2][2], 's');
}
