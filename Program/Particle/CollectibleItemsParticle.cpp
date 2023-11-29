#include "CollectibleItemsParticle.h"
#include <random>

void CollectibleItemsParticle::Initialize(const Vector3& position, const Vector3& size)
{

	Particle::Initialize(position, size);

	color_ = { 0.5f,1.0f,0.5f,1.0f };

	speed_ = 0.2f;

	lifeTime_ = 0.8f;

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distDirection(-1.0f, 1.0f);

	direction_ = { distDirection(randomEngine), distDirection(randomEngine), distDirection(randomEngine) };

}

void CollectibleItemsParticle::Update(const Matrix4x4& billBoardMatrix)
{

	transform_.translate.x += speed_ * direction_.x;
	transform_.translate.y += speed_ * direction_.y;
	transform_.translate.z += speed_ * direction_.z;

	Particle::Update(billBoardMatrix);

}

