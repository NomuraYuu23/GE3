#include "Random.h"

Random* Random::GetInstance()
{
	static Random instance;
	return &instance;
}

void Random::Initialize()
{

	std::mt19937 randomEngine(seedGenerator_());
	randomEngine_ = randomEngine;

}
