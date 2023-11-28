#include "UI.h"

void UI::UIData::Initialize(uint32_t textureHandle)
{

	textureHandle_ = textureHandle;

	position_ = { 0.0f,0.0f };

	color_ = { 1.0f,1.0f,1.0f, 1.0f };

	sprite_.reset(Sprite::Create(textureHandle_, position_, color_));

}

void UI::Initialize(const std::array<uint32_t, kCountOfTextureHandleIndex>& textureHandles)
{

	input_ = Input::GetInstance();

	for (uint32_t i = 0; i < kCountOfTextureHandleIndex; i++) {
		uiDatas_[i].Initialize(textureHandles[i]);
	}

}

void UI::Update()
{



}

void UI::Draw()
{

	for (uint32_t i = 0; i < kCountOfTextureHandleIndex; i++) {
		uiDatas_[i].sprite_->Draw();
	}

}
