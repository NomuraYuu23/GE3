#include "UI.h"
#include "../../Engine/GlobalVariables/GlobalVariables.h"
#include "../../Program/Player/Player.h"

void UI::Initialize(const std::array<uint32_t, kCountOfTextureHandleIndex>& textureHandles)
{

	input_ = Input::GetInstance();

	for (uint32_t i = 0; i < kCountOfTextureHandleIndex; i++) {
		uiDatas_[i].Initialize(textureHandles[i]);
	}

	OperationInitialize();
	RemainingExplosionInitialize();

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

	globalVariables->AddItem(groupName, "kCameraButtonPositionX", static_cast<float>(uiDatas_[kCameraButton].position_.x));
	globalVariables->AddItem(groupName, "kCameraButtonPositionY", static_cast<float>(uiDatas_[kCameraButton].position_.y));

	globalVariables->AddItem(groupName, "kJumpOperationPositionX", static_cast<float>(uiDatas_[kJumpOperation].position_.x));
	globalVariables->AddItem(groupName, "kJumpOperationPositionY", static_cast<float>(uiDatas_[kJumpOperation].position_.y));

	globalVariables->AddItem(groupName, "kMoveOperationPositionX", static_cast<float>(uiDatas_[kMoveOperation].position_.x));
	globalVariables->AddItem(groupName, "kMoveOperationPositionY", static_cast<float>(uiDatas_[kMoveOperation].position_.y));
	
	globalVariables->AddItem(groupName, "kPauseOperationPositionX", static_cast<float>(uiDatas_[kPauseOperation].position_.x));
	globalVariables->AddItem(groupName, "kPauseOperationPositionY", static_cast<float>(uiDatas_[kPauseOperation].position_.y));

	globalVariables->AddItem(groupName, "kCameraOperationPositionX", static_cast<float>(uiDatas_[kCameraOperation].position_.x));
	globalVariables->AddItem(groupName, "kCameraOperationPositionY", static_cast<float>(uiDatas_[kCameraOperation].position_.y));

	globalVariables->AddItem(groupName, "kRemainingExplosionPositionX", static_cast<float>(uiDatas_[kRemainingExplosion].position_.x));
	globalVariables->AddItem(groupName, "kRemainingExplosionPositionY", static_cast<float>(uiDatas_[kRemainingExplosion].position_.y));

	globalVariables->AddItem(groupName, "kNumHundredPositionX", static_cast<float>(uiDatas_[kNumHundred].position_.x));
	globalVariables->AddItem(groupName, "kNumHundredPositionY", static_cast<float>(uiDatas_[kNumHundred].position_.y));

	globalVariables->AddItem(groupName, "kNumTenPositionX", static_cast<float>(uiDatas_[kNumTen].position_.x));
	globalVariables->AddItem(groupName, "kNumTenPositionY", static_cast<float>(uiDatas_[kNumTen].position_.y));

	globalVariables->AddItem(groupName, "kNumOnePositionX", static_cast<float>(uiDatas_[kNumOne].position_.x));
	globalVariables->AddItem(groupName, "kNumOnePositionY", static_cast<float>(uiDatas_[kNumOne].position_.y));

	ApplyGlobalVariables();

}

void UI::Update()
{

	ApplyGlobalVariables();

	RemainingExplosionUpdate();

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

	uiDatas_[kCameraButton].position_.x = globalVariables->GetFloatValue(groupName, "kCameraButtonPositionX");
	uiDatas_[kCameraButton].position_.y = globalVariables->GetFloatValue(groupName, "kCameraButtonPositionY");

	uiDatas_[kJumpOperation].position_.x = globalVariables->GetFloatValue(groupName, "kJumpOperationPositionX");
	uiDatas_[kJumpOperation].position_.y = globalVariables->GetFloatValue(groupName, "kJumpOperationPositionY");

	uiDatas_[kMoveOperation].position_.x = globalVariables->GetFloatValue(groupName, "kMoveOperationPositionX");
	uiDatas_[kMoveOperation].position_.y = globalVariables->GetFloatValue(groupName, "kMoveOperationPositionY");

	uiDatas_[kPauseOperation].position_.x = globalVariables->GetFloatValue(groupName, "kPauseOperationPositionX");
	uiDatas_[kPauseOperation].position_.y = globalVariables->GetFloatValue(groupName, "kPauseOperationPositionY");

	uiDatas_[kCameraOperation].position_.x = globalVariables->GetFloatValue(groupName, "kCameraOperationPositionX");
	uiDatas_[kCameraOperation].position_.y = globalVariables->GetFloatValue(groupName, "kCameraOperationPositionY");

	uiDatas_[kRemainingExplosion].position_.x = globalVariables->GetFloatValue(groupName, "kRemainingExplosionPositionX");
	uiDatas_[kRemainingExplosion].position_.y = globalVariables->GetFloatValue(groupName, "kRemainingExplosionPositionY");

	uiDatas_[kNumHundred].position_.x = globalVariables->GetFloatValue(groupName, "kNumHundredPositionX");
	uiDatas_[kNumHundred].position_.y = globalVariables->GetFloatValue(groupName, "kNumHundredPositionY");

	uiDatas_[kNumTen].position_.x = globalVariables->GetFloatValue(groupName, "kNumTenPositionX");
	uiDatas_[kNumTen].position_.y = globalVariables->GetFloatValue(groupName, "kNumTenPositionY");

	uiDatas_[kNumOne].position_.x = globalVariables->GetFloatValue(groupName, "kNumOnePositionX");
	uiDatas_[kNumOne].position_.y = globalVariables->GetFloatValue(groupName, "kNumOnePositionY");

}

void UI::OperationInitialize() {

	float sizeMagnification = 0.3f;

	for (uint32_t i = 0; i < kTextureHandleIndex::kRemainingExplosion; i++) {

		Vector2 size = { uiDatas_[i].sprite_->GetSize().x * sizeMagnification, uiDatas_[i].sprite_->GetSize().y * sizeMagnification };
		uiDatas_[i].sprite_->SetSize(size);

	}

}

void UI::OperationUpdate() {

}

void UI::RemainingExplosionInitialize() {

	float sizeMagnification = 0.5f;

	Vector2 size = { uiDatas_[kTextureHandleIndex::kRemainingExplosion].sprite_->GetSize().x * sizeMagnification, uiDatas_[kTextureHandleIndex::kRemainingExplosion].sprite_->GetSize().y * sizeMagnification };
	uiDatas_[kRemainingExplosion].sprite_->SetSize(size);

	sizeMagnification = 2.0f;

	for (uint32_t i = kTextureHandleIndex::kNumHundred; i < kTextureHandleIndex::kCountOfTextureHandleIndex; i++) {

		uiDatas_[i].sprite_->SetTextureSize(Vector2{ 64.0f, 64.0f});
		uiDatas_[i].sprite_->SetTextureLeftTop(Vector2{ 0.0f,0.0f});
		Vector2 size = { uiDatas_[i].sprite_->GetSize().x * sizeMagnification * 0.1f, uiDatas_[i].sprite_->GetSize().y * sizeMagnification };
		uiDatas_[i].sprite_->SetSize(size);

	}

}

void UI::RemainingExplosionUpdate() {

	for (uint32_t i = kTextureHandleIndex::kNumHundred; i < kTextureHandleIndex::kCountOfTextureHandleIndex; i++) {
		float left = player_->GetExprosionNum() * 64.0f;
		uiDatas_[i].sprite_->SetTextureLeftTop(Vector2{ left, 0.0f });
	}

	float left = (player_->GetExprosionNum() / 100) * 64.0f;
	uiDatas_[kTextureHandleIndex::kNumHundred].sprite_->SetTextureLeftTop(Vector2{ left, 0.0f });

	left = ((player_->GetExprosionNum() % 100) / 10) * 64.0f;
	uiDatas_[kTextureHandleIndex::kNumTen].sprite_->SetTextureLeftTop(Vector2{ left, 0.0f });

	left = (player_->GetExprosionNum() % 10) * 64.0f;
	uiDatas_[kTextureHandleIndex::kNumOne].sprite_->SetTextureLeftTop(Vector2{ left, 0.0f });

}