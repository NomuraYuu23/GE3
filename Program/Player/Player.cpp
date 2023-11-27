#include "../Player/Player.h"
#include "../../Engine/Input/Input.h"
#include "../../Engine/Math/Math.h"
#include "../../Engine/Math/Ease.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include "../LockOn/LockOn.h"

// コンボ定数表
const std::array<Player::ConstAttack, Player::kComboNum> Player::kConstAttaks = {
	{
		{ { 20, 20, 20, 20}, { 0.0f, 0.0f, 0.2f, 0.0f} },
		{ { 10, 10, 10, 10}, { 0.2f, 0.0f, 0.0f, 0.0f} },
		{ { 10, 10, 60, 30}, { 0.0f, 0.0f, 0.2f, 0.0f} },
	}
};


void Player::Initialize(const std::vector<Model*>& models,
	const std::vector<Material*>& materials)
{

	//nullポインタチェック
	assert(models.front());
	//基底クラスの初期化
	BaseCharacter::Initialize(models, materials);
	worldTransform_.transform_.translate = workRoot_.kInitialPosition;
	worldTransform_.UpdateMatrix();

	//nullポインタチェック
	assert(models.front());

	//基底クラスの初期化
	BaseCharacter::Initialize(models, materials);

	worldTransformBody_.Initialize();
	worldTransformBody_.parent_ = &worldTransform_;
	worldTransformHead_.Initialize();
	worldTransformHead_.transform_.translate.y += 3.5f;
	worldTransformHead_.parent_ = &worldTransformBody_;
	worldTransformL_arm_.Initialize();
	worldTransformL_arm_.transform_.translate.y += 2.5f;
	worldTransformL_arm_.transform_.translate.x -= 1.0f;
	worldTransformL_arm_.parent_ = &worldTransformBody_;
	worldTransformR_arm_.Initialize();
	worldTransformR_arm_.transform_.translate.y += 2.5f;
	worldTransformR_arm_.transform_.translate.x += 1.0f;
	worldTransformR_arm_.parent_ = &worldTransformBody_;

	worldTransformWeapon_.Initialize();
	worldTransformWeapon_.transform_.translate.y += 3.0f;
	worldTransformWeapon_.parent_ = &worldTransform_;

	//浮遊ギミック
	InitializeFloatinggimmick();

	//ぶらぶらギミック
	InitializeSwinggimmick();

	velocity_ = {0.0f,0.0f,0.0f};

	isLanding = true;

	collider_ = std::make_unique<Sphere>();
	collider_->Initialize(worldTransform_.transform_.translate, workRoot_.kColliderSize);

	// 攻撃
	//
	workAttack_.attackCenterAdd_ = {0.0f,0.0,10.0f};

	workAttack_.attackRadius_ = 5.0f;

	// コライダー
	workAttack_.attackCollider_ = std::make_unique<Sphere>();
	workAttack_.attackCollider_->Initialize(workAttack_.attackCenterAdd_, workAttack_.attackRadius_);

	// あたり判定を取るか
	workAttack_.isAttackJudgment_ = false;

	// グローバル
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "dashPeriod", static_cast<int>(workDash_.period_));

}

void Player::Update()
{

#ifdef _DEBUG
	ApplyGlobalVariables();
#endif // _DEBUG

	worldTransform_.usedDirection_ = true;

	if (behaviorRequest_) {
		//振るまいを変更する
		behavior_ = behaviorRequest_.value();
		//各振るまいごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		case Behavior::kDash:
			BehaviorDashInitialize();
			break;
		}
		//振るまいリクエストをリセット
		behaviorRequest_ = std::nullopt;
	}

	// 振るまい
	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	case Behavior::kDash:
		BehaviorDashUpdate();
		break;
	}

	//行列を定数バッファに転送
	allUpdateMatrix();

	if (worldTransform_.worldMatrix_.m[3][1] <= -10.0f) {
		isDead_ = true;
	}

	collider_->center_ = { worldTransform_.worldMatrix_.m[3][0], worldTransform_.worldMatrix_.m[3][1] + collider_->radius_, worldTransform_.worldMatrix_.m[3][2] };
	collider_->worldTransformUpdate();

}

void Player::Draw(const ViewProjection& viewProjection)
{

	models_[int(ModelIndex::kModelIndexBody)]->Draw(worldTransformBody_, viewProjection);
	models_[int(ModelIndex::kModelIndexHead)]->Draw(worldTransformHead_, viewProjection);
	models_[int(ModelIndex::kModelIndexL_arm)]->Draw(worldTransformL_arm_, viewProjection);
	models_[int(ModelIndex::kModelIndexR_arm)]->Draw(worldTransformR_arm_, viewProjection);
	if (behavior_ == Behavior::kAttack && workAttack_.inComboPhase_ < kRecovery) {
		models_[int(ModelIndex::kModelIndexWeapon)]->Draw(worldTransformWeapon_, viewProjection);
	}

}

void Player::BehaviorRootInitialize()
{

	// 浮遊ギミック
	InitializeFloatinggimmick();

	// ぶらぶらギミック
	InitializeSwinggimmick();

	//武器角度
	worldTransformWeapon_.transform_.rotate = {0.0f,0.0f,0.0f};

}

void Player::BehaviorRootUpdate()
{

	Walk();
	Jump();
	Fall();
	Move();
	//行列を定数バッファに転送
	worldTransform_.UpdateMatrix();

	Landing();

	AttackStart();

	DashStart();

	// 浮遊ギミック
	UpdateFloatinggimmick();

	// ぶらぶらギミック
	UpdateSwinggimmick();


}

void Player::BehaviorAttackInitialize()
{

	// 攻撃行動用の媒介変数
	workAttack_.behaviorAttackParameter_ = 0.0f;
	workAttack_.attackCollider_->center_ = { -100.0f,-100.0f,-100.0f };
	workAttack_.attackCollider_->worldTransformUpdate();

	workAttack_.behaviorAttackPeriod_ =	kConstAttaks[workAttack_.comboIndex_].time_[workAttack_.inComboPhase_];

}

void Player::BehaviorAttackUpdate()
{

	// 1フレームでのパラメータ加算値
	const float step = 1.0f / static_cast<float>(workAttack_.behaviorAttackPeriod_);

	// パラメータを1ステップ分加算
	workAttack_.behaviorAttackParameter_ += step;

	if (workAttack_.comboIndex_ == 0) {
		AttackCombo1st();
	}
	else if (workAttack_.comboIndex_ == 1){
		AttackCombo2nd();
	}
	else {
		AttackCombo3rd();
	}

	AttackComboContinuationJudgment();
	AttackComboPhaseFinished();


}

void Player::BehaviorDashInitialize()
{

	workDash_.deshPrameter_ = 0;
	worldTransform_.direction_ = workRoot_.targetDirection_;

}

void Player::BehaviorDashUpdate()
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	Vector3 move = { 0.0f,0.0f,1.0f };

	// 移動量に速さを反映
	move = v3Calc->Multiply(workDash_.spped_, v3Calc->Normalize(move));

	// 移動ベクトルをカメラの角度だけ回転する
	move = m4Calc->TransformNormal(move, worldTransform_.rotateMatrix_);

	// 移動
	velocity_.x = move.x;
	velocity_.z = move.z;

	worldTransform_.transform_.translate = v3Calc->Add(worldTransform_.transform_.translate, velocity_);

	if (++workDash_.deshPrameter_ >= workDash_.period_) {
		behaviorRequest_ = Behavior::kRoot;
	}

}

void Player::InitializeFloatinggimmick()
{

	// 浮遊ギミックの媒介変数
	workFloating_.floatingParameter_ = 0.0f;
	// 浮遊移動のサイクル<frame>
	workFloating_.floatingPeriod_ = 60;
	// 浮遊の振幅<m>
	workFloating_.floatingAmplitude_ = 0.5f;

}

void Player::UpdateFloatinggimmick()
{

	//π
	const float pi = 3.14f;

	// 1フレームでのパラメータ加算値
	const float step = 2.0f * pi / workFloating_.floatingPeriod_;

	// パラメータを1ステップ分加算
	workFloating_.floatingParameter_ += step;
	//2πを超えたら0に戻す
	workFloating_.floatingParameter_ = std::fmod(workFloating_.floatingParameter_, 2.0f * pi);


	//浮遊を座標に反映
	worldTransformBody_.transform_.translate.y = std::sin(workFloating_.floatingParameter_) * workFloating_.floatingAmplitude_ + 1.0f;

}

void Player::InitializeSwinggimmick()
{

	// ぶらぶらアニメーションの媒介変数
	workSwing_.swingParameter_ = 0.0f;
	// ぶらぶらアニメーションのサイクル<frame>
	workSwing_.swingPeriod_ = 60;

}

void Player::UpdateSwinggimmick()
{

	//π
	const float pi = 3.14f;

	// 1フレームでのパラメータ加算値
	const float step = 2.0f * pi / workSwing_.swingPeriod_;
	// パラメータを1ステップ分加算
	workSwing_.swingParameter_ += step;
	// 2πを超えたら0に戻す
	workSwing_.swingParameter_ = std::fmod(workSwing_.swingParameter_, 2.0f * pi);

	worldTransformL_arm_.transform_.rotate.x = std::sinf(workSwing_.swingParameter_) / 2.0f;
	worldTransformR_arm_.transform_.rotate.x = std::sinf(workSwing_.swingParameter_) / 2.0f;


}

void Player::Move()
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	worldTransform_.transform_.translate = v3Calc->Add(worldTransform_.transform_.translate, velocity_);

}

void Player::Walk()
{

	//インスタンス
	Input* input = Input::GetInstance();
	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	//移動
	if (input->GetJoystickConnected()) {

		const float kThreshold = 0.7f;
		bool isMoving = false;

		// 移動量
		Vector3 move = { input->GetLeftAnalogstick().x, 0.0f, -input->GetLeftAnalogstick().y };
		if (v3Calc->Length(move) > kThreshold) {
			isMoving = true;
		}

		if (isMoving) {

			// 移動量に速さを反映
			move = v3Calc->Multiply(workRoot_.kWalkSpeed, v3Calc->Normalize(move));

			// カメラの角度から回転行列を計算する
			Matrix4x4 rotateMatrixX = m4Calc->MakeRotateXMatrix(viewProjection_->transform_.rotate.x);
			Matrix4x4 rotateMatrixY = m4Calc->MakeRotateYMatrix(viewProjection_->transform_.rotate.y);
			Matrix4x4 rotateMatrixZ = m4Calc->MakeRotateZMatrix(viewProjection_->transform_.rotate.z);

			Matrix4x4 rotateMatrix = m4Calc->Multiply(
				rotateMatrixX, m4Calc->Multiply(rotateMatrixY, rotateMatrixZ));

			// 移動ベクトルをカメラの角度だけ回転する
			move = m4Calc->TransformNormal(move, rotateMatrix);

			//　親がいれば
			if (worldTransform_.parent_) {
				rotateMatrix = m4Calc->Inverse(worldTransform_.parent_->rotateMatrix_);
				move = m4Calc->TransformNormal(move, rotateMatrix);
			}

			// 移動
			velocity_.x = move.x;
			velocity_.z = move.z;

			// 移動方向に見た目を合わせる(Y軸)
			// あたん
			workRoot_.targetDirection_.x = v3Calc->Normalize(move).x;
			workRoot_.targetDirection_.z = v3Calc->Normalize(move).z;

		}
		else if (lockOn_ && lockOn_->ExistTarget()) {
			// 自キャラをロックオン対象の方向に向ける処理
			// ロックオン座標
			Vector3 lockOn = lockOn_->GetTargetPosition();
			// ワールド座標
			Vector3 worldPosition = { worldTransform_.worldMatrix_.m[3][0],worldTransform_.worldMatrix_.m[3][1] ,worldTransform_.worldMatrix_.m[3][2] };

			// 追従対象からロックオン対象へのベクトル
			Vector3 sub = v3Calc->Subtract(lockOn, worldPosition);

			workRoot_.targetDirection_.x = v3Calc->Normalize(sub).x;
			workRoot_.targetDirection_.z = v3Calc->Normalize(sub).z;

			// 移動
			velocity_.x = 0.0f;
			velocity_.z = 0.0f;

		}
		else {
			// 移動
			velocity_.x = 0.0f;
			velocity_.z = 0.0f;
		}

		// 角度補間
		worldTransform_.direction_ = Ease::Easing(Ease::EaseName::Lerp,worldTransform_.direction_, workRoot_.targetDirection_, workRoot_.targetAngleT_);
	}

}

void Player::Jump()
{

	//インスタンス
	Input* input = Input::GetInstance();

	//移動
	if (input->GetJoystickConnected()) {
		
		if (Input::GetInstance()->TriggerJoystick(0) && isLanding) {
			velocity_.y += workRoot_.kJumpSpeed;
			isLanding = false;
		}

	}

}

void Player::Fall()
{

	if (!isLanding) {
		velocity_.y -= workRoot_.kFallAcceleration;
	}

}

void Player::Landing()
{

	if (!isLanding) {
		if (worldTransform_.parent_) {
			LostParent();
		}
	}
	else {
		velocity_.y = 0.0f;
	}
	isLanding = false;
}

void Player::AttackStart()
{

	//攻撃
	if (Input::GetInstance()->TriggerJoystick(1)) {
		behaviorRequest_ = Behavior::kAttack;
	}

}

void Player::DashStart()
{

	//ダッシュ
	if (Input::GetInstance()->TriggerJoystick(2)) {
		behaviorRequest_ = Behavior::kDash;
	}

}

void Player::Restart()
{

	worldTransform_.transform_.translate = workRoot_.kInitialPosition;
	worldTransform_.direction_= workRoot_.kInitialDirection;
	worldTransform_.parent_ = nullptr;
	worldTransform_.UpdateMatrix();

	isDead_ = false;

}

void Player::OnCollision(WorldTransform* worldTransform)
{
	if (velocity_.y <= 0.0f) {
		if (!worldTransform_.parent_ ||
			(worldTransform_.parent_ != worldTransform)) {
			GotParent(worldTransform);
		}
		worldTransform_.transform_.translate.y = 0;
		allUpdateMatrix();

		isLanding = true;
	}

}

void Player::GotParent(WorldTransform* parent)
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	if (worldTransform_.parent_) {
		LostParent();
	}

	//位置
	Vector3 position = { worldTransform_.worldMatrix_.m[3][0] - parent->worldMatrix_.m[3][0],
						worldTransform_.worldMatrix_.m[3][1] - parent->worldMatrix_.m[3][1],
						worldTransform_.worldMatrix_.m[3][2] - parent->worldMatrix_.m[3][2] };

	Matrix4x4 rotateMatrix = m4Calc->Inverse(parent->rotateMatrix_);

	// 位置を親の角度だけ回転する
	position = m4Calc->TransformNormal(position, rotateMatrix);

	worldTransform_.transform_.translate = position;
	worldTransform_.parent_ = parent;

	Vector3 direction = m4Calc->TransformNormal(worldTransform_.direction_, rotateMatrix);

	// 移動方向に見た目を合わせる(Y軸)
	// あたん
	workRoot_.targetDirection_.x = v3Calc->Normalize(direction).x;
	workRoot_.targetDirection_.z = v3Calc->Normalize(direction).z;
	worldTransform_.direction_.x = v3Calc->Normalize(direction).x;
	worldTransform_.direction_.z = v3Calc->Normalize(direction).z;

	worldTransform_.UpdateMatrix();

}

void Player::LostParent()
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	Vector3 position = { worldTransform_.worldMatrix_.m[3][0] ,worldTransform_.worldMatrix_.m[3][1] ,worldTransform_.worldMatrix_.m[3][2] };
	
	worldTransform_.transform_.translate = position;

	Matrix4x4 rotateMatrix = worldTransform_.parent_->rotateMatrix_;
	Vector3 direction = m4Calc->TransformNormal(worldTransform_.direction_, rotateMatrix);

	// 移動方向に見た目を合わせる(Y軸)
	// あたん
	workRoot_.targetDirection_.x = v3Calc->Normalize(direction).x;
	workRoot_.targetDirection_.z = v3Calc->Normalize(direction).z;
	worldTransform_.direction_.x = v3Calc->Normalize(direction).x;
	worldTransform_.direction_.z = v3Calc->Normalize(direction).z;

	worldTransform_.parent_ = nullptr;

	worldTransform_.UpdateMatrix();

}

void Player::AttackComboContinuationJudgment()
{

	Input* input = Input::GetInstance();

	if (workAttack_.comboIndex_ < kComboNum - 1) {
		if (input->TriggerJoystick(1)) {
			workAttack_.comboNext_ = true;
		}
	}

}

void Player::AttackComboPhaseFinished()
{

	if (workAttack_.behaviorAttackParameter_ >= 1.0f) {
		workAttack_.inComboPhase_++;
		BehaviorAttackInitialize();
		if (workAttack_.inComboPhase_ == static_cast<uint32_t>(ComboPhase::kCountOfComboPhase)) {
			AttackComboFinished();
			workAttack_.isAttackJudgment_ = false;
			workAttack_.attackCollider_->center_ = { -2048.0f, -2048.0f, -2048.0f };
			workAttack_.attackCollider_->worldTransformUpdate();
		}
		else if (workAttack_.inComboPhase_ >= static_cast<uint32_t>(ComboPhase::kSwing)) {
			workAttack_.isAttackJudgment_ = true;
		}
		else {
			workAttack_.isAttackJudgment_ = false;
			workAttack_.attackCollider_->center_ = { -2048.0f, -2048.0f, -2048.0f };
			workAttack_.attackCollider_->worldTransformUpdate();
		}
	}

}

void Player::AttackComboFinished()
{

	workAttack_.inComboPhase_ = 0;
	// コンボ継続か
	if (workAttack_.comboNext_) {
		//フラグリセット
		workAttack_.comboNext_ = false;
		workAttack_.comboIndex_++;
		BehaviorAttackInitialize();
	}
	else {
		workAttack_.comboIndex_ = 0;
		behaviorRequest_ = Behavior::kRoot;
	}

}

void Player::AttackCombo1st()
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	//π
	const float pi = 3.14f;

	ComboPhase inComboPhase = static_cast<ComboPhase>(workAttack_.inComboPhase_);
	// スタート
	Vector3 startRotate = { 0.0f, 0.0f, 0.0f };
	// エンド
	Vector3 endRotate = { 0.0F, 0.0f, 0.0f };

	// 武器
	Vector3 startWeaponRotate = { 0.0f, 0.0f, 0.0f };
	// エンド
	Vector3 endWeaponRotate = { 0.0F, 0.0f, 0.0f };

	switch (inComboPhase)
	{
	case Player::kAnticipation:
		// 振りかぶり

		startRotate = { 0.0f, 0.0f, 0.0f };
		endRotate = { -pi - pi / 8.0f, 0.0f, 0.0f };
		worldTransformL_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformR_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		startWeaponRotate = { pi, 0.0f, 0.0f };
		endWeaponRotate = { -pi - pi / 8.0f + pi, 0.0f, 0.0f };
		worldTransformWeapon_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startWeaponRotate, endWeaponRotate, workAttack_.behaviorAttackParameter_);

		break;
	case Player::kCharge:
		break;
	case Player::kSwing:

		startRotate = { -pi - pi / 8.0f, 0.0f, 0.0f };
		endRotate = { -pi / 2.0f, 0.0f, 0.0f };
		worldTransformL_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformR_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		startWeaponRotate = { -pi - pi / 8.0f + pi, 0.0f, 0.0f };
		endWeaponRotate = { -pi / 2.0f + pi, 0.0f, 0.0f };
		worldTransformWeapon_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startWeaponRotate, endWeaponRotate, workAttack_.behaviorAttackParameter_);

		break;
	case Player::kRecovery:

		startRotate = { -pi / 2.0f, 0.0f, 0.0f };
		endRotate = { 0.0f, 0.0f, 0.0f };
		worldTransformL_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseInQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformR_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseInQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		startWeaponRotate = { -pi / 2.0f, 0.0f, 0.0f };
		endWeaponRotate = { pi, 0.0f, 0.0f };
		worldTransformWeapon_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startWeaponRotate, endWeaponRotate, workAttack_.behaviorAttackParameter_);

		break;
	case Player::kCountOfComboPhase:
	default:
		assert(0);
		break;
	}

	// 移動量
	Vector3 move = { 0.0f, 0.0f, 1.0f };
	// 移動量に速さを反映
	move = v3Calc->Multiply(kConstAttaks[workAttack_.comboIndex_].speed_[workAttack_.inComboPhase_], v3Calc->Normalize(move));

	// 移動ベクトルをカメラの角度だけ回転する
	move = m4Calc->TransformNormal(move, worldTransform_.rotateMatrix_);

	worldTransform_.transform_.translate.x += move.x;
	worldTransform_.transform_.translate.z += move.z;

	// 移動方向に見た目を合わせる(Y軸)
	if (std::fabsf(move.x) > 0.1 || std::fabsf(move.z) > 0.1) {
		// あたん
		worldTransform_.direction_.x = v3Calc->Normalize(move).x;
		worldTransform_.direction_.z = v3Calc->Normalize(move).z;
	}

	// あたり判定
	if (workAttack_.isAttackJudgment_) {
		Vector3 attackCenterAdd = m4Calc->TransformNormal(workAttack_.attackCenterAdd_, worldTransform_.worldMatrix_);

		workAttack_.attackCollider_->center_ = { worldTransform_.worldMatrix_.m[3][0] + attackCenterAdd.x,
		worldTransform_.worldMatrix_.m[3][1] + attackCenterAdd.y,
		worldTransform_.worldMatrix_.m[3][2] + attackCenterAdd.z };
		workAttack_.attackCollider_->worldTransformUpdate();
	}

}

void Player::AttackCombo2nd()
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	//π
	const float pi = 3.14f;

	ComboPhase inComboPhase = static_cast<ComboPhase>(workAttack_.inComboPhase_);
	// スタート
	Vector3 startRotate = { 0.0f, 0.0f, 0.0f };
	// エンド
	Vector3 endRotate = { 0.0F, 0.0f, 0.0f };

	// 武器
	Vector3 startWeaponRotate = { 0.0f, 0.0f, 0.0f };
	// エンド
	Vector3 endWeaponRotate = { 0.0F, 0.0f, 0.0f };

	switch (inComboPhase)
	{
	case Player::kAnticipation:
		// 振りかぶり

		startRotate = { 0.0f, 0.0f, 0.0f };
		endRotate = { -pi / 2.0f,  -pi / 2.0f, 0.0f };
		worldTransformL_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformR_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformBody_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformBody_.transform_.rotate.x = 0.0f;

		startWeaponRotate = { pi, 0.0f, 0.0f };
		endWeaponRotate = { -pi / 2.0f + pi,  -pi / 2.0f, 0.0f };
		worldTransformWeapon_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startWeaponRotate, endWeaponRotate, workAttack_.behaviorAttackParameter_);

		break;
	case Player::kCharge:
		break;
	case Player::kSwing:

		startRotate = { -pi / 2.0f,  -pi / 2.0f, 0.0f };
		endRotate = { -pi / 2.0f,  pi / 2.0f, 0.0f };
		worldTransformL_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformR_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformBody_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformBody_.transform_.rotate.x = 0.0f;

		startWeaponRotate = { -pi / 2.0f + pi,  -pi / 2.0f, 0.0f };
		endWeaponRotate = { -pi / 2.0f + pi,  pi / 2.0f, 0.0f };
		worldTransformWeapon_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startWeaponRotate, endWeaponRotate, workAttack_.behaviorAttackParameter_);

		break;
	case Player::kRecovery:

		startRotate = { -pi / 2.0f,  pi / 2.0f, 0.0f };
		endRotate = { 0.0f, 0.0f, 0.0f };
		worldTransformL_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseInQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformR_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseInQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformBody_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformBody_.transform_.rotate.x = 0.0f;

		startWeaponRotate = { -pi / 2.0f + pi,  pi / 2.0f, 0.0f };
		endWeaponRotate = { pi, 0.0f, 0.0f };
		worldTransformWeapon_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startWeaponRotate, endWeaponRotate, workAttack_.behaviorAttackParameter_);

		break;
	case Player::kCountOfComboPhase:
	default:
		assert(0);
		break;
	}

	// 移動量
	Vector3 move = { 0.0f, 0.0f, 1.0f };
	// 移動量に速さを反映
	move = v3Calc->Multiply(kConstAttaks[workAttack_.comboIndex_].speed_[workAttack_.inComboPhase_], v3Calc->Normalize(move));

	// 移動ベクトルをカメラの角度だけ回転する
	move = m4Calc->TransformNormal(move, worldTransform_.rotateMatrix_);

	worldTransform_.transform_.translate.x += move.x;
	worldTransform_.transform_.translate.z += move.z;

	// 移動方向に見た目を合わせる(Y軸)
	if (std::fabsf(move.x) > 0.1 || std::fabsf(move.z) > 0.1) {
		// あたん
		worldTransform_.direction_.x = v3Calc->Normalize(move).x;
		worldTransform_.direction_.z = v3Calc->Normalize(move).z;
	}

	// あたり判定
	if (workAttack_.isAttackJudgment_) {
		Vector3 attackCenterAdd = m4Calc->TransformNormal(workAttack_.attackCenterAdd_, worldTransform_.worldMatrix_);

		workAttack_.attackCollider_->center_ = { worldTransform_.worldMatrix_.m[3][0] + attackCenterAdd.x,
		worldTransform_.worldMatrix_.m[3][1] + attackCenterAdd.y,
		worldTransform_.worldMatrix_.m[3][2] + attackCenterAdd.z };
		workAttack_.attackCollider_->worldTransformUpdate();
	}

}

void Player::AttackCombo3rd()
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	//π
	const float pi = 3.14f;

	ComboPhase inComboPhase = static_cast<ComboPhase>(workAttack_.inComboPhase_);
	// スタート
	Vector3 startRotate = { 0.0f, 0.0f, 0.0f };
	// エンド
	Vector3 endRotate = { 0.0F, 0.0f, 0.0f };
	// 武器
	Vector3 startWeaponRotate = { 0.0f, 0.0f, 0.0f };
	// エンド
	Vector3 endWeaponRotate = { 0.0F, 0.0f, 0.0f };

	switch (inComboPhase)
	{
	case Player::kAnticipation:
		// 振りかぶり

		startRotate = { 0.0f, 0.0f, 0.0f };
		endRotate = { -pi / 2.0f,  0.0f, 0.0f };
		worldTransformL_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformR_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformBody_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformBody_.transform_.rotate.x = 0.0f;

		startWeaponRotate = { pi,  0.0f, 0.0f };
		endWeaponRotate = { -pi / 2.0f + pi, 0.0f, 0.0f };
		worldTransformWeapon_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startWeaponRotate, endWeaponRotate, workAttack_.behaviorAttackParameter_);

		break;
	case Player::kCharge:
		break;
	case Player::kSwing:

		startRotate = { -pi / 2.0f, 0.0f, 0.0f };
		endRotate = { -pi / 2.0f,  pi * 6.0f, 0.0f };
		worldTransformL_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformR_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformBody_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformBody_.transform_.rotate.x = 0.0f;

		startWeaponRotate = { -pi / 2.0f + pi,  0.0f, 0.0f };
		endWeaponRotate = { -pi / 2.0f + pi, pi * 6.0f, 0.0f };
		worldTransformWeapon_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startWeaponRotate, endWeaponRotate, workAttack_.behaviorAttackParameter_);

		break;
	case Player::kRecovery:

		startRotate = { -pi / 2.0f,  0.0f, 0.0f };
		endRotate = { 0.0f, 0.0f, 0.0f };
		worldTransformL_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseInQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformR_arm_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseInQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformBody_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startRotate, endRotate, workAttack_.behaviorAttackParameter_);
		worldTransformBody_.transform_.rotate.x = 0.0f;

		startWeaponRotate = { -pi / 2.0f + pi,  0.0f, 0.0f };
		endWeaponRotate = { pi, 0.0f, 0.0f };
		worldTransformWeapon_.transform_.rotate = Ease::Easing(Ease::EaseName::EaseOutQuart, startWeaponRotate, endWeaponRotate, workAttack_.behaviorAttackParameter_);

		break;
	case Player::kCountOfComboPhase:
	default:
		assert(0);
		break;
	}

	// 移動量
	Vector3 move = { 0.0f, 0.0f, 1.0f };
	// 移動量に速さを反映
	move = v3Calc->Multiply(kConstAttaks[workAttack_.comboIndex_].speed_[workAttack_.inComboPhase_], v3Calc->Normalize(move));

	// 移動ベクトルをカメラの角度だけ回転する
	move = m4Calc->TransformNormal(move, worldTransform_.rotateMatrix_);

	worldTransform_.transform_.translate.x += move.x;
	worldTransform_.transform_.translate.z += move.z;

	// 移動方向に見た目を合わせる(Y軸)
	if (std::fabsf(move.x) > 0.1 || std::fabsf(move.z) > 0.1) {
		// あたん
		worldTransform_.direction_.x = v3Calc->Normalize(move).x;
		worldTransform_.direction_.z = v3Calc->Normalize(move).z;
	}

	// あたり判定
	if (workAttack_.isAttackJudgment_) {
		Vector3 attackCenterAdd = m4Calc->TransformNormal(workAttack_.attackCenterAdd_, worldTransform_.worldMatrix_);

		workAttack_.attackCollider_->center_ = { worldTransform_.worldMatrix_.m[3][0] + attackCenterAdd.x,
		worldTransform_.worldMatrix_.m[3][1] + attackCenterAdd.y,
		worldTransform_.worldMatrix_.m[3][2] + attackCenterAdd.z };
		workAttack_.attackCollider_->worldTransformUpdate();
	}


}

void Player::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";

	workDash_.period_ = globalVariables->GetIntValue(groupName, "dashPeriod");

}

void Player::allUpdateMatrix(){
	worldTransform_.UpdateMatrix();
	worldTransformBody_.UpdateMatrix();
	worldTransformHead_.UpdateMatrix();
	worldTransformL_arm_.UpdateMatrix();
	worldTransformR_arm_.UpdateMatrix();
	worldTransformWeapon_.UpdateMatrix();
}
