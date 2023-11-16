#include "Random.h"

Random* Random::GetInstance()
{
	static Random instance;
	return &instance;
}

void Random::Initialize()
{

	Update();

}

void Random::Update()
{

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	randomEngine_ = randomEngine;

}
