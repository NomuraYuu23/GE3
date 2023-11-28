#include "PlayerJumpParticle.h"
#include <random>

void PlayerJumpParticle::Initialize(const Vector3& position, const Vector3& size)
{

	Particle::Initialize(position, size);

	color_ = { 1.0f,1.0f,1.0f,1.0f };

	speed_ = 0.3f;

	lifeTime_ = 0.5f;

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distDirection(-1.0f,1.0f);

	direction_ = { distDirection(randomEngine), 0.0f, distDirection(randomEngine) };

}

void PlayerJumpParticle::Update(const Matrix4x4& billBoardMatrix)
{

	transform_.translate.x += speed_ * direction_.x;
	transform_.translate.z += speed_ * direction_.z;

	transform_.translate.y += 0.1f;

	Particle::Update(billBoardMatrix);

}
