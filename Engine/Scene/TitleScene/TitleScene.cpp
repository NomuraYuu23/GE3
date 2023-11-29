#include "TitleScene.h"
#include "../../base/TextureManager.h"
#include "../../2D/ImguiManager.h"

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

	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get(), skydomeMaterial_.get());

	playerWorldTransform_.Initialize();
	playerWorldTransform_.transform_.translate = {1.1f, 0.1f, -6.0f };
	playerWorldTransform_.transform_.rotate.y = 3.14f;
	playerWorldTransform_.UpdateMatrix();

	TransformStructure transformStructure = { {1.0f, 1.0f, 1.0f},{0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
	Vector4 color = {1.0f, 1.0f, 1.0f, 1.0f};

	playerMaterial_->Update(transformStructure, color, HalfLambert);


}

void TitleScene::Update()
{

	if ((input_->TriggerKey(DIK_SPACE) || input_->TriggerJoystick(0)) && !startAnimationVariables_.isAnimation_) {
		// 行きたいシーンへ
		requestSeneNo = kSelect;
		startAnimationVariables_.isAnimation_ = true;
	}

	if (startAnimationVariables_.isAnimation_) {
		StartAnimation();
	}

	playerWorldTransform_.transform_.rotate.y += 0.01f;
	playerWorldTransform_.transform_.rotate.y = std::fmod(playerWorldTransform_.transform_.rotate.y, 6.28f);
	playerWorldTransform_.UpdateMatrix();

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

	//光源
	directionalLight_->Draw(dxCommon_->GetCommadList());

	//3Dオブジェクトはここ
	skydome_->Draw(viewProjection_);
	playerModel_->Draw(playerWorldTransform_, viewProjection_,playerMaterial_.get());


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

	skydomeModel_.reset(Model::Create("Resources/TD2_November/skydome/", "skydome.obj", dxCommon_));
	playerModel_.reset(Model::Create("Resources/TD2_November/Player/", "playerBomb.obj", dxCommon_));

}

void TitleScene::MaterialCreate()
{

	skydomeMaterial_.reset(Material::Create());
	playerMaterial_.reset(Material::Create());

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
