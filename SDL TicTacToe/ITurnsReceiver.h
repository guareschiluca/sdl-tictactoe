#pragma once

/*
 * Common interface for anything whose execution is
 * strictly related to turns. The TurnsScheduler uses
 * this interface to send relevant messages at the
 * right time, so implementations of this interface
 * do not need to bother about when to do something:
 * if they receive the message, it's their turn.
 */
class ITurnsReceiver
{
public:
	virtual void OnTurnBegan() = 0;
	virtual void OnTurnUpdate() = 0;
	virtual void OnTurnFinished() = 0;
	virtual bool IsOnTurn() const = 0;

	virtual bool PeekConcluded() const = 0;
	virtual bool ConsumeConcluded() = 0;
};
