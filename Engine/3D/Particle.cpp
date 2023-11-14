#include "Particle.h"
#include "../base/BufferResource.h"
#include "../base/WinApp.h"
#include "../base/DirectXCommon.h"
#include "ParticleManager.h"
#include <cassert>

Particle::~Particle(){}

void Particle::Initialize(uint32_t numInstance)
{

	assert(numInstance > 0);

	numInstance_ = numInstance;

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	transform_ = new TransformStructure[numInstance_];
	worldMatrix_ = new Matrix4x4[numInstance_];

	for (uint32_t i = 0; i < numInstance_; i++) {

		transform_[i].scale = { 1.0f,1.0f,1.0f };
		transform_[i].rotate = { 0.0f,0.0f,0.0f };
		transform_[i].translate = { 0.1f * i,0.1f * i,0.0f };

		worldMatrix_[i] = matrix4x4Calc->MakeIdentity4x4();

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

		worldMatrix_[i] = matrix4x4Calc->MakeAffineMatrix(transform_[i].scale, transform_[i].rotate, transform_[i].translate);

	}

}

void Particle::Map(const ViewProjection& viewProjection, uint32_t indexMap)
{

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();
	ParticleManager* particleManager = ParticleManager::GetInstance();

	for (uint32_t i = 0; i < numInstance_; i++) {
		particleManager->SetTransformationMatrixMapWorld(worldMatrix_[i], indexMap + i);
		particleManager->SetTransformationMatrixMapWVP(matrix4x4Calc->Multiply(worldMatrix_[i], viewProjection.viewProjectionMatrix_), indexMap + i);
	}

}
