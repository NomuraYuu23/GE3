#include "Particle.h"
#include "../base/BufferResource.h"
#include "../base/WinApp.h"
#include "../base/DirectXCommon.h"
#include "ParticleManager.h"
#include "../Random/Random.h"
#include <cassert>

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

	basic_ = new Basic[numInstance_];

	for (uint32_t i = 0; i < numInstance_; i++) {

		basic_[i].transform_.scale = {1.0f,1.0f,1.0f};
		basic_[i].transform_.rotate = { 0.0f,0.0f,0.0f };
		basic_[i].transform_.translate = { distribution(randomEngine) , distribution(randomEngine), distribution(randomEngine) };

		basic_[i].worldMatrix_ = matrix4x4Calc->MakeIdentity4x4();

		basic_[i].color_ = { distColor(randomEngine),distColor(randomEngine),distColor(randomEngine), 1.0f };

	}

	UpdateMatrix();

}

void Particle::Update()
{

	UpdateMatrix();

}

void Particle::UpdateMatrix()
{

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	for (uint32_t i = 0; i < numInstance_; i++) {

		basic_[i].worldMatrix_ = matrix4x4Calc->MakeAffineMatrix(basic_[i].transform_.scale, basic_[i].transform_.rotate, basic_[i].transform_.translate);

	}

}

void Particle::Map(const ViewProjection& viewProjection, uint32_t indexMap)
{

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();
	ParticleManager* particleManager = ParticleManager::GetInstance();

	for (uint32_t i = 0; i < numInstance_; i++) {
		particleManager->SetParticleForGPUMapWorld(basic_[i].worldMatrix_, indexMap + i);
		particleManager->SetParticleForGPUMapWVP(matrix4x4Calc->Multiply(basic_[i].worldMatrix_, viewProjection.viewProjectionMatrix_), indexMap + i);
		particleManager->SetParticleForGPUMapColor(basic_[i].color_, indexMap + i);
	}

}
