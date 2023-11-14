#include "Particle3D.h"

#include "../base/BufferResource.h"
#include "../base/WinApp.h"
#include "../base/DirectXCommon.h"
#include "ParticleManager.h"
#include <cassert>

Particle3D::~Particle3D()
{

	ParticleManager::GetInstance()->ParticleDelete(numInstance_, indexMap_);

}

void Particle3D::Initialize(uint32_t numInstance, uint32_t indexMap) {

	assert(numInstance > 0);

	numInstance_ = numInstance;

	indexMap_ = indexMap;

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();
	ParticleManager* particleManager = ParticleManager::GetInstance();

	for (uint32_t i = 0; i < numInstance_; i++){

		transform_[i].scale = {1.0f,1.0f,1.0f};
		transform_[i].rotate = { 0.0f,0.0f,0.0f };
		transform_[i].translate = { 0.1f * i,0.1f * i,0.0f };

		worldMatrix_[i] = matrix4x4Calc->MakeIdentity4x4();

		particleManager->SetTransformationMatrixMapWorld(matrix4x4Calc->MakeIdentity4x4(), indexMap_ + i);
		particleManager->SetTransformationMatrixMapWVP(matrix4x4Calc->MakeIdentity4x4(), indexMap_ + i);

	}

	UpdateMatrix();

}

void Particle3D::UpdateMatrix() {

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	for (uint32_t i = 0; i < numInstance_; i++) {

		worldMatrix_[i] = matrix4x4Calc->MakeAffineMatrix(transform_[i].scale, transform_[i].rotate, transform_[i].translate);

	}

}

void Particle3D::Map(const ViewProjection& viewProjection)
{

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();
	ParticleManager* particleManager = ParticleManager::GetInstance();

	for (uint32_t i = 0; i < numInstance_; i++) {
		particleManager->SetTransformationMatrixMapWorld(worldMatrix_[i], indexMap_ + i);
		particleManager->SetTransformationMatrixMapWVP(matrix4x4Calc->Multiply(worldMatrix_[i], viewProjection.viewProjectionMatrix_), indexMap_ + i);
	}

}
