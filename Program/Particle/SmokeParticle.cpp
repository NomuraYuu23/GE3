#include "SmokeParticle.h"
#include <random>

void SmokeParticle::Initialize(const Vector3& position, const Vector3& size)
{

	Particle::Initialize(position, size);

	color_ = { 0.2f,0.2f,0.2f,1.0f };

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distSpeed(0.1f, 0.3f);
	std::uniform_real_distribution<float> distlifeTime(0.1f, 0.5f);

	speed_ = distSpeed(randomEngine);

	lifeTime_ = 0.5f;

}

void SmokeParticle::Update(const Matrix4x4& billBoardMatrix)
{

	transform_.translate.y += speed_;

	Particle::Update(billBoardMatrix);

}
