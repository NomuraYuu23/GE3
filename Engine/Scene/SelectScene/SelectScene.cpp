#include "SelectScene.h"
#include "../../../Engine/Math/DeltaTime.h"
#include "../SceneManager/SceneManager.h"
#include "../../base/TextureManager.h"

void SelectScene::Initialize()
{

	//ModelCreate();
	//MaterialCreate();
	//TextureLoad();

	BackGroundInitialize();

	SelectionSquareInitialize();

	OperationInitialize();

}

void SelectScene::Update()
{

	SelectChange();

	// タイトルへ
	if (input_->TriggerJoystick(1)) {
		SelectReturn();
	}

	// ゲームへ
	if (input_->TriggerJoystick(0)) {
		SelectDecision();
	}

}

void SelectScene::Draw()
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
		
	// 背景スプライト
	background_.sprite_->Draw();

	// 選択マススプライト
	for (uint32_t i = 0; i < selectionSquareMax_; i++) {
		selectionSquare_[i].sprite_->Draw();
	}

	// 操作説明スプライト
	decisionOperation_.sprite_->Draw();
	moveOperation_.sprite_->Draw();
	returnOperation_.sprite_->Draw();

	decisionButton_.sprite_->Draw();
	moveButton_.sprite_->Draw();
	returnButton_.sprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void SelectScene::ModelCreate()
{
}

void SelectScene::MaterialCreate()
{
}

void SelectScene::TextureLoad()
{
}

void SelectScene::SelectChange()
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

			if (move.x > 0) {
				selectionSquareNum_++;
				if (selectionSquareNum_ == selectionSquareMax_) {
					selectionSquareNum_ = 0;
				}
			}
			else {
				selectionSquareNum_--;
				if (selectionSquareNum_ == selectionSquareMax_) {
					selectionSquareNum_ = selectionSquareMax_ - 1;
				}
			}
		}
	}
	else if (selectElapsedCooltime_ > 0.0f) {
		selectElapsedCooltime_ -= kDeltaTime_;
	}

}

void SelectScene::SelectDecision()
{

	// 行きたいシーンへ
	requestSeneNo = kGame;

	// シーンマネージャーにステージ番号を送る
	sceneManager_->SetStageNum(selectionSquareNum_);

}

void SelectScene::SelectReturn()
{

	// 行きたいシーンへ
	requestSeneNo = kTitle;

}

void SelectScene::BackGroundInitialize()
{

	// 背景スプライト
	background_.Initialize(TextureManager::Load("Resources/TD2_November/skydome/skydome.png", dxCommon_));
	background_.position_ = { 640.0f,360.0f };
	background_.sprite_->SetSize(Vector2{ 1280.0f, 720.0f });
	background_.Update();

}

void SelectScene::SelectionSquareInitialize()
{

	float sizeMagnification = 1.0f;
	Vector2 size = { 0.0f,0.0f };

	// 選択マススプライト
	for (uint32_t i = 0; i < selectionSquareMax_; i++) {
		selectionSquare_[i].Initialize(TextureManager::Load("Resources/TD2_November/UI/stickL.png", dxCommon_));
		size = { selectionSquare_[i].sprite_->GetSize().x * sizeMagnification, selectionSquare_[i].sprite_->GetSize().y * sizeMagnification };
		selectionSquare_[i].sprite_->SetSize(size);
		// ポジションをここでかえる
		selectionSquare_[i].Update();
	}

}

void SelectScene::OperationInitialize()
{

	Vector2 size = { 0.0f,0.0f };
	float sizeMagnification = 0.3f;

	// 操作説明スプライト
	decisionOperation_.Initialize(TextureManager::Load("Resources/TD2_November/UI/decisionOperation.png", dxCommon_));
	size = { decisionOperation_.sprite_->GetSize().x * sizeMagnification, decisionOperation_.sprite_->GetSize().y * sizeMagnification };
	decisionOperation_.sprite_->SetSize(size);
	decisionOperation_.position_ = { 150.0f, 550.0f };
	decisionOperation_.Update();

	moveOperation_.Initialize(TextureManager::Load("Resources/TD2_November/UI/moveOperation.png", dxCommon_));
	size = { moveOperation_.sprite_->GetSize().x * sizeMagnification, moveOperation_.sprite_->GetSize().y * sizeMagnification };
	moveOperation_.sprite_->SetSize(size);
	moveOperation_.position_ = { 450.0f, 550.0f };
	moveOperation_.Update();
	
	returnOperation_.Initialize(TextureManager::Load("Resources/TD2_November/UI/returnOperation.png", dxCommon_));
	size = { returnOperation_.sprite_->GetSize().x * sizeMagnification, returnOperation_.sprite_->GetSize().y * sizeMagnification };
	returnOperation_.sprite_->SetSize(size);
	returnOperation_.position_ = { 750.0f, 550.0f };
	returnOperation_.Update();

	decisionButton_.Initialize(TextureManager::Load("Resources/TD2_November/UI/buttonA.png", dxCommon_));
	size = { decisionButton_.sprite_->GetSize().x * sizeMagnification, decisionButton_.sprite_->GetSize().y * sizeMagnification };
	decisionButton_.sprite_->SetSize(size);
	decisionButton_.position_ = { 100.0f, 550.0f };
	decisionButton_.Update();
	
	moveButton_.Initialize(TextureManager::Load("Resources/TD2_November/UI/stickL.png", dxCommon_));
	size = { moveButton_.sprite_->GetSize().x * sizeMagnification, moveButton_.sprite_->GetSize().y * sizeMagnification };
	moveButton_.sprite_->SetSize(size);
	moveButton_.position_ = { 400.0f, 550.0f };
	moveButton_.Update();
	
	returnButton_.Initialize(TextureManager::Load("Resources/TD2_November/UI/buttonB.png", dxCommon_));
	size = { returnButton_.sprite_->GetSize().x * sizeMagnification, returnButton_.sprite_->GetSize().y * sizeMagnification };
	returnButton_.sprite_->SetSize(size);
	returnButton_.position_ = { 700.0f, 550.0f };
	returnButton_.Update();

}

void SelectScene::SelectUIData::Initialize(uint32_t textureHandle)
{

	textureHandle_ = textureHandle;

	position_ = { 0.0f,0.0f };

	color_ = { 1.0f,1.0f,1.0f, 1.0f };

	sprite_.reset(Sprite::Create(textureHandle_, position_, color_));

}

void SelectScene::SelectUIData::Update()
{

	sprite_->SetPosition(position_);
	sprite_->SetColor(color_);

}
