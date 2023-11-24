#include "PlayerWalkParticle.h"

void PlayerWalkParticle::Initialize(const Vector3& position, const Vector3& size)
{

	Particle::Initialize(position, size);

	color_ = { 1.0f,1.0f,1.0f,1.0f };

	speed_ = 0.1f;

	lifeTime_ = 0.5f;

}

void PlayerWalkParticle::Update(const Matrix4x4& billBoardMatrix)
{

	transform_.translate.y += speed_;

	Particle::Update(billBoardMatrix);

}
