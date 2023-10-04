#include "GameScene.h"

#include "externals/imgui/imgui_impl_dx12.h"
#include "externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "WinApp.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "D3DResourceLeakChecker.h"
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

	//光源
	directionalLight.reset(DirectionalLight::Create());

	//Player
	//マテリアル
	playerMaterial_.reset(Material::Create());
	std::vector<Material*> playerMaterials = { playerMaterial_.get() };
	//モデル
	playerModel_.reset(Model::Create("Resources/AL4/player", "player.obj", dxCommon_, playerMaterial_.get()));
	std::vector<Model*> playerModels = { playerModel_.get() };
	//オブジェクト
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels, playerMaterials, &viewProjection_);

	// 追従カメラ生成
	followCamera_ = std::make_unique<FollowCamera>();
	// 追従カメラの初期化
	followCamera_->Initialize();
	//自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(player_->GetWorldTransformAddress());

	//追従カメラのビュープロジェクションを自キャラにセット
	player_->SetViewProjection(followCamera_->GetViewProjectionAddress());

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

	//追従カメラの更新
	followCamera_->Update();
	// 追従カメラのビュープロジェクションをコピー
	viewProjection_= followCamera_->GetViewProjection();
	// ビュー行列の転送
	viewProjection_.UpdateMatrix();


	//プレイヤー
	player_->Update();

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
	//オブジェクト
	player_->Draw();

	Model::PostDraw();

#pragma region 前景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	//背景スプライト描画

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}
