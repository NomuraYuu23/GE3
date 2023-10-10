#include "GameScene.h"

#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "WinApp.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "D3DResourceLeakChecker.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameScene::GameScene(){}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene(){}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection.Initialize();

	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	material_.reset(Material::Create());
	model_.reset(Model::Create("Resources", "Ball.obj", dxCommon_));
	model_->SetMaterial(material_.get());

	worldTransform1.Initialize();
	worldTransform2.Initialize();
	worldTransform2.transform_.translate = { 1.0f,0.0f,0.0f };
	worldTransform2.UpdateMatrix();

	directionalLight.reset(DirectionalLight::Create());

}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update(){

	// モデル球

	// マテリアル
	TransformStructure uvTransform{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	Vector4 colorBall = { 1.0f,1.0f,1.0f,1.0f };
	int enableLightingBall = HalfLambert;
	material_->Update(uvTransform, colorBall, enableLightingBall);

	//光源
	DirectionalLightData directionalLightData;
	directionalLightData.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData.direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData.intencity = 1.0f;
	directionalLight->Update(directionalLightData);

}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw() {

	//ゲームの処理 

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	//背景スプライト描画

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();


#pragma endregion

	Model::PreDraw(dxCommon_->GetCommadList());

	//光源
	directionalLight->Draw(dxCommon_->GetCommadList());
	//モデル
	model_->Draw(worldTransform1, viewProjection);
	//モデル
	model_->Draw(worldTransform2, viewProjection);

	Model::PostDraw();

#pragma region 前景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	//背景スプライト描画

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}
