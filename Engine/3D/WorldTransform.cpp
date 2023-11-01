#include "WorldTransform.h"

#include "../base/BufferResource.h"
#include "../base/WinApp.h"
#include "../base/DirectXCommon.h"

void WorldTransform::Initialize() {

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	transformationMatrixBuff_ = BufferResource::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(TransformationMatrix));
	//書き込むためのアドレスを取得
	transformationMatrixBuff_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixMap_));

	transformationMatrixMap_->World = matrix4x4Calc->MakeIdentity4x4();
	transformationMatrixMap_->WVP = matrix4x4Calc->MakeIdentity4x4();

	UpdateMatrix();

}

void WorldTransform::UpdateMatrix() {

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	worldMatrix_ = matrix4x4Calc->MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);

	if (parent_) {
		worldMatrix_ = matrix4x4Calc->Multiply(worldMatrix_, parent_->worldMatrix_);
	}

}

void WorldTransform::MapSprite()
{

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	//Sprite用のWorldViewProjectionMatrixを作る
	Matrix4x4 WorldMatrixSprite = matrix4x4Calc->MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 viewMatrixSprite = matrix4x4Calc->MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = matrix4x4Calc->MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = matrix4x4Calc->Multiply(WorldMatrixSprite, matrix4x4Calc->Multiply(viewMatrixSprite, projectionMatrixSprite));
	transformationMatrixMap_->WVP = worldViewProjectionMatrixSprite;
	transformationMatrixMap_->World = WorldMatrixSprite;

}

void WorldTransform::Map(const ViewProjection& viewProjection)
{

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	transformationMatrixMap_->World = worldMatrix_;
	transformationMatrixMap_->WVP = matrix4x4Calc->Multiply(worldMatrix_, viewProjection.viewProjectionMatrix_);

}
