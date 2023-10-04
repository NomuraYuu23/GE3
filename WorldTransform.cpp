#include "WorldTransform.h"
#include "DirectXCommon.h"
#include "BufferResource.h"

void WorldTransform::Initialize(const ViewProjection* viewProjection) {

	DirectXCommon* dxCommon = DirectXCommon::GetInstance();
	Matrix4x4Calc* calc = Matrix4x4Calc::GetInstance();

	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	transformationMatrixBuff_ = BufferResource::CreateBufferResource(dxCommon->GetDevice(), sizeof(TransformationMatrix));
	//書き込むためのアドレスを取得
	transformationMatrixBuff_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixMap_));
	//単位行列を書き込んでおく
	transformationMatrixMap_->World = calc->MakeIdentity4x4();
	transformationMatrixMap_->WVP = calc->MakeIdentity4x4();

	viewProjection_ = viewProjection;

	UpdateMatrix();

}

void WorldTransform::UpdateMatrix() {

	Matrix4x4Calc* calc = Matrix4x4Calc::GetInstance();

	worldMatrix_ = calc->MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

	if (parent_) {
		worldMatrix_ = calc->Multiply(worldMatrix_, parent_->worldMatrix_);
	}

	Matrix4x4 worldViewProjectionMatrix = calc->Multiply(worldMatrix_, viewProjection_->viewProjectionMatrix_);
	transformationMatrixMap_->WVP = worldViewProjectionMatrix;
	transformationMatrixMap_->World = worldMatrix_;

}