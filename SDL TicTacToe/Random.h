#pragma once

#include <random>

using namespace std;

/*
 * Utility class for generation of random numbers
 */
class Random
{
	static random_device rd;
	static default_random_engine engine;
public:
	//	Generate a random integer in the given range [min; max)
	static int Range(int minInclusive, int maxExclusive);
	//	Generate a random float in the given range [min; max]
	static float RangeF(float minInclusive, float maxInclusive);
	//	Generate a random number in the [0; 1] range and check it against a chance
	static bool GetChance(float chance);
};
