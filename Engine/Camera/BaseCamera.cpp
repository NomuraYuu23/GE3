#include "BaseCamera.h"

void BaseCamera::Initialize()
{

	//ビュープロジェクション
	viewProjection_.Initialize();

	// カメラ行列
	matrix_ = Matrix4x4Calc::GetInstance()->MakeAffineMatrix(viewProjection_.transform_.scale, viewProjection_.transform_.rotate, viewProjection_.transform_.translate);

}

void BaseCamera::Update()
{

	Matrix4x4Calc* calc = Matrix4x4Calc::GetInstance();

	//ビュープロジェクション
	viewProjection_.UpdateMatrix();

	//拡大縮小行列
	Matrix4x4 scaleMatrix = calc->MakeScaleMatrix(viewProjection_.transform_.scale);

	Matrix4x4 rotateMatrix = viewProjection_.rotateMatrix_;

	//平行移動行列
	Matrix4x4 translateMatrix = calc->MakeTranslateMatrix(viewProjection_.transform_.translate);

	matrix_ = calc->Multiply(scaleMatrix, calc->Multiply(rotateMatrix, translateMatrix));

}
