#include "Pause.h"
#include "../Math/DeltaTime.h"

void Pause::Initialize(const std::array<uint32_t, PauseTextureNo::kCountOfPauseTextureNo>& textureHandles)
{

	input_ = Input::GetInstance();
	isPause_ = false;
	pauseMenuSelect_ = PauseMenu::kReturnToGame;
	goToTheTitle_ = false;
	textureHandles_ = textureHandles;

	// スプライト
	Vector4 color = {1.0f,1.0f,1.0f,1.0f};
	Vector2 position = { 640.0f, 180.0f };
	pausingSprite_.reset(Sprite::Create(textureHandles_[PauseTextureNo::kPausingTextureNo], position, color));
	position = { 640.0f, 360.0f };
	goToTitleSprite_.reset(Sprite::Create(textureHandles_[PauseTextureNo::kGoToTitleTextureNo], position, color));
	position = { 640.0f, 450.0f };
	returnToGameSprite_.reset(Sprite::Create(textureHandles_[PauseTextureNo::kReturnToGameTextureNo], position, color));

}

void Pause::Update()
{

	PoseSwitching();

	if (isPause_) {
		PauseMenuOperation();
	}

}

void Pause::Draw()
{

	if (isPause_) {
		pausingSprite_->Draw();
		goToTitleSprite_->Draw();
		returnToGameSprite_->Draw();
	}

}

void Pause::PoseSwitching()
{

	if (input_->TriggerKey(DIK_TAB) || input_->TriggerJoystick(7)) {
		if (isPause_) {
			isPause_ = false;
		}
		else {
			isPause_ = true;
			goToTheTitle_ = false;
			pauseMenuSelect_ = PauseMenu::kReturnToGame;
		}
	}

}

void Pause::PauseMenuOperation()
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();

	bool isPuls = false;
	bool isMinus = false;

	//移動
	if (input_->GetJoystickConnected() && selectElapsedCooltime_ <= 0.0f) {

		const float kThreshold = 0.7f;

		// 移動量
		Vector3 move = { 0.0f, input_->GetLeftAnalogstick().y, 0.0f };
		if (v3Calc->Length(move) > kThreshold) {

			// クールタイム経過時間初期化
			selectElapsedCooltime_ = selectCooltime_;

			if (move.y > 0) {
				isPuls = true;
			}
			else {
				isMinus = true;
			}

		}
	}
	else if (selectElapsedCooltime_ > 0.0f) {
		selectElapsedCooltime_ -= kDeltaTime_;
	}

	// メニュー移動(上)
	if (input_->TriggerKey(DIK_W) || input_->TriggerKey(DIK_UP) || isPuls) {
		pauseMenuSelect_--;
		if (pauseMenuSelect_ < 0) {
			pauseMenuSelect_ = PauseMenu::kCountOfPauseMenu - 1;
		}
	}
	// メニュー移動(下)
	else if (input_->TriggerKey(DIK_S) || input_->TriggerKey(DIK_DOWN) || isMinus) {
		pauseMenuSelect_++;
		if (pauseMenuSelect_ == PauseMenu::kCountOfPauseMenu) {
			pauseMenuSelect_ = 0;
		}
	}

	switch (pauseMenuSelect_)
	{
	case PauseMenu::kGoToTitle:
		PauseMenuGoToTitle();
		break;
	case PauseMenu::kReturnToGame:
		PauseMenuReturnToGame();
		break;
	default:
		break;
	}

}

void Pause::PauseMenuGoToTitle()
{

	// 選択している部分を色変更
	Vector4 white = { 1.0f,1.0f,1.0f,1.0f };
	Vector4 red = { 1.0f,0.0f,0.0f,1.0f };

	returnToGameSprite_->SetColor(white);
	goToTitleSprite_->SetColor(red);

	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerJoystick(0)) {
		goToTheTitle_ = true;
	}


}

void Pause::PauseMenuReturnToGame()
{

	// 選択している部分を色変更(黒)
	Vector4 red = { 1.0f,0.1f,0.1f,1.0f };
	Vector4 white = { 1.0f,1.0f,1.0f,1.0f };


	returnToGameSprite_->SetColor(red);
	goToTitleSprite_->SetColor(white);

	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerJoystick(0)) {
		isPause_ = false;
	}

}
