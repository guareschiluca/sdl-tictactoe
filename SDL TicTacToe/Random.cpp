#include "Random.h"

random_device Random::rd{};
default_random_engine Random::engine(rd());

int Random::Range(int minInclusive, int maxExclusive)
{
	uniform_int_distribution<int> range(minInclusive, maxExclusive - 1);
	
	return range(engine);
}
float Random::RangeF(float minInclusive, float maxInclusive)
{
	uniform_real_distribution<float> range(minInclusive, maxInclusive);
	
	return range(engine);
}
bool Random::GetChance(float chance)
{
	uniform_real_distribution<float> range(0.0f, 1.0f);
	
	return range(engine) <= chance;
}
