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

	BackGroundUpdate();

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
	name_.sprite_->Draw();

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
				if (selectionSquareNum_ == selectionSquareMax_ - 1) {
					selectionSquareNum_ = 0;
				}
				else {
					selectionSquareNum_++;
				}
			}
			else {
				if (selectionSquareNum_ == 0) {
					selectionSquareNum_ = selectionSquareMax_ - 1;
				}
				else {
					selectionSquareNum_--;
				}
			}

			SelectionSquareUpdate();

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

	name_.Initialize(TextureManager::Load("Resources/TD2_November/select/stageSelect.png", dxCommon_));
	name_.position_ = { 640.0f,180.0f };
	//name_.sprite_->SetSize(Vector2{ 1280.0f, 720.0f });
	name_.Update();

}

void SelectScene::BackGroundUpdate()
{

	Vector2 leftTop = background_.sprite_->GetTextureLeftTop();
	float speed = 2.0f;


	leftTop.x += speed;
	leftTop.x = std::fmodf(leftTop.x, background_.sprite_->GetTextureInitSize().x);

	background_.sprite_->SetTextureLeftTop(leftTop);

}

void SelectScene::SelectionSquareInitialize()
{

	float sizeMagnification = 1.0f;
	Vector2 size = { 0.0f,0.0f };


	selectionSquare_[0].Initialize(TextureManager::Load("Resources/TD2_November/select/select1.png", dxCommon_));
	selectionSquare_[1].Initialize(TextureManager::Load("Resources/TD2_November/select/select2.png", dxCommon_));
	selectionSquare_[2].Initialize(TextureManager::Load("Resources/TD2_November/select/select3.png", dxCommon_));
	selectionSquare_[3].Initialize(TextureManager::Load("Resources/TD2_November/select/select4.png", dxCommon_));

	// 選択マススプライト
	for (uint32_t i = 0; i < selectionSquareMax_; i++) {
		size = { selectionSquare_[i].sprite_->GetSize().x * sizeMagnification, selectionSquare_[i].sprite_->GetSize().y * sizeMagnification };
		selectionSquare_[i].sprite_->SetSize(size);
		// ポジションをここでかえる
		selectionSquare_[i].position_.x = 213.3f + 284.4f * i;
		selectionSquare_[i].position_.y = 360.0f;
		selectionSquare_[i].Update();
	}

	SelectionSquareUpdate();

}

void SelectScene::SelectionSquareUpdate()
{

	// 選択マススプライト
	for (uint32_t i = 0; i < selectionSquareMax_; i++) {
		if (i == selectionSquareNum_) {
			selectionSquare_[i].color_ = { 1.0f, 1.0f, 0.1f, 1.0f};
		}
		else {
			selectionSquare_[i].color_ = { 1.0f, 1.0f, 1.0f,1.0f };
		}
		selectionSquare_[i].Update();
	}

}

void SelectScene::OperationInitialize()
{

	Vector2 size = { 0.0f,0.0f };
	float sizeMagnification = 0.5f;

	// 操作説明スプライト
	decisionOperation_.Initialize(TextureManager::Load("Resources/TD2_November/UI/decisionOperation.png", dxCommon_));
	size = { decisionOperation_.sprite_->GetSize().x * sizeMagnification, decisionOperation_.sprite_->GetSize().y * sizeMagnification };
	decisionOperation_.sprite_->SetSize(size);
	decisionOperation_.position_ = { 310.0f, 600.0f };
	decisionOperation_.Update();

	moveOperation_.Initialize(TextureManager::Load("Resources/TD2_November/UI/moveOperation.png", dxCommon_));
	size = { moveOperation_.sprite_->GetSize().x * sizeMagnification, moveOperation_.sprite_->GetSize().y * sizeMagnification };
	moveOperation_.sprite_->SetSize(size);
	moveOperation_.position_ = { 670.0f, 600.0f };
	moveOperation_.Update();
	
	returnOperation_.Initialize(TextureManager::Load("Resources/TD2_November/UI/returnOperation.png", dxCommon_));
	size = { returnOperation_.sprite_->GetSize().x * sizeMagnification, returnOperation_.sprite_->GetSize().y * sizeMagnification };
	returnOperation_.sprite_->SetSize(size);
	returnOperation_.position_ = { 1030.0f, 600.0f };
	returnOperation_.Update();

	decisionButton_.Initialize(TextureManager::Load("Resources/TD2_November/UI/buttonA.png", dxCommon_));
	size = { decisionButton_.sprite_->GetSize().x * sizeMagnification, decisionButton_.sprite_->GetSize().y * sizeMagnification };
	decisionButton_.sprite_->SetSize(size);
	decisionButton_.position_ = { 210.0f, 600.0f };
	decisionButton_.Update();
	
	moveButton_.Initialize(TextureManager::Load("Resources/TD2_November/UI/stickL.png", dxCommon_));
	size = { moveButton_.sprite_->GetSize().x * sizeMagnification, moveButton_.sprite_->GetSize().y * sizeMagnification };
	moveButton_.sprite_->SetSize(size);
	moveButton_.position_ = { 570.0f, 600.0f };
	moveButton_.Update();
	
	returnButton_.Initialize(TextureManager::Load("Resources/TD2_November/UI/buttonB.png", dxCommon_));
	size = { returnButton_.sprite_->GetSize().x * sizeMagnification, returnButton_.sprite_->GetSize().y * sizeMagnification };
	returnButton_.sprite_->SetSize(size);
	returnButton_.position_ = { 930.0f, 600.0f };
	returnButton_.Update();

}
