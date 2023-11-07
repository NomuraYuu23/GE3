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
GameScene::GameScene() {

}

/// <summary>
/// デストラクタ
/// </summary>
GameScene::~GameScene() {
	for (size_t i = 0; i < playerModels_.size(); i++) {
		delete playerModels_[i];
		delete playerMaterials_[i];
	}
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


	floorManager_ = std::make_unique<FloorManager>();

	floorMaterial_.reset(Material::Create());

	floorModel_.reset(Model::Create("Resources/AL4/floor/", "floor.obj", dxCommon_));

	floorManager_->Initialize(floorModel_.get(), floorMaterial_.get());

	floorManager_->SetColliderDebugDraw(colliderDebugDraw_.get());
	floorManager_->AddFloor({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, false, false);

	//ビュープロジェクション
	viewProjection_.Initialize();

	viewProjection_.transform_.translate = { 0.0f,23.0f,-35.0f };
	viewProjection_.transform_.rotate = { 0.58f,0.0f,0.0f };

	//デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();
	isDebugCameraActive_ = false;

	//フォローカメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	//光源
	directionalLight.reset(DirectionalLight::Create());

	//プレイヤー関連
	playerModels_.push_back(Model::Create("Resources/AL4/float_Body/", "float_Body.obj", dxCommon_));
	playerModels_.push_back(Model::Create("Resources/AL4/float_Head/", "float_Head.obj", dxCommon_));
	playerModels_.push_back(Model::Create("Resources/AL4/float_L_arm/", "float_L_arm.obj", dxCommon_));
	playerModels_.push_back(Model::Create("Resources/AL4/float_R_arm/", "float_R_arm.obj", dxCommon_));
	playerModels_.push_back(Model::Create("Resources/AL4/player_Weapon/", "player_Weapon.obj", dxCommon_));

	for (size_t i = 0; i < playerModels_.size(); i++) {
		playerMaterials_.push_back(Material::Create());
	}


	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels_, playerMaterials_);
	player_->SetViewProjection(followCamera_->GetViewProjectionAddress());

	followCamera_->SetTarget(player_->GetWorldTransformAddress());

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize(player_.get(), floorManager_.get());

	colliderDebugDraw_->AddCollider(&player_->GetCollider());

	/// aaaaa
	///bbbbb
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update() {
	ImguiDraw();
	//光源
	DirectionalLightData directionalLightData;
	directionalLightData.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData.direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData.intencity = 1.0f;
	directionalLight->Update(directionalLightData);



	floorManager_->Update();

	player_->Update();

	collisionManager_->AllCollision();

	followCamera_->Update();

	viewProjection_ = followCamera_->GetViewProjection();

	viewProjection_.UpdateMatrix();

	// デバッグカメラ
	DebugCameraUpdate();

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
	/*3Dオブジェクトはここ*/
	floorManager_->Draw(viewProjection_);
	player_->Draw(viewProjection_);

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

void GameScene::ImguiDraw() {

#ifdef _DEBUG

	ImGui::Begin("カメラ");
	ImGui::DragFloat3("カメラの座標", &viewProjection_.transform_.translate.x, 0.1f);
	ImGui::DragFloat3("カメラの回転", &viewProjection_.transform_.rotate.x, 0.01f);

	ImGui::End();

	ImGui::Begin("床の生成");
	ImGui::DragFloat3("床の座標", &floorTransform_.translate.x, 0.1f);
	ImGui::DragFloat3("床の回転", &floorTransform_.rotate.x, 0.01f);
	ImGui::Checkbox("動く床にする", &isFloorMove_);
	if (isFloorMove_) {
		ImGui::Checkbox("縦移動にする", &isVertical_);
	}
	else {
		isVertical_ = false;
	}
	if (ImGui::Button("床の追加")) {
		floorManager_->AddFloor(floorTransform_.translate, floorTransform_.rotate, isFloorMove_, isVertical_);
	}
	ImGui::End();

#endif // _DEBUG

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
