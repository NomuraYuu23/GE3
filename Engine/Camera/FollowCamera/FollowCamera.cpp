#include "FollowCamera.h"
#include "../../Math/Vector3.h"
#include "../../Math/Matrix4x4.h"
#include "../../Input/input.h"

void FollowCamera::Initialize() {

	viewProjection_.Initialize();
	//y固定
	viewProjection_.transform_.translate.y = 10.0f;
	viewProjection_.transform_.rotate.x = 0.1f;

	//ビュー更新
	viewProjection_.UpdateMatrix();

}

void FollowCamera::Update() {

	//インスタンス
	Input* input = Input::GetInstance();
	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	if (input->GetJoystickConnected()) {

		const float RotateSpeed = 0.000001f;

		viewProjection_.transform_.rotate.y += input->GetRightAnalogstick().x * RotateSpeed;
		viewProjection_.transform_.rotate.x += input->GetRightAnalogstick().y * RotateSpeed;
	}

	//追従対象がいれば
	if (target_) {
		//追従対象からカメラまでのオフセット
		Vector3 offset = { 0.0f, 5.0f, -50.0f };

		//カメラの角度から回転行列を計算する
		Matrix4x4 rotateMatrixX = m4Calc->MakeRotateXMatrix(viewProjection_.transform_.rotate.x);
		Matrix4x4 rotateMatrixY = m4Calc->MakeRotateYMatrix(viewProjection_.transform_.rotate.y);
		Matrix4x4 rotateMatrixZ = m4Calc->MakeRotateZMatrix(viewProjection_.transform_.rotate.z);

		Matrix4x4 rotateMatrix = m4Calc->Multiply(
			rotateMatrixX, m4Calc->Multiply(rotateMatrixY, rotateMatrixZ));

		//オフセットをカメラの回転に合わせて回転させる
		offset = m4Calc->TransformNormal(offset, rotateMatrix);

		//座標をコピーしてオフセット分ずらす
		Vector3 targetPos = { target_->worldMatrix_.m[3][0], target_->worldMatrix_.m[3][1], target_->worldMatrix_.m[3][2] };
		viewProjection_.transform_.translate = v3Calc->Add(targetPos, offset);

	}

	//y固定
	//viewProjection_.transform_.translate.y = 10.0f;

	//ビュー更新
	viewProjection_.UpdateMatrix();

}
