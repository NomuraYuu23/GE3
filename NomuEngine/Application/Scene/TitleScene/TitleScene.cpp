#include "TitleScene.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"

void TitleScene::Initialize()
{

	IScene::Initialize();

	ModelCreate();
	MaterialCreate();
	TextureLoad();

	sprite_.reset(Sprite::Create(
		textureHandle_,
		Vector2 { 640.0f, 360.0f },
		Vector4{ 1.0f,1.0f, 1.0f, 1.0f }));
	
	// 色
	spriteColor_ = Vector4{ 1.0f,1.0f, 1.0f, 1.0f };
	// 反転X
	spriteIsFlipX_ = false;
	// 反転Y
	spriteIsFlipY_ = false;
	// 表示をしないか
	spriteIsInvisible_ = false;
	// 位置
	spritePosition_ = sprite_->GetPosition();
	// 回転
	spriteRotate_ = sprite_->GetRotate();
	// 大きさ
	spriteSize_ = sprite_->GetSize();
	// 左上
	spriteTextureLeftTop_ = sprite_->GetTextureLeftTop();
	// テクスチャサイズ
	spriteTextureSize_ = sprite_->GetTextureSize();

	operationSprite_.reset(
		Sprite::Create(TextureManager::Load("Resources/GE3/operation.png", dxCommon_, textureHandleManager_.get()), 
			Vector2{ 0.0f,0.0f }, 
			Vector4{ 1.0f,1.0f, 1.0f, 1.0f}));
	operationSprite_->SetAnchorPoint(Vector2{ 0.0f, 0.0f });


}

void TitleScene::Update()
{

	if (input_->TriggerKey(DIK_SPACE) || input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		// 行きたいシーンへ
		requestSceneNo = kGame;
	}

	SpriteImgui();

}

void TitleScene::Draw()
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
	sprite_->Draw();
	operationSprite_->Draw();


	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void TitleScene::ModelCreate()
{
}

void TitleScene::MaterialCreate()
{
}

void TitleScene::TextureLoad()
{
	
	textureHandle_ = TextureManager::Load("Resources/default/monsterBall.png", dxCommon_, textureHandleManager_.get());
	textureHandle2_ = TextureManager::Load("Resources/default/checkerBoard.png", dxCommon_, textureHandleManager_.get());


}

void TitleScene::SpriteImgui()
{

#ifdef _USE_IMGUI

	int32_t textureHandle = sprite_->GetTevtureHandle();

	ImGui::Begin("Sprite");
	ImGui::DragFloat4("色", &spriteColor_.x, 0.01f);
	ImGui::Checkbox("X反転", &spriteIsFlipX_);
	ImGui::Checkbox("Y反転", &spriteIsFlipY_);
	ImGui::Checkbox("表示しない", &spriteIsInvisible_);
	ImGui::DragFloat2("位置", &spritePosition_.x, 0.01f);
	ImGui::DragFloat("回転", &spriteRotate_, 0.01f);
	ImGui::DragFloat2("大きさ", &spriteSize_.x, 0.01f);
	ImGui::Text("テクスチャハンドル");
	ImGui::RadioButton("モンスターボール", &textureHandle, textureHandle_);
	ImGui::SameLine(); 
	ImGui::RadioButton("チェック", &textureHandle, textureHandle2_);
	ImGui::DragFloat2("左上", &spriteTextureLeftTop_.x, 0.01f);
	ImGui::DragFloat2("テクスチャサイズ", &spriteTextureSize_.x, 0.01f);

	ImGui::End();


	sprite_->SetColor(spriteColor_);
	sprite_->SetIsFlipX(spriteIsFlipX_);
	sprite_->SetIsFlipY(spriteIsFlipY_);
	sprite_->SetIsInvisible(spriteIsInvisible_);
	sprite_->SetPosition(spritePosition_);
	sprite_->SetRotate(spriteRotate_);
	sprite_->SetSize(spriteSize_);
	sprite_->SetTextureHandle(textureHandle);
	sprite_->SetTextureLeftTop(spriteTextureLeftTop_);
	sprite_->SetTextureSize(spriteTextureSize_);

#endif // _USE_IMGUI

}
