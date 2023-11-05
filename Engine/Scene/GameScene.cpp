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
GameScene::GameScene(){
	
}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene(){
}

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {
	
	
	//機能
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// デバッグ描画
	colliderDebugDraw_ = std::make_unique<ColliderDebugDraw>();
	colliderSphereModel_.reset(Model::Create("Resources/TD2_November/collider/sphere/", "sphere.obj", dxCommon_));
	colliderBoxModel_.reset(Model::Create("Resources/TD2_November/collider/box/", "box.obj", dxCommon_));
	std::vector<Model*> colliderModels = { colliderSphereModel_.get(),colliderBoxModel_.get(),colliderBoxModel_.get() };
	colliderMaterial_.reset(Material::Create());
	colliderDebugDraw_->Initialize(colliderModels, colliderMaterial_.get());

	//ビュープロジェクション
	viewProjection_.Initialize();

	viewProjection_.transform_.translate = { 0.0f,23.0f,-35.0f };
	viewProjection_.transform_.rotate = { 0.58f,0.0f,0.0f };

	//デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();
	isDebugCameraActive_ = false;

	//光源
	directionalLight.reset(DirectionalLight::Create());

	//Player
	//マテリアル
	playerMaterial_.reset(Material::Create());
	std::vector<Material*> playerMaterials = { playerMaterial_.get() };
	//モデル
	// 自キャラのモデル
	playerBodyModel_.reset(Model::Create("Resources/AL4/float_Body", "float_Body.obj", dxCommon_));
	playerHeadModel_.reset(Model::Create("Resources/AL4/float_Head", "float_Head.obj", dxCommon_));
	playerL_armModel_.reset(Model::Create("Resources/AL4/float_L_arm", "float_L_arm.obj", dxCommon_));
	playerR_armModel_.reset(Model::Create("Resources/AL4/float_R_arm", "float_R_arm.obj", dxCommon_));
	playerWeaponModel_.reset(Model::Create("Resources/AL4/player_Weapon", "player_Weapon.obj", dxCommon_));

	std::vector<Model*> playerModels = {
		playerBodyModel_.get(),
		playerHeadModel_.get(),
		playerL_armModel_.get(),
		playerR_armModel_.get(),
		playerWeaponModel_.get(),
	};
	//オブジェクト
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels, playerMaterials);

	// 追従カメラ生成
	followCamera_ = std::make_unique<FollowCamera>();
	// 追従カメラの初期化
	followCamera_->Initialize();
	//自キャラのワールドトランスフォームを追従カメラにセット
	followCamera_->SetTarget(player_->GetWorldTransformAddress());

	//追従カメラのビュープロジェクションを自キャラにセット
	player_->SetViewProjection(followCamera_->GetViewProjectionAddress());

	//skydome
	//マテリアル
	skydomeMaterial_.reset(Material::Create());
	//モデル
	skydomeModel_.reset(Model::Create("Resources/AL4/skydome", "skydome.obj", dxCommon_));
	//オブジェクト
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get(), skydomeMaterial_.get());

	// フロアマネージャー
	//マテリアル
	floorMaterial_.reset(Material::Create());
	//モデル
	floorModel_.reset(Model::Create("Resources/AL4/floor", "floor.obj", dxCommon_));
	//オブジェクト
	floorManager_ = std::make_unique<FloorManager>();
	floorManager_->Initialize(floorModel_.get(), floorMaterial_.get());
	// 床生成
	floorManager_->AddFloor(Vector3(0.0f, 0.0f, 0.0f), Vector3(0.0f, 0.0f, 0.0f), false);
	floorManager_->AddFloor(Vector3(0.0f, 0.0f, 30.0f), Vector3(0.0f, -1.57f, 0.0f), true);
	floorManager_->AddFloor(Vector3(0.0f, 0.0f, 60.0f), Vector3(0.0f, 0.0f, 0.0f), false);
	floorManager_->AddFloor(Vector3(30.0f, 0.0f, 60.0f), Vector3(0.0f, 0.0f, 0.0f), true);
	floorManager_->AddFloor(Vector3(60.0f, 0.0f, 60.0f), Vector3(0.0f, 0.0f, 0.0f), false);

	// ゴール
	//マテリアル
	goalMaterial_.reset(Material::Create());
	std::vector<Material*> goalMaterials = { goalMaterial_.get() };
	//モデル
	goalModel_.reset(Model::Create("Resources/AL4/goal", "goal.obj", dxCommon_));
	std::vector<Model*> goalModels = { goalModel_.get() };
	//オブジェクト
	goal_ = std::make_unique<Goal>();
	goal_->Initialize(goalModels, goalMaterials);

	//エネミーの生成
	enemy_ = std::make_unique<Enemy>();
	enemyBodyMaterial_.reset(Material::Create());
	enemyL_armMaterial_.reset(Material::Create());
	enemyR_armMaterial_.reset(Material::Create());
	std::vector<Material*> enemyMaterials = {
		enemyBodyMaterial_.get(),
		enemyL_armMaterial_.get(),
		enemyR_armMaterial_.get() };

	//エネミーのモデル
	enemyBodyModel_.reset(Model::Create("Resources/AL4/enemy_Body", "enemy_Body.obj", dxCommon_));
	enemyL_armModel_.reset(Model::Create("Resources/AL4/enemy_Arm", "enemy_Arm.obj", dxCommon_));
	enemyR_armModel_.reset(Model::Create("Resources/AL4/enemy_Arm", "enemy_Arm.obj", dxCommon_));

	std::vector<Model*> enemyModels = {
		enemyBodyModel_.get(),
		enemyL_armModel_.get(),
		enemyR_armModel_.get()
	};

	//エネミーの初期化
	enemy_->Initialize(enemyModels, enemyMaterials);

	// 衝突マネージャー
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize(player_.get(), floorManager_.get(), goal_.get(), enemy_.get());
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update(){
	ImguiDraw();
	//光源
	DirectionalLightData directionalLightData;
	directionalLightData.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData.direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData.intencity = 1.0f;
	directionalLight->Update(directionalLightData);


	//追従カメラの更新
	followCamera_->Update();
	// 追従カメラのビュープロジェクションをコピー
	viewProjection_ = followCamera_->GetViewProjection();
	// ビュー行列の転送
	viewProjection_.UpdateMatrix();

	// デバッグカメラ
	DebugCameraUpdate();
	
	//プレイヤー
	player_->Update();
	//スカイドーム
	skydome_->Update();
	//フロア
	floorManager_->Update();
	//エネミー
	enemy_->Update();
	// ゴール
	goal_->Update();

	// 衝突処理
	collisionManager_->AllCollision();

	// デバッグ描画
	colliderDebugDraw_->Update();

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
	//3Dオブジェクトはここ
	player_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);
	floorManager_->Draw(viewProjection_);
	goal_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);

#ifdef _DEBUG

	// デバッグ描画
	colliderDebugDraw_->Draw(viewProjection_);

#endif // _DEBUG

	Model::PostDraw();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());
	

	//背景
	//前景スプライト描画

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::ImguiDraw(){

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
