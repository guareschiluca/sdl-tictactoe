#pragma once

#pragma region C++ Includdes
#include <vector>
#pragma endregion

#pragma region Engine Includes
#include "IRenderable.h"
#pragma endregion

#pragma region Game Includes
#include "Tokens.h"
#pragma endregion

using namespace std;

/*
 * Class representing the game field of the Tic-Tac-Toe game.
 * It manages a 3x3 grid and exposes methods to interact with
 * it, as well as methods to make, evaluate and find moves.
 * Also exposes a set of methods to query the state of the
 * game based on the contents of the grid.
 * 
 * Many AI-related funcitons have been implemented as part of
 * this Field class, instead of being part of the AI controller
 * class. In the scope of this project, it makes no difference
 * and it could seem more natural to move those functions to
 * the AI controller class, since it's the only user. Since it
 * actually makes no difference in this project (we still need
 * a reference to the Field in the AI controller to make
 * decision), we decided to implement those functions here for
 * the following reasons:
 * - data ownership: these funcitons operate on data owned by
 *		the Field class
 * - scalability: these functions may also be relevant to a
 *		player-aid functionallity to make hints or tutorials
 */
class Field : public IRenderable
{
	// Fields
public:
protected:
private:
	const SDL_Rect & area;
	SDL_Rect fieldArea;
	SDL_Rect cellsAreas[3][3];
	int glyphRadius;
	FactionGlyph cells[9];
	vector<int> emptyCells;
	// Constructors
public:
	Field(const SDL_Rect & area);
protected:
private:
	static int winCombos[8][3];
	// Methods
public:
	void Reset();
	bool TestCell(SDL_Point point, int & row, int & col) const;
	__inline const vector<int> & GetEmptyCells() const { return emptyCells; }
	int GetRandomEmptyCell() const;
	int GetMoveScore(FactionGlyph glyph, int row, int col) const;
	int FindBestMove(FactionGlyph glyph, bool * isConclusiveMove = nullptr) const;
	bool MakeMove(int row, int col, FactionGlyph glyph);
	bool IsFull() const;
	bool IsGameWon() const { return GetWinner() != FG_None; }
	bool IsGameDraw() const { return !IsGameWon() && IsFull(); }
	bool IsGameOver() const { return IsGameWon() || IsGameDraw(); }
	bool IsGameOn() const { return !IsGameOver(); }
	FactionGlyph GetWinner() const;
	FactionGlyph GetCell(int row, int col) const;

	//	IRenderable implementation
	const SDL_Rect & GetRect() const override { return fieldArea; }
	void PreRender(SDL_Renderer * r) override;
	void Render(SDL_Renderer * r) const override;
protected:
private:
	void CalculateFieldMetrics();
	void RenderGameScreen(SDL_Renderer * r) const;
	void RenderGameDrawScreen(SDL_Renderer * r) const;
	void RenderGameWonScreen(SDL_Renderer * r) const;
};
