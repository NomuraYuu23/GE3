#include "FollowCamera.h"
#include "../../Math/Vector3.h"
#include "../../Math/Matrix4x4.h"
#include "../../Input/input.h"
#include <algorithm>
#include "../../GlobalVariables/GlobalVariables.h"
#include "../../Math/Math.h"
#include "../../Math/Ease.h"

void FollowCamera::Initialize() {

	BaseCamera::Initialize();

	//y固定
	viewProjection_.transform_.translate.y = 10.0f;
	viewProjection_.transform_.rotate.x = 0.1f;
	//viewProjection_.transform_.rotate.y = 3.14f / 2.0f;
	//destinationAngle_.y = 3.14f / 2.0f;

	BaseCamera::Update();

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";
	//グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "moveRate", moveRate_);
	globalVariables->AddItem(groupName, "rotateRate", rotateRate_);

}

void FollowCamera::Update() {

	//インスタンス
	Input* input = Input::GetInstance();
	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG

	// スティック入力で角度を変更処理
	if (input->GetJoystickConnected()) {

		const float RotateSpeed = 0.000001f;

		destinationAngle_.y += input->GetRightAnalogstick().x * RotateSpeed;
		destinationAngle_.x += input->GetRightAnalogstick().y * RotateSpeed;
		// xに制限
		float limit = 3.14f / 4.0f;
		destinationAngle_.x = std::clamp(destinationAngle_.x, 0.0f, limit);
	}

	//追従対象がいれば
	if (target_) {
		// 追従座標の補間
		Vector3 targetPos = { target_->worldMatrix_.m[3][0], target_->worldMatrix_.m[3][1], target_->worldMatrix_.m[3][2] };
		interTarget_ = Ease::Easing(Ease::EaseName::Lerp, interTarget_, targetPos, moveRate_);

		// オフセット
		Vector3 offset = OffsetCalc();

		viewProjection_.transform_.translate = v3Calc->Add(interTarget_, offset);

	}

	//y固定
	//viewProjection_.transform_.translate.y = 10.0f;

	viewProjection_.transform_.rotate.y = Math::LerpShortAngle(viewProjection_.transform_.rotate.y, destinationAngle_.y, rotateRate_);
	viewProjection_.transform_.rotate.x = Math::LerpShortAngle(viewProjection_.transform_.rotate.x, destinationAngle_.x, rotateRate_);

	//ビュー更新
	BaseCamera::Update();
}


Vector3 FollowCamera::OffsetCalc() const
{

	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	//追従対象からカメラまでのオフセット
	Vector3 offset = { 0.0f, 5.0f, -50.0f };

	Matrix4x4 rotateMatrix;

	//カメラの角度から回転行列を計算する
	Matrix4x4 rotateMatrixX = m4Calc->MakeRotateXMatrix(viewProjection_.transform_.rotate.x);
	Matrix4x4 rotateMatrixY = m4Calc->MakeRotateYMatrix(viewProjection_.transform_.rotate.y);
	Matrix4x4 rotateMatrixZ = m4Calc->MakeRotateZMatrix(viewProjection_.transform_.rotate.z);

	rotateMatrix = m4Calc->Multiply(
	rotateMatrixX, m4Calc->Multiply(rotateMatrixY, rotateMatrixZ));


	//オフセットをカメラの回転に合わせて回転させる
	offset = m4Calc->TransformNormal(offset, rotateMatrix);

	return offset;

}

void FollowCamera::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "FollowCamera";

	moveRate_ = globalVariables->GetFloatValue(groupName, "moveRate");
	rotateRate_ = globalVariables->GetFloatValue(groupName, "rotateRate");

}
