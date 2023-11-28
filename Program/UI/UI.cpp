#include "UI.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Program/Player/Player.h"

void UI::UIData::Initialize(uint32_t textureHandle)
{

	textureHandle_ = textureHandle;

	position_ = { 0.0f,0.0f };

	color_ = { 1.0f,1.0f,1.0f, 1.0f };

	sprite_.reset(Sprite::Create(textureHandle_, position_, color_));

	Vector2 size = { sprite_->GetSize().x * 0.5f,sprite_->GetSize().y * 0.5f };
	sprite_->SetSize(size);

}

void UI::UIData::Update()
{

	sprite_->SetPosition(position_);
	sprite_->SetColor(color_);

}

void UI::Initialize(const std::array<uint32_t, kCountOfTextureHandleIndex>& textureHandles)
{

	input_ = Input::GetInstance();

	for (uint32_t i = 0; i < kCountOfTextureHandleIndex; i++) {
		uiDatas_[i].Initialize(textureHandles[i]);
	}

	// グローバル
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "UI";
	//グループを追加
	GlobalVariables::GetInstance()->CreateGroup(groupName);

	globalVariables->AddItem(groupName, "kJumpButtonPositionX", static_cast<float>(uiDatas_[kJumpButton].position_.x));
	globalVariables->AddItem(groupName, "kJumpButtonPositionY", static_cast<float>(uiDatas_[kJumpButton].position_.y));

	globalVariables->AddItem(groupName, "kMoveButtonPositionX", static_cast<float>(uiDatas_[kMoveButton].position_.x));
	globalVariables->AddItem(groupName, "kMoveButtonPositionY", static_cast<float>(uiDatas_[kMoveButton].position_.y));

	globalVariables->AddItem(groupName, "kPauseButtonPositionX", static_cast<float>(uiDatas_[kPauseButton].position_.x));
	globalVariables->AddItem(groupName, "kPauseButtonPositionY", static_cast<float>(uiDatas_[kPauseButton].position_.y));

	globalVariables->AddItem(groupName, "kJumpOperationPositionX", static_cast<float>(uiDatas_[kJumpOperation].position_.x));
	globalVariables->AddItem(groupName, "kJumpOperationPositionY", static_cast<float>(uiDatas_[kJumpOperation].position_.y));

	globalVariables->AddItem(groupName, "kMoveOperationPositionX", static_cast<float>(uiDatas_[kMoveOperation].position_.x));
	globalVariables->AddItem(groupName, "kMoveOperationPositionY", static_cast<float>(uiDatas_[kMoveOperation].position_.y));
	
	globalVariables->AddItem(groupName, "kPauseOperationPositionX", static_cast<float>(uiDatas_[kPauseOperation].position_.x));
	globalVariables->AddItem(groupName, "kPauseOperationPositionY", static_cast<float>(uiDatas_[kPauseOperation].position_.y));

	ApplyGlobalVariables();

}

void UI::Update()
{

	ApplyGlobalVariables();

	for (uint32_t i = 0; i < kCountOfTextureHandleIndex; i++) {
		uiDatas_[i].Update();
	}

}

void UI::Draw()
{

	for (uint32_t i = 0; i < kCountOfTextureHandleIndex; i++) {
		uiDatas_[i].sprite_->Draw();
	}

}

void UI::ApplyGlobalVariables()
{

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "UI";

	uiDatas_[kJumpButton].position_.x = globalVariables->GetFloatValue(groupName, "kJumpButtonPositionX");
	uiDatas_[kJumpButton].position_.y = globalVariables->GetFloatValue(groupName, "kJumpButtonPositionY");

	uiDatas_[kMoveButton].position_.x = globalVariables->GetFloatValue(groupName, "kMoveButtonPositionX");
	uiDatas_[kMoveButton].position_.y = globalVariables->GetFloatValue(groupName, "kMoveButtonPositionY");

	uiDatas_[kPauseButton].position_.x = globalVariables->GetFloatValue(groupName, "kPauseButtonPositionX");
	uiDatas_[kPauseButton].position_.y = globalVariables->GetFloatValue(groupName, "kPauseButtonPositionY");

	uiDatas_[kJumpOperation].position_.x = globalVariables->GetFloatValue(groupName, "kJumpOperationPositionX");
	uiDatas_[kJumpOperation].position_.y = globalVariables->GetFloatValue(groupName, "kJumpOperationPositionY");

	uiDatas_[kMoveOperation].position_.x = globalVariables->GetFloatValue(groupName, "kMoveOperationPositionX");
	uiDatas_[kMoveOperation].position_.y = globalVariables->GetFloatValue(groupName, "kMoveOperationPositionY");

	uiDatas_[kPauseOperation].position_.x = globalVariables->GetFloatValue(groupName, "kPauseOperationPositionX");
	uiDatas_[kPauseOperation].position_.y = globalVariables->GetFloatValue(groupName, "kPauseOperationPositionY");

}