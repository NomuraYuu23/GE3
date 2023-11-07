#include "WorldTransform.h"

#include "../base/BufferResource.h"
#include "../base/WinApp.h"
#include "../base/DirectXCommon.h"

void WorldTransform::Initialize() {

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	// 回転行列
	rotateMatrix_ = matrix4x4Calc->MakeRotateXYZMatrix(transform_.rotate);

	// このフレームで直接回転行列をいれてるか
	usedRotateMatrix_ = false;

	// スケールを考えない
	parentMatrix_ = matrix4x4Calc->MakeAffineMatrix(Vector3{1.0f,1.0f,1.0f}, transform_.rotate, transform_.translate);

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

	//拡大縮小行列
	Matrix4x4 scaleMatrix = matrix4x4Calc->MakeScaleMatrix(transform_.scale);

	// 回転行列作るか
	if (!usedRotateMatrix_) {
		// 回転行列
		rotateMatrix_ = matrix4x4Calc->MakeRotateXYZMatrix(transform_.rotate);
	}

	//平行移動行列
	Matrix4x4 translateMatrix = matrix4x4Calc->MakeTranslateMatrix(transform_.translate);

	// ワールド行列
	worldMatrix_ = matrix4x4Calc->Multiply(scaleMatrix, matrix4x4Calc->Multiply(rotateMatrix_, translateMatrix));

	//拡大縮小行列
	scaleMatrix = matrix4x4Calc->MakeScaleMatrix(Vector3{ 1.0f,1.0f,1.0f });
	// 親子関係用
	parentMatrix_ = matrix4x4Calc->Multiply(scaleMatrix, matrix4x4Calc->Multiply(rotateMatrix_, translateMatrix));

	// 親子関係
	if (parent_) {
		worldMatrix_ = matrix4x4Calc->Multiply(worldMatrix_, parent_->parentMatrix_);
		parentMatrix_ = matrix4x4Calc->Multiply(parentMatrix_, parent_->parentMatrix_);
	}


	// このフレームで直接回転行列をいれてるかfalseに
	usedRotateMatrix_ = false;

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
