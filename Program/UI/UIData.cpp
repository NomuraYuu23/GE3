#include "UIData.h"

void UIData::Initialize(uint32_t textureHandle)
{

	textureHandle_ = textureHandle;

	position_ = { 0.0f,0.0f };

	color_ = { 1.0f,1.0f,1.0f, 1.0f };

	sprite_.reset(Sprite::Create(textureHandle_, position_, color_));

}

void UIData::Update()
{

	sprite_->SetPosition(position_);
	sprite_->SetColor(color_);

}