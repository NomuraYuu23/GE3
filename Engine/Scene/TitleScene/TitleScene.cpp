#include "TitleScene.h"
#include "../../base/TextureManager.h"

void TitleScene::Initialize()
{

	ModelCreate();
	MaterialCreate();
	TextureLoad();

	titleNamePosition_ = { 640.0f, 360.0f};
	titleNameColor_ = {1.0f,1.0f,1.0f,1.0f};
	titleNameSprite_.reset(Sprite::Create(titleNameTextureHandle_, titleNamePosition_, titleNameColor_));

	startPosition_ = { 640.0f, 540.0f };
	startColor_ = { 1.0f,1.0f,1.0f,1.0f };
	startSprite_.reset(Sprite::Create(startTextureHandle_, startPosition_, startColor_));

	startAnimationVariables_.animationParameter_ = 0.0f;
	startAnimationVariables_.animationTime_ = 30;
	startAnimationVariables_.switchingAddTime_ = 0.2f;
	startAnimationVariables_.switchingTime_ = startAnimationVariables_.switchingAddTime_;
	startAnimationVariables_.isAnimation_ = false;


}

void TitleScene::Update()
{

	if ((input_->TriggerKey(DIK_SPACE) || input_->TriggerJoystick(0)) && !startAnimationVariables_.isAnimation_) {
		// 行きたいシーンへ
		requestSeneNo = kGame;
		startAnimationVariables_.isAnimation_ = true;
	}

	if (startAnimationVariables_.isAnimation_) {
		StartAnimation();
	}

}

void TitleScene::Draw()
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

#pragma region パーティクル描画
	Model::PreParticleDraw(dxCommon_->GetCommadList(), viewProjection_);

	//光源
	directionalLight_->Draw(dxCommon_->GetCommadList());

	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());


	//背景
	//前景スプライト描画
	titleNameSprite_->Draw();
	startSprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void TitleScene::ModelCreate()
{
}

void TitleScene::MaterialCreate()
{
}

void TitleScene::TextureLoad()
{

	titleNameTextureHandle_ = TextureManager::Load("Resources/TD2_November/title/titleName.png", dxCommon_);
	startTextureHandle_ = TextureManager::Load("Resources/TD2_November/title/start.png", dxCommon_);


}

void TitleScene::StartAnimation()
{

	startAnimationVariables_.animationParameter_ += 1.0f / startAnimationVariables_.animationTime_;
	if (startAnimationVariables_.switchingTime_ <= startAnimationVariables_.animationParameter_) {
		startAnimationVariables_.switchingTime_ += startAnimationVariables_.switchingAddTime_;
		// 表示変更
		if (startSprite_->GetIsInvisible()) {
			startSprite_->SetIsInvisible(false);
		}
		else {
			startSprite_->SetIsInvisible(true);
		}
	}
	if (1.0f <= startAnimationVariables_.animationParameter_) {
		startAnimationVariables_.isAnimation_ = false;
	}

}
