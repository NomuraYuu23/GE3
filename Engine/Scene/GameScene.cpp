#include "GameScene.h"

#include "../../externals/imgui/imgui_impl_dx12.h"
#include "../../externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "../base/WinApp.h"
#include "../base/DirectXCommon.h"
#include "../base/TextureManager.h"
#include "../base/D3DResourceLeakChecker.h"
#include <vector>

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

	//機能
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ビュープロジェクション
	viewProjection_.Initialize();

	//デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	isDebugCameraActive_ = false;

	//光源
	directionalLight.reset(DirectionalLight::Create());

}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update(){

	//光源
	DirectionalLightData directionalLightData;
	directionalLightData.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData.direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData.intencity = 1.0f;
	directionalLight->Update(directionalLightData);

	// デバッグカメラ
	DebugCameraUpdate();

}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw() {

	//ゲームの処理 

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();


#pragma endregion

	Model::PreDraw(dxCommon_->GetCommadList());

	//光源
	directionalLight->Draw(dxCommon_->GetCommadList());

	Model::PostDraw();

#pragma region 前景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	//背景スプライト描画

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::DebugCameraUpdate()
{

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
		else {
			isDebugCameraActive_ = true;
		}
	}

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		// デバッグカメラのビュー行列をコピー
		viewProjection_ = debugCamera_->GetViewProjection();
		// ビュー行列の転送
		viewProjection_.UpdateMatrix();
	}
#endif

}
