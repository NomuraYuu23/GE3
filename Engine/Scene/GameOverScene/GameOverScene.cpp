#include "GameOverScene.h"
#include "../../Math/DeltaTime.h"
#include "../../base/TextureManager.h"

void GameOverScene::Initialize()
{

	//ModelCreate();
	//MaterialCreate();
	//TextureLoad();

	backGround_.Initialize(TextureManager::Load("Resources/TD2_November/gameOver/backGround.png", dxCommon_));
	backGround_.position_ = { 640.0f, 360.0f};
	backGround_.Update();
	backGround_.sprite_->SetSize(Vector2{ 1280.0f,720.0f });

	goToSelect_.Initialize(TextureManager::Load("Resources/TD2_November/gameOver/goToSelect.png", dxCommon_));
	goToSelect_.position_ = { 320.0f, 550.0f };
	goToSelect_.color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	goToSelect_.Update();
	goToSelect_.sprite_->SetSize(Vector2{ goToSelect_.sprite_->GetTextureInitSize().x * 0.7f, goToSelect_.sprite_->GetTextureInitSize().y * 0.7f });

	respawn_.Initialize(TextureManager::Load("Resources/TD2_November/gameOver/respawn.png", dxCommon_));
	respawn_.position_ = { 960.0f, 550.0f };
	respawn_.color_ = { 1.0f, 1.0f, 0.1f, 1.0f };
	respawn_.Update();
	respawn_.sprite_->SetSize(Vector2{ respawn_.sprite_->GetTextureInitSize().x * 0.7f, respawn_.sprite_->GetTextureInitSize().y * 0.7f });
}

void GameOverScene::Update()
{

	SelectChange();

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
	goToSelect_.sprite_->Draw();
	respawn_.sprite_->Draw();

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
				goToSelect_.color_ = { 1.0f, 1.0f, 0.1f, 1.0f };
				goToSelect_.Update();
				respawn_.color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
				respawn_.Update();
			}
			else {
				isRespawn_ = true;
				goToSelect_.color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
				goToSelect_.Update();
				respawn_.color_ = { 1.0f, 1.0f, 0.1f, 1.0f };
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
	}

}
