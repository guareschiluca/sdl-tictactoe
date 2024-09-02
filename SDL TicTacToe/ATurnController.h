#pragma once

#pragma region Game Includes
#include "Tokens.h"
#include "ITurnsReceiver.h"
#pragma endregion

/*
 * This class acts as a middle class for any
 * controller who will take part to a turn.
 * Its main goal is to respond to turns' events
 * and give the possibility to sub-classes to
 * implement custom logic.
 */
class ATurnController : public ITurnsReceiver
{
	// Fields
public:
protected:
private:
	bool concluded = false;
	bool onTurn = false;
	FactionGlyph factionGlyph;
	// Constructors
public:
	ATurnController(FactionGlyph factionGlyph);
	virtual ~ATurnController() { }
protected:
private:
	// Methods
public:
	FactionGlyph GetFactionGlyph() const { return factionGlyph; }

	//	ITurnsReceiver implementation
	void OnTurnBegan() override;
	void OnTurnUpdate() override;
	void OnTurnFinished() override;
	__inline bool IsOnTurn() const override { return onTurn; }
	__inline bool PeekConcluded() const override { return concluded; }
	__inline bool ConsumeConcluded() override { const bool concludedCache = concluded; concluded = false; return concludedCache; }
protected:
	__inline void Conclude() { concluded = true; }
private:
	virtual void TurnOpeningOperations() = 0;
	virtual void TurnUpdateOperations() = 0;
	virtual void TurnClosingOperations() = 0;
};
