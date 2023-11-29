#include "ClearScene.h"
#include "../../base/TextureManager.h"

void ClearScene::Initialize()
{

	//ModelCreate();
	//MaterialCreate();
	//TextureLoad();

	backGround_.Initialize(TextureManager::Load("Resources/TD2_November/clear/backGround.png", dxCommon_));
	backGround_.position_ = { 640.0f, 360.0f };
	backGround_.color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	backGround_.Update();

	goToSelect_.Initialize(TextureManager::Load("Resources/TD2_November/clear/goToSelect.png", dxCommon_));
	goToSelect_.position_ = { 640.0f, 540.0f };
	goToSelect_.color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	goToSelect_.Update();

}

void ClearScene::Update(){
	if (input_->GetJoystickConnected()) {

		if (input_->TriggerKey(DIK_SPACE) || input_->TriggerJoystick(0)) {
			// 行きたいシーンへ
			requestSeneNo = kSelect;
		}
	}

	Animation();

}

void ClearScene::Draw()
{

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();


#pragma endregion

	Model::PreDraw(dxCommon_->GetCommadList());

	//3Dオブジェクトはここ

	Model::PostDraw();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());


	//背景
	//前景スプライト描画
	backGround_.sprite_->Draw();
	goToSelect_.sprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void ClearScene::ModelCreate()
{
}

void ClearScene::MaterialCreate()
{
}

void ClearScene::TextureLoad()
{
}

void ClearScene::Animation()
{

	// アニメーションパラメータ
	animationParameter_ += 3.14f * 2.0f / animationPeriod_;

	if (animationParameter_ >= 3.14f * 2.0f) {
		animationParameter_ = std::fmodf(animationParameter_, 3.14f * 2.0f);
	}

	float plusY = std::sinf(animationParameter_) * 10.0f;

	goToSelect_.sprite_->SetPosition(Vector2{ goToSelect_.position_.x, goToSelect_.position_.y + plusY });

}
