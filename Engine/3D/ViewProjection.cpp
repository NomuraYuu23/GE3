#include "ViewProjection.h"
#include "../base/WinApp.h"

void ViewProjection::Initialize() {

	transform_ = { { 1.0f, 1.0f, 1.0f},{ 0.0f, 0.0f, 0.0f},{ 0.0f, 0.0f, -10.0f} };

	fovY_ = 0.45f;

	aspectRatio_ = float(WinApp::kWindowWidth) / float(WinApp::kWindowHeight);

	nearClip_ = 0.1f;

	farClip_ = 1000.0f;

	// 回転行列
	rotateMatrix_ = Matrix4x4Calc::GetInstance()->MakeRotateXYZMatrix(transform_.rotate);

	// 回転行列
	rotateAngleMatrix_ = Matrix4x4Calc::GetInstance()->MakeRotateXYZMatrix(transform_.rotate);

	// 回転行列
	rotateDirectionMatrix_ = Matrix4x4Calc::GetInstance()->MakeIdentity4x4();

	// 方向ベクトルで回転行列
	//rotateMatrix_usedRotate_ = true;

	UpdateMatrix();

}

void ViewProjection::UpdateMatrix() {

	Matrix4x4Calc* calc = Matrix4x4Calc::GetInstance();

	//拡大縮小行列
	Matrix4x4 scaleMatrix = calc->MakeScaleMatrix(transform_.scale);

	// どう回転行列作るか
	// 回転行列
	rotateAngleMatrix_ = calc->MakeRotateXYZMatrix(transform_.rotate);

	rotateMatrix_ = calc->Multiply(rotateAngleMatrix_, rotateDirectionMatrix_);

	//平行移動行列
	Matrix4x4 translateMatrix = calc->MakeTranslateMatrix(transform_.translate);

	Matrix4x4 transformMatrix = calc->Multiply(scaleMatrix, calc->Multiply(rotateMatrix_, translateMatrix));
	viewMatrix_ = calc->Inverse(transformMatrix);
	projectionMatrix_ = calc->MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);

	viewProjectionMatrix_ = calc->Multiply(viewMatrix_, projectionMatrix_);

}