#pragma once

#pragma region C++ Includes
#include <vector>
#pragma endregion

#pragma region Game Includes
#include "Tokens.h"
#include "IUpdatable.h"
#include "ITurnsReceiver.h"
#pragma endregion

using namespace std;

/*
 * Core gameplay class, manages the correct sequence of
 * turns and schedules the turn's lifecycle. Strictly
 * bound to the ITurnsReceiver, uses that interface to
 * communicate with arbitrary implementations which need
 * to hook to the turns sequence and its internal lifecycle.
 */
class TurnsScheduler : public IUpdatable
{
	// Fields
public:
protected:
private:
	vector<ITurnsReceiver * > turns;
	ITurnsReceiver * currentTurn = nullptr;
	// Constructors
public:
protected:
private:
	// Methods
public:
	void AddTurn(ITurnsReceiver * turnToAdd);
	void RemoveTurn(ITurnsReceiver * turnToRemove);
	void StartOver();
	const ITurnsReceiver * GetCurrentTurn() const { return currentTurn; }

	//	IUpdatable implementation
	void Update() override;
protected:
private:
	void AdvanceTurn();
};

