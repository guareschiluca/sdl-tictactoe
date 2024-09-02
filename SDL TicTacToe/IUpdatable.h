#pragma once

/*
 * Common interface for anything that needs to
 * receive a message once per frame.
 */
class IUpdatable
{
public:
	virtual void Update() = 0;
};
