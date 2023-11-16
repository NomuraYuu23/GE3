#include "Particle.h"
#include "../base/BufferResource.h"
#include "../base/WinApp.h"
#include "../base/DirectXCommon.h"
#include "ParticleManager.h"
#include "../Random/Random.h"
#include <cassert>
#include <numbers>

const float Particle::kDeltaTime_ = 1.0f / 60.0f;

Particle::~Particle(){}

void Particle::Initialize(uint32_t numInstance)
{

	assert(numInstance > 0);

	numInstance_ = numInstance;

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();
	
	Random* random = Random::GetInstance();
	std::mt19937 randomEngine = random->GetRandomEngine();
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);

	basic_ = new Basic[numInstance_];

	for (uint32_t i = 0; i < numInstance_; i++) {

		basic_[i].transform_.scale = {1.0f,1.0f,1.0f};
		basic_[i].transform_.rotate = { 0.0f,0.0f,0.0f };
		basic_[i].transform_.translate = { distribution(randomEngine) , distribution(randomEngine), distribution(randomEngine) };

		basic_[i].worldMatrix_ = matrix4x4Calc->MakeIdentity4x4();

		basic_[i].color_ = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine), 1.0f };

		basic_[i].lifeTime_ = distTime(randomEngine);

		basic_[i].currentTime_ = 0.0f;

		UpdateMatrix(i);

	}

	billBoard_.useIt_ = false;

	billBoard_.matrix_ = matrix4x4Calc->MakeIdentity4x4();

}

void Particle::Update()
{

	for (uint32_t i = 0; i < numInstance_; i++) {
		if (basic_[i].lifeTime_ <= basic_[i].currentTime_) {
			continue;
		}
		TimeElapsed(i);
		GraduallyDisappear(i);
		UpdateMatrix(i);
	}

}

void Particle::UpdateMatrix(uint32_t num)
{

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	if (billBoard_.useIt_) {
		Matrix4x4 scaleMatrix = matrix4x4Calc->MakeScaleMatrix(basic_[num].transform_.scale);
		Matrix4x4 translateMatrix = matrix4x4Calc->MakeTranslateMatrix(basic_[num].transform_.translate);
		basic_[num].worldMatrix_ = matrix4x4Calc->Multiply(scaleMatrix, matrix4x4Calc->Multiply(billBoard_.matrix_, translateMatrix));
	}
	else {
		basic_[num].worldMatrix_ = matrix4x4Calc->MakeAffineMatrix(basic_[num].transform_.scale, basic_[num].transform_.rotate, basic_[num].transform_.translate);
	}
}

void Particle::Map(const ViewProjection& viewProjection, uint32_t indexMap)
{

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();
	ParticleManager* particleManager = ParticleManager::GetInstance();

	for (uint32_t i = 0; i < numInstance_; i++) {
		if (basic_[i].lifeTime_ <= basic_[i].currentTime_) {
			continue;
		}

		ParticleForGPU particleForGPU;
		particleForGPU.World = basic_[i].worldMatrix_;
		particleForGPU.WVP = matrix4x4Calc->Multiply(basic_[i].worldMatrix_, viewProjection.viewProjectionMatrix_);
		particleForGPU.color = basic_[i].color_;
		particleManager->SetParticleForGPUMap(particleForGPU, indexMap + i);

	}

}

void Particle::TimeElapsed(uint32_t num)
{

	basic_[num].currentTime_ += kDeltaTime_;
	if (basic_[num].currentTime_ >= basic_[num].lifeTime_) {
		basic_[num].currentTime_ = basic_[num].lifeTime_;
	}

}

void Particle::GraduallyDisappear(uint32_t num)
{

	basic_[num].color_.w = 1.0f - (basic_[num].currentTime_ / basic_[num].lifeTime_);

}

void Particle::BillBoardUpdate(const Matrix4x4& cameraMatrix4x4)
{
	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	Matrix4x4 backToFrontMatrix = matrix4x4Calc->MakeRotateYMatrix(std::numbers::pi_v<float>);
	Matrix4x4 billBoardMatrix = matrix4x4Calc->Multiply(backToFrontMatrix, cameraMatrix4x4);
	billBoardMatrix.m[3][0] = 0.0f;
	billBoardMatrix.m[3][1] = 0.0f;
	billBoardMatrix.m[3][2] = 0.0f;

}
