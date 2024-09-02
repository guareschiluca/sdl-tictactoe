#pragma once

/*
 * Colleciton of game-specific data.
 */

enum FactionGlyph
{
	FG_None,
	FG_Cross,
	FG_Circle
};

enum ControlType
{
	CT_Human = 1 << 0,
	CT_CPU = 1 << 1,
	CT_CPU_Easy = CT_CPU | 1 << 2,
	CT_CPU_Medium = CT_CPU | 1 << 3,
	CT_CPU_Hard = CT_CPU | 1 << 4
};
