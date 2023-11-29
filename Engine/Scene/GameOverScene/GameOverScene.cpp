#include "GameOverScene.h"
#include "../../Math/DeltaTime.h"
#include "../../base/TextureManager.h"
#include "../SceneManager/SceneManager.h"

void GameOverScene::Initialize()
{

	//ModelCreate();
	//MaterialCreate();
	//TextureLoad();

	if (respawnConditions <= sceneManager_->GetRespawnItem()) {
		canRespawn = true;

		goToSelect_.Initialize(TextureManager::Load("Resources/TD2_November/gameOver/goToSelect.png", dxCommon_));
		goToSelect_.position_ = { 320.0f, 550.0f };
		goToSelect_.color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
		goToSelect_.Update();
		goToSelect_.sprite_->SetSize(Vector2{ goToSelect_.sprite_->GetTextureInitSize().x * 0.7f, goToSelect_.sprite_->GetTextureInitSize().y * 0.7f });

		respawn_.Initialize(TextureManager::Load("Resources/TD2_November/gameOver/respawn.png", dxCommon_));
		respawn_.position_ = { 960.0f, 550.0f };
		respawn_.color_ = { 1.0f, 0.1f, 0.1f, 1.0f };
		respawn_.Update();
		respawn_.sprite_->SetSize(Vector2{ respawn_.sprite_->GetTextureInitSize().x * 0.7f, respawn_.sprite_->GetTextureInitSize().y * 0.7f });

		Vector2 size = { 0.0f,0.0f };
		float sizeMagnification = 0.5f;

		// 操作説明スプライト
		decisionOperation_.Initialize(TextureManager::Load("Resources/TD2_November/UI/decisionOperation.png", dxCommon_));
		size = { decisionOperation_.sprite_->GetSize().x * sizeMagnification, decisionOperation_.sprite_->GetSize().y * sizeMagnification };
		decisionOperation_.sprite_->SetSize(size);
		decisionOperation_.position_ = { 240.0f + 60.0f + 100.0f, 650.0f };
		decisionOperation_.Update();

		moveOperation_.Initialize(TextureManager::Load("Resources/TD2_November/UI/moveOperation.png", dxCommon_));
		size = { moveOperation_.sprite_->GetSize().x * sizeMagnification, moveOperation_.sprite_->GetSize().y * sizeMagnification };
		moveOperation_.sprite_->SetSize(size);
		moveOperation_.position_ = { 240.0f * 3.0f + 60.0f + 100.0f, 650.0f };
		moveOperation_.Update();

		decisionButton_.Initialize(TextureManager::Load("Resources/TD2_November/UI/buttonA.png", dxCommon_));
		size = { decisionButton_.sprite_->GetSize().x * sizeMagnification, decisionButton_.sprite_->GetSize().y * sizeMagnification };
		decisionButton_.sprite_->SetSize(size);
		decisionButton_.position_ = { 240.0f + 60.0f, 650.0f };
		decisionButton_.Update();

		moveButton_.Initialize(TextureManager::Load("Resources/TD2_November/UI/stickL.png", dxCommon_));
		size = { moveButton_.sprite_->GetSize().x * sizeMagnification, moveButton_.sprite_->GetSize().y * sizeMagnification };
		moveButton_.sprite_->SetSize(size);
		moveButton_.position_ = { 240.0f * 3.0f + 60.0f, 650.0f };
		moveButton_.Update();

		isRespawn_ = true;

	}
	else {
		canRespawn = false;

		goToSelect_.Initialize(TextureManager::Load("Resources/TD2_November/gameOver/goToSelect.png", dxCommon_));
		goToSelect_.position_ = { 640.0f, 550.0f };
		goToSelect_.color_ = { 1.0f, 0.1f, 0.1f, 1.0f };
		goToSelect_.Update();
		goToSelect_.sprite_->SetSize(Vector2{ goToSelect_.sprite_->GetTextureInitSize().x * 0.7f, goToSelect_.sprite_->GetTextureInitSize().y * 0.7f });


		Vector2 size = { 0.0f,0.0f };
		float sizeMagnification = 0.5f;

		// 操作説明スプライト
		decisionOperation_.Initialize(TextureManager::Load("Resources/TD2_November/UI/decisionOperation.png", dxCommon_));
		size = { decisionOperation_.sprite_->GetSize().x * sizeMagnification, decisionOperation_.sprite_->GetSize().y * sizeMagnification };
		decisionOperation_.sprite_->SetSize(size);
		decisionOperation_.position_ = { 640.0f + 60.0f, 650.0f };
		decisionOperation_.Update();

		decisionButton_.Initialize(TextureManager::Load("Resources/TD2_November/UI/buttonA.png", dxCommon_));
		size = { decisionButton_.sprite_->GetSize().x * sizeMagnification, decisionButton_.sprite_->GetSize().y * sizeMagnification };
		decisionButton_.sprite_->SetSize(size);
		decisionButton_.position_ = { 640.0f - 60.0f, 650.0f };
		decisionButton_.Update();

		isRespawn_ = false;

	}

	backGround_.Initialize(TextureManager::Load("Resources/TD2_November/gameOver/backGround.png", dxCommon_));
	backGround_.position_ = { 640.0f, 360.0f};
	backGround_.Update();
	backGround_.sprite_->SetSize(Vector2{ 1280.0f,720.0f });

}

void GameOverScene::Update()
{

	if (isDecided_) {
		return;
	}

	Animation();

	if (canRespawn) {
		SelectChange();
	}

	if (input_->TriggerJoystick(0)) {
		Decision();
	}

}

void GameOverScene::Draw()
{

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	backGround_.sprite_->Draw();

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
	if (canRespawn) {
		respawn_.sprite_->Draw();

		// 操作説明スプライト
		moveOperation_.sprite_->Draw();
		moveButton_.sprite_->Draw();
	}

	goToSelect_.sprite_->Draw();
	decisionOperation_.sprite_->Draw();
	decisionButton_.sprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameOverScene::ModelCreate()
{
}

void GameOverScene::MaterialCreate()
{
}

void GameOverScene::TextureLoad()
{
}

void GameOverScene::SelectChange()
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	//移動
	if (input_->GetJoystickConnected() && selectElapsedCooltime_ <= 0.0f) {

		const float kThreshold = 0.7f;

		// 移動量
		Vector3 move = { input_->GetLeftAnalogstick().x, 0.0f, 0.0f };
		if (v3Calc->Length(move) > kThreshold) {

			// クールタイム経過時間初期化
			selectElapsedCooltime_ = selectCooltime_;

			if (isRespawn_) {
				isRespawn_ = false;
				goToSelect_.color_ = { 1.0f, 0.1f, 0.1f, 1.0f };
				goToSelect_.Update();
				respawn_.color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
				respawn_.Update();
			}
			else {
				isRespawn_ = true;
				goToSelect_.color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
				goToSelect_.Update();
				respawn_.color_ = { 1.0f, 0.1f, 0.1f, 1.0f };
				respawn_.Update();
			}

		}
	}
	else if (selectElapsedCooltime_ > 0.0f) {
		selectElapsedCooltime_ -= kDeltaTime_;
	}

}

void GameOverScene::Decision()
{

	if (isRespawn_) {
		// 行きたいシーンへ
		requestSeneNo = kGame;
	}
	else {
		// 行きたいシーンへ
		requestSeneNo = kSelect;
		sceneManager_->SetRespawnPosition(Vector3{0.0f,0.0f,0.0f});
		sceneManager_->SetRespawnItem(0);
	}

	isDecided_ = true;

}

void GameOverScene::Animation()
{

	// アニメーションパラメータ
	animationParameter_ += 3.14f * 2.0f / animationPeriod_;

	if (animationParameter_ >= 3.14f * 2.0f) {
		animationParameter_ = std::fmodf(animationParameter_, 3.14f * 2.0f);
	}

	float plusY = std::sinf(animationParameter_) * 10.0f;

	if (canRespawn) {
		if (isRespawn_) {
			respawn_.sprite_->SetPosition(Vector2{ respawn_.position_.x, respawn_.position_.y + plusY });
			goToSelect_.sprite_->SetPosition(Vector2{ goToSelect_.position_.x, goToSelect_.position_.y});

		}
		else {
			respawn_.sprite_->SetPosition(Vector2{ respawn_.position_.x, respawn_.position_.y});
			goToSelect_.sprite_->SetPosition(Vector2{ goToSelect_.position_.x, goToSelect_.position_.y + plusY });
		}

	}
	else {
		goToSelect_.sprite_->SetPosition(Vector2{ goToSelect_.position_.x, goToSelect_.position_.y + plusY });
	}

}
