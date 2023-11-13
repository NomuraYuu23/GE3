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

	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	transformationMatrixBuff_ = BufferResource::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(TransformationMatrix) * numInstance_);
	//書き込むためのアドレスを取得
	transformationMatrixBuff_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixMap_));

	for (size_t i = 0; i < numInstance_; i++){

		// 回転行列
		data_[i].rotateMatrix_ = matrix4x4Calc->MakeRotateXYZMatrix(data_[i].transform_.rotate);

		// 方向ベクトルで回転行列
		data_[i].usedDirection_ = false;

		// スケールを考えない
		data_[i].parentMatrix_ = matrix4x4Calc->MakeAffineMatrix(Vector3{ 1.0f,1.0f,1.0f }, data_[i].transform_.rotate, data_[i].transform_.translate);

		transformationMatrixMap_[i].World = matrix4x4Calc->MakeIdentity4x4();
		transformationMatrixMap_[i].WVP = matrix4x4Calc->MakeIdentity4x4();

	}

	UpdateMatrix();

}

void Particle3D::UpdateMatrix() {

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	for (size_t i = 0; i < numInstance_; i++) {

		//拡大縮小行列
		Matrix4x4 scaleMatrix = matrix4x4Calc->MakeScaleMatrix(data_[i].transform_.scale);

		// どう回転行列作るか
		if (data_[i].usedDirection_) {
			// 回転行列
			data_[i].rotateMatrix_ = matrix4x4Calc->DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, data_[i].direction_);
		}
		else {
			// 回転行列
			data_[i].rotateMatrix_ = matrix4x4Calc->MakeRotateXYZMatrix(data_[i].transform_.rotate);
		}

		//平行移動行列
		Matrix4x4 translateMatrix = matrix4x4Calc->MakeTranslateMatrix(data_[i].transform_.translate);

		// ワールド行列
		data_[i].worldMatrix_ = matrix4x4Calc->Multiply(scaleMatrix, matrix4x4Calc->Multiply(data_[i].rotateMatrix_, translateMatrix));

		//拡大縮小行列
		scaleMatrix = matrix4x4Calc->MakeScaleMatrix(Vector3{ 1.0f,1.0f,1.0f });
		// 親子関係用
		data_[i].parentMatrix_ = matrix4x4Calc->Multiply(scaleMatrix, matrix4x4Calc->Multiply(data_[i].rotateMatrix_, translateMatrix));

		//// 親子関係
		//if (parent_) {
		//	worldMatrix_ = matrix4x4Calc->Multiply(worldMatrix_, parent_->parentMatrix_);
		//	parentMatrix_ = matrix4x4Calc->Multiply(parentMatrix_, parent_->parentMatrix_);
		//}

	}

}

void Particle3D::Map(const ViewProjection& viewProjection)
{

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	for (size_t i = 0; i < numInstance_; i++) {

		transformationMatrixMap_[i].World = data_[i].worldMatrix_;
		transformationMatrixMap_[i].WVP = matrix4x4Calc->Multiply(data_[i].worldMatrix_, viewProjection.viewProjectionMatrix_);

	}

}
