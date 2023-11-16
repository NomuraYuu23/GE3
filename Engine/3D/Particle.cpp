#include "Particle.h"
#include "../base/BufferResource.h"
#include "../base/WinApp.h"
#include "../base/DirectXCommon.h"
#include "ParticleManager.h"
#include <cassert>
#include <numbers>
#include <random>
#include "../Math/DeltaTime.h"

Particle::~Particle(){}

void Particle::Initialize()
{

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);

	transform_.scale = {1.0f,1.0f,1.0f};
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = { distribution(randomEngine) , distribution(randomEngine), distribution(randomEngine) };

	worldMatrix_ = matrix4x4Calc->MakeIdentity4x4();

	color_ = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine), 1.0f };

	lifeTime_ = distTime(randomEngine);

	currentTime_ = 0.0f;

	useBillBoard_ = true;

	UpdateMatrix(matrix4x4Calc->MakeIdentity4x4());

	isDead_ = false;

}

void Particle::Update(const Matrix4x4& billBoardMatrix)
{
	if (lifeTime_ <= currentTime_) {
		isDead_ = true;
	}
	TimeElapsed();
	GraduallyDisappear();
	UpdateMatrix(billBoardMatrix);

}

void Particle::UpdateMatrix(const Matrix4x4& billBoardMatrix)
{

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	if (useBillBoard_) {
		Matrix4x4 scaleMatrix = matrix4x4Calc->MakeScaleMatrix(transform_.scale);
		Matrix4x4 translateMatrix = matrix4x4Calc->MakeTranslateMatrix(transform_.translate);
		worldMatrix_ = matrix4x4Calc->Multiply(scaleMatrix, matrix4x4Calc->Multiply(billBoardMatrix, translateMatrix));
	}
	else {
		worldMatrix_ = matrix4x4Calc->MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	}
}

ParticleForGPU Particle::Map(const ViewProjection& viewProjection)
{

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();
	ParticleManager* particleManager = ParticleManager::GetInstance();

	ParticleForGPU particleForGPU;
	particleForGPU.World = worldMatrix_;
	particleForGPU.WVP = matrix4x4Calc->Multiply(worldMatrix_, viewProjection.viewProjectionMatrix_);
	particleForGPU.color = color_;

	return particleForGPU;

}

void Particle::TimeElapsed()
{

	currentTime_ += kDeltaTime_;
	if (currentTime_ >= lifeTime_) {
		currentTime_ = lifeTime_;
	}

}

void Particle::GraduallyDisappear()
{

	color_.w = 1.0f - (currentTime_ / lifeTime_);

}