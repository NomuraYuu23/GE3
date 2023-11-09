#include "Pause.h"

void Pause::Initialize(const std::array<uint32_t, kCountOfPauseTextureNo>& textureHandles)
{

	input_ = Input::GetInstance();
	isPause_ = false;
	pauseMenuSelect_ = kReturnToGame;
	textureHandles_ = textureHandles;

	// スプライト
	Vector4 color = {1.0f,1.0f,1.0f,1.0f};
	Vector2 position = { 640.0f, 360.0f };
	pausingSprite_.reset(Sprite::Create(textureHandles_[kPausing], position, color));

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
	}

}

void Pause::PoseSwitching()
{

	if (input_->TriggerKey(DIK_TAB)) {
		if (isPause_) {
			isPause_ = false;
		}
		else {
			isPause_ = true;
			pauseMenuSelect_ = kReturnToGame;
		}
	}

}

void Pause::PauseMenuOperation()
{

	// メニュー移動(上)
	if (input_->TriggerKey(DIK_W) || input_->TriggerKey(DIK_UP)) {
		pauseMenuSelect_--;
		if (pauseMenuSelect_ < 0) {
			pauseMenuSelect_ = kCountOfPauseMenu - 1;
		}
	}
	// メニュー移動(下)
	else if (input_->TriggerKey(DIK_S) || input_->TriggerKey(DIK_DOWN)) {
		pauseMenuSelect_++;
		if (pauseMenuSelect_ == kCountOfPauseMenu) {
			pauseMenuSelect_ = 0;
		}
	}

}
