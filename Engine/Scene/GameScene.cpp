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

	//フロアマネージャー
	floorManager_ = std::make_unique<FloorManager>();

	floorMaterial_.reset(Material::Create());

	floorModel_.reset(Model::Create("Resources/AL4/floor/", "floor.obj", dxCommon_));

	floorManager_->Initialize(floorModel_.get(), floorMaterial_.get());

	floorManager_->SetColliderDebugDraw(colliderDebugDraw_.get());
	floorManager_->AddFloor({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, false, false);
	floorManager_->AddFloor({ 0.0f,0.0f,75.0f }, { 0.0f,0.0f,0.0f }, false, false);

	//ボックスマネージャー
	boxManager_ = std::make_unique<BoxManager>();
	boxMaterial_.reset(Material::Create());

	boxModel_.reset(Model::Create("Resources/TD2_November/floorBox/", "box.obj", dxCommon_));

	boxManager_->Initialize(boxModel_.get(), boxMaterial_.get());

	boxManager_->SetColliderDebugDraw(colliderDebugDraw_.get());
	boxManager_->AddBox(firstBoxTrnasform_, false, false);

	//壊れるボックス生成
	breakBoxManager_ = std::make_unique<BreakBoxManager>();
	breakBoxMaterial_.reset(Material::Create());

	breakBoxModel_.reset(Model::Create("Resources/TD2_November/breakBox/", "box.obj", dxCommon_));

	breakBoxManager_->Initialize(breakBoxModel_.get(), breakBoxMaterial_.get());

	breakBoxManager_->SetColliderDebugDraw(colliderDebugDraw_.get());

	//回復アイテム生成
	recoveryItemManager_ = std::make_unique<RecoveryItemManager>();
	recoveryItemMaterial_.reset(Material::Create());
	recoveryItemModel_.reset(Model::Create("Resources/TD2_November/recoveryItem/", "Bullet.obj", dxCommon_));

	recoveryItemManager_->Initialize(recoveryItemModel_.get(), recoveryItemMaterial_.get());

	recoveryItemManager_->SetColliderDebugDraw(colliderDebugDraw_.get());

	//収集アイテム生成
	collectibleItemManager_ = std::make_unique<CollectibleItemManager>();
	collectibleItemMaterial_.reset(Material::Create());
	collectibleItemModel_.reset(Model::Create("Resources/TD2_November/collectibleItem/", "box.obj", dxCommon_));

	collectibleItemManager_->Initialize(collectibleItemModel_.get(), collectibleItemMaterial_.get());

	collectibleItemManager_->SetColliderDebugDraw(colliderDebugDraw_.get());

	//チェックポイント生成
	checkPointManager_ = std::make_unique<CheckPointManager>();
	checkPointMaterial_.reset(Material::Create());
	checkPointModel_.reset(Model::Create("Resources/TD2_November/checkPoint/", "box.obj", dxCommon_));

	checkPointManager_->Initialize(checkPointModel_.get(), checkPointMaterial_.get());

	checkPointManager_->SetColliderDebugDraw(colliderDebugDraw_.get());
	checkPointManager_->AddCheck(firstCheckPointTransform_);

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

	//エネミー関連
	enemyModels_.push_back(Model::Create("Resources/AL4/enemy_Body/", "enemy_Body.obj", dxCommon_));
	enemyModels_.push_back(Model::Create("Resources/AL4/enemy_Arm/", "enemy_Arm.obj", dxCommon_));
	enemyModels_.push_back(Model::Create("Resources/AL4/enemy_Arm/", "enemy_Arm.obj", dxCommon_));
	for (size_t i = 0; i < enemyModels_.size(); i++) {
		enemyMaterials_.push_back(Material::Create());
	}
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Initialize(enemyModels_, enemyMaterials_);
	enemyManager_->SetColliderDebugDraw(colliderDebugDraw_.get());
	firstEnemyTransform_ = {
		.scale = {1.0f,1.0f,1.0f},
		.rotate = {0.0f,0.0f,0.0f},
		.translate = {0.0f,0.0f,50.0f},
	};
	enemyManager_->AddEnemy(firstEnemyTransform_);

	//プレイヤー関連
	playerModels_.push_back(Model::Create("Resources/AL4/float_Body/", "float_Body.obj", dxCommon_));
	playerModels_.push_back(Model::Create("Resources/AL4/float_Head/", "float_Head.obj", dxCommon_));
	playerModels_.push_back(Model::Create("Resources/AL4/float_L_arm/", "float_L_arm.obj", dxCommon_));
	playerModels_.push_back(Model::Create("Resources/AL4/float_R_arm/", "float_R_arm.obj", dxCommon_));
	playerModels_.push_back(Model::Create("Resources/AL4/player_Weapon/", "player_Weapon.obj", dxCommon_));
	playerModels_.push_back(Model::Create("Resources/TD2_November/exprode/", "sphere.obj", dxCommon_));

	for (size_t i = 0; i < playerModels_.size(); i++) {
		playerMaterials_.push_back(Material::Create());
	}


	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels_, playerMaterials_);
	player_->SetViewProjection(followCamera_->GetViewProjectionAddress());

	followCamera_->SetTarget(player_->GetWorldTransformAddress());

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize(player_.get(), floorManager_.get(), boxManager_.get(), 
		breakBoxManager_.get(), recoveryItemManager_.get(), enemyManager_.get(), 
		collectibleItemManager_.get(), checkPointManager_.get());

	colliderDebugDraw_->AddCollider(&player_->GetCollider());
	colliderDebugDraw_->AddCollider(&player_->GetExplosionCollider());
	//ステージ識別番号
	stages = {
		"Stage1",
		"Stage2",
		"Stage3",
		"Stage4"
	};
}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update() {
	ImguiDraw();

	//光源
	DirectionalLightData directionalLightData{};
	directionalLightData.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData.direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData.intencity = 1.0f;
	directionalLight->Update(directionalLightData);



	floorManager_->Update();
	boxManager_->Update();
	breakBoxManager_->Update();
	
	checkPointManager_->Update();

	collisionManager_->AllCollision();

	// デバッグ描画
	colliderDebugDraw_->Update();

	player_->Update();

	enemyManager_->Update();

	recoveryItemManager_->Update();

	collectibleItemManager_->Update();

	followCamera_->Update();

	viewProjection_ = followCamera_->GetViewProjection();

	viewProjection_.UpdateMatrix();

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
	/*3Dオブジェクトはここ*/
	boxManager_->Draw(viewProjection_);
	breakBoxManager_->Draw(viewProjection_);
	floorManager_->Draw(viewProjection_);
	checkPointManager_->Draw(viewProjection_);
	
	recoveryItemManager_->Draw(viewProjection_);
	collectibleItemManager_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	enemyManager_->Draw(viewProjection_);

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

	ImGui::Begin("FPS");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();

	ImGui::Begin("カメラ");
	ImGui::DragFloat3("カメラの座標", &viewProjection_.transform_.translate.x, 0.1f);
	ImGui::DragFloat3("カメラの回転", &viewProjection_.transform_.rotate.x, 0.01f);

	ImGui::End();

	ImGui::Begin("ステージ関連", nullptr, ImGuiWindowFlags_MenuBar);

	if (ImGui::BeginMenuBar()){
		if (ImGui::BeginMenu("オブジェクトの生成")){
			if (ImGui::TreeNode("床生成")) {
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
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("ボックス生成")) {
				ImGui::DragFloat3("ボックスの座標", &floorTransform_.translate.x, 0.1f);
				ImGui::DragFloat3("ボックスの回転", &floorTransform_.rotate.x, 0.01f);
				ImGui::DragFloat3("ボックスの大きさ", &floorTransform_.scale.x, 0.1f, 0.1f, 999.0f, "%.1f");
				ImGui::Checkbox("動くボックスにする", &isFloorMove_);
				if (isFloorMove_) {
					ImGui::Checkbox("縦移動にする", &isVertical_);
				}
				else {
					isVertical_ = false;
				}
				if (ImGui::Button("ボックスの追加")) {
					boxManager_->AddBox(floorTransform_, isFloorMove_, isVertical_);
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("壊れるボックス生成")) {
				ImGui::DragFloat3("壊れるボックスの座標", &breakBoxTransform_.translate.x, 0.1f);
				ImGui::DragFloat3("壊れるボックスの回転", &breakBoxTransform_.rotate.x, 0.01f);
				ImGui::DragFloat3("壊れるボックスの大きさ", &breakBoxTransform_.scale.x, 0.1f, 0.1f, 999.0f, "%.1f");
				ImGui::Checkbox("動いて壊れるボックスにする", &isBreakBoxMove_);
				if (isBreakBoxMove_) {
					ImGui::Checkbox("縦移動にする", &isBreakBoxVertical_);
				}
				else {
					isVertical_ = false;
				}
				if (ImGui::Button("壊れるボックスの追加")) {
					breakBoxManager_->AddBox(breakBoxTransform_, isBreakBoxMove_, isBreakBoxVertical_);
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("回復アイテム生成")) {
				ImGui::DragFloat3("アイテムの座標", &recoveryItemTransform_.translate.x, 0.1f);
				ImGui::DragFloat3("アイテムの回転", &recoveryItemTransform_.rotate.x, 0.01f);
				ImGui::DragFloat3("アイテムの大きさ", &recoveryItemTransform_.scale.x, 0.1f, 0.1f, 999.0f, "%.1f");
				ImGui::DragInt("アイテムの回復値", &recoveryValue_, 1.0f, 1, 99);
				if (ImGui::Button("アイテムの追加")) {
					recoveryItemManager_->AddItem(recoveryItemTransform_, recoveryValue_);
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("収集アイテム生成")) {
				ImGui::DragFloat3("アイテムの座標", &collectibleItemTransform_.translate.x, 0.1f);
				ImGui::DragFloat3("アイテムの回転", &collectibleItemTransform_.rotate.x, 0.01f);
				ImGui::DragFloat3("アイテムの大きさ", &collectibleItemTransform_.scale.x, 0.1f, 0.1f, 999.0f, "%.1f");
				ImGui::Checkbox("落下させるか", &isCollectibleItemFall_);
				
				if (ImGui::Button("収集アイテムの追加")) {
					collectibleItemManager_->AddItem(collectibleItemTransform_, isCollectibleItemFall_);
				}
				ImGui::TreePop();
			}

			if (ImGui::TreeNode("チェックポイント生成")) {
				ImGui::DragFloat3("チェックポイントの座標", &checkPointTransform_.translate.x, 0.1f);
				ImGui::DragFloat3("チェックポイントの回転", &checkPointTransform_.rotate.x, 0.01f);
				ImGui::DragFloat3("チェックポイントの大きさ", &checkPointTransform_.scale.x, 0.1f, 0.1f, 999.0f, "%.1f");

				if (ImGui::Button("チェックポイントの追加")) {
					checkPointManager_->AddCheck(checkPointTransform_);
				}
				ImGui::TreePop();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("オブジェクト一覧")) {
			if (ImGui::BeginMenu("床一覧")) {
				floorManager_->DrawImgui();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("ボックス一覧")) {
				boxManager_->DrawImgui();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("壊れる箱一覧")) {
				breakBoxManager_->DrawImgui();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("回復アイテム一覧")) {
				recoveryItemManager_->DrawImgui();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("収集アイテム一覧")) {
				collectibleItemManager_->DrawImgui();
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("チェックポイント一覧")) {
				checkPointManager_->DrawImgui();
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("ファイル関連")){
			if (ImGui::Button("全保存")) {
				FilesSave();
			}
			if (ImGui::Button("全ロード(手動)")) {
				FilesLoad();
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();

	

	
#endif // _DEBUG

}

void GameScene::DebugCameraUpdate()
{

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_LSHIFT)) {
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

void GameScene::FilesSave(){
	floorManager_->SaveFile();
	boxManager_->SaveFile();
	breakBoxManager_->SaveFile();
	checkPointManager_->SaveFile();
	collectibleItemManager_->SaveFile();
	recoveryItemManager_->SaveFile();
	std::string message = std::format("{}.json saved.", "all");
	MessageBoxA(nullptr, message.c_str(), "StagesObject", 0);
}

void GameScene::FilesLoad(){
	boxManager_->LoadFiles();
	floorManager_->LoadFiles();
	breakBoxManager_->LoadFiles();
	checkPointManager_->LoadFiles();
	collectibleItemManager_->LoadFiles();
	recoveryItemManager_->LoadFiles();
	std::string message = std::format("{}.json loaded.", "all");
	MessageBoxA(nullptr, message.c_str(), "StagesObject", 0);
}
