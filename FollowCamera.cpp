#include "FollowCamera.h"
#include "Vector3.h"
#include "Matrix4x4.h"
#include "input.h"

void FollowCamera::Initialize() {

	viewProjection_.Initialize();

}

void FollowCamera::Update() {

	//インスタンス
	Input* input = Input::GetInstance();
	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	//追従対象がいれば
	if (target_) {
		//追従対象からカメラまでのオフセット
		Vector3 offset = { 0.0f, 5.0f, -30.0f };

		//カメラの角度から回転行列を計算する
		Matrix4x4 rotateMatrixX = m4Calc->MakeRotateXMatrix(viewProjection_.transform_.rotate.x);
		Matrix4x4 rotateMatrixY = m4Calc->MakeRotateYMatrix(viewProjection_.transform_.rotate.y);
		Matrix4x4 rotateMatrixZ = m4Calc->MakeRotateZMatrix(viewProjection_.transform_.rotate.z);

		Matrix4x4 rotateMatrix = m4Calc->Multiply(
			rotateMatrixX, m4Calc->Multiply(rotateMatrixY, rotateMatrixZ));

		//オフセットをカメラの回転に合わせて回転させる
		offset = m4Calc->TransformNormal(offset, rotateMatrix);

		//座標をコピーしてオフセット分ずらす
		viewProjection_.transform_.translate = v3Calc->Add(target_->transform_.translate, offset);

	}

	if (input->GetJoystickConnected()) {

		const float RotateSpeed = 0.000001f;

		viewProjection_.transform_.rotate.y += input->GetRightAnalogstick().x * RotateSpeed;

	}

	//ビュー更新
	viewProjection_.UpdateMatrix();

}
