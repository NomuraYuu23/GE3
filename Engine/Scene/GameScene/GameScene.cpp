#include "GameScene.h"
#include "../../base/WinApp.h"
#include "../../base/TextureManager.h"
#include "../../2D/ImguiManager.h"
#include "../../base/D3DResourceLeakChecker.h"

#include "../../../externals/imgui/imgui_impl_dx12.h"
#include "../../../externals/imgui/imgui_impl_win32.h"
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#include "../../base/WinApp.h"
#include "../../base/DirectXCommon.h"
#include "../../base/TextureManager.h"
#include "../../base/D3DResourceLeakChecker.h"
#include <vector>

#include "../SceneManager/SceneManager.h"

/// <summary>
/// コンストラクタ
/// </summary>
GameScene::GameScene(){
	
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


	ModelCreate();
	MaterialCreate();
	TextureLoad();

	// デバッグ描画
	colliderDebugDraw_ = std::make_unique<ColliderDebugDraw>();
	std::vector<Model*> colliderModels = { colliderSphereModel_.get(),colliderBoxModel_.get(),colliderBoxModel_.get() };
	colliderDebugDraw_->Initialize(colliderModels, colliderMaterial_.get());

	// 影
	shadowManager_ = ShadowManager::GetInstance();
	shadowManager_->Initialize(shadowModel_.get());
	shadowManager_->Reset();

	//フロアマネージャー
	floorManager_ = std::make_unique<FloorManager>();
	floorManager_->Initialize(floorModel_.get(), floorMaterial_.get());

	floorManager_->SetColliderDebugDraw(colliderDebugDraw_.get());
	floorManager_->AddFloor({ 0.0f,0.0f,0.0f }, { 0.0f,0.0f,0.0f }, false, false);
	floorManager_->AddFloor({ 0.0f,0.0f,75.0f }, { 0.0f,0.0f,0.0f }, false, false);

	//ボックスマネージャー
	boxManager_ = std::make_unique<BoxManager>();

	boxManager_->Initialize(boxModel_.get(), boxMaterial_.get());

	boxManager_->SetColliderDebugDraw(colliderDebugDraw_.get());
	boxManager_->AddBox(firstBoxTrnasform_, false, false);

	//壊れるボックス生成
	breakBoxManager_ = std::make_unique<BreakBoxManager>();

	breakBoxManager_->Initialize(breakBoxModel_.get(), breakBoxMaterial_.get());

	breakBoxManager_->SetColliderDebugDraw(colliderDebugDraw_.get());

	//回復アイテム生成
	recoveryItemManager_ = std::make_unique<RecoveryItemManager>();

	recoveryItemManager_->Initialize(recoveryItemModel_.get(), recoveryItemMaterial_.get());

	recoveryItemManager_->SetColliderDebugDraw(colliderDebugDraw_.get());

	//収集アイテム生成
	collectibleItemManager_ = std::make_unique<CollectibleItemManager>();

	collectibleItemManager_->Initialize(collectibleItemModel_.get(), collectibleItemMaterial_.get());

	collectibleItemManager_->SetColliderDebugDraw(colliderDebugDraw_.get());

	//チェックポイント生成
	checkPointManager_ = std::make_unique<CheckPointManager>();

	checkPointManager_->Initialize(checkPointModel_.get(), checkPointMaterial_.get());

	checkPointManager_->SetColliderDebugDraw(colliderDebugDraw_.get());
	checkPointManager_->AddCheck(firstCheckPointTransform_);

	pause_ = std::make_unique<Pause>();
	pause_->Initialize(pauseTextureHandles_);

	//ビュープロジェクション
	viewProjection_.Initialize();

	viewProjection_.transform_.translate = { 0.0f,23.0f,-35.0f };
	viewProjection_.transform_.rotate = { 0.58f,0.0f,0.0f };
	
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();

	//フォローカメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();

	goal_ = std::make_unique<Goal>();
	goal_->Initialize(goalModel_.get(), goalMaterial_.get());

	//エネミー関連
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

	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels_, playerMaterials_, sceneManager_->GetRespawnPosition());
	player_->SetViewProjection(followCamera_->GetViewProjectionAddress());
	Vector3 playerSize = {
		player_->GetColliderRadius() * 2.0f,
		player_->GetColliderRadius() * 2.0f,
		player_->GetColliderRadius() * 2.0f
	};
	shadowManager_->AddMeker(player_->GetWorldTransformAddress(), playerSize);
	//enemyManager_->SetPlayer(player_.get());

	followCamera_->SetTarget(player_->GetWorldTransformAddress());
	player_->SetFollowCamera(followCamera_.get());

	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize(player_.get(), floorManager_.get(), boxManager_.get(),
		breakBoxManager_.get(), recoveryItemManager_.get(), enemyManager_.get(),
		collectibleItemManager_.get(), checkPointManager_.get(), goal_.get());

	colliderDebugDraw_->AddCollider(&player_->GetCollider());
	colliderDebugDraw_->AddCollider(&player_->GetExplosionCollider());
	//ステージ識別番号
	stages_ = {
		"Stage1",
		"Stage2",
		"Stage3",
		"Stage4"
	};

	stageName_ = stages_[0].c_str();

	//パーティクル
	particleManager_ = ParticleManager::GetInstance();
	std::array<Model*, ParticleManager::ParticleModel::kCountofParticleModel> particleModel;
	particleModel[ParticleManager::ParticleModel::kUvChecker] = particleUvcheckerModel_.get();
	particleModel[ParticleManager::ParticleModel::kCircle] = particleCircleModel_.get();
	particleManager_->ModelCreate(particleModel);

	isDebugCameraActive_ = false;

	// ui
	ui = std::make_unique<UI>();
	ui->Initialize(uiTextureHandles_);
	ui->SetPlayer(player_.get());

	// スカイドーム
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get(), skydomeMaterial_.get());

}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update() {
	ImguiDraw();

	if (!pause_->IsPause()) {
		//光源
		DirectionalLightData directionalLightData{};
		directionalLightData.color = { 1.0f,1.0f,1.0f,1.0f };
		directionalLightData.direction = { 0.0f, -1.0f, 0.0f };
		directionalLightData.intencity = 1.0f;
		directionalLight_->Update(directionalLightData);

		floorManager_->Update();
		boxManager_->Update();
		breakBoxManager_->Update();

		checkPointManager_->Update();

		goal_->Update();

		player_->Update();

		collisionManager_->AllCollision();

		// デバッグ描画
		colliderDebugDraw_->Update();

		enemyManager_->Update();

		recoveryItemManager_->Update();

		collectibleItemManager_->Update();

		followCamera_->Update();

		viewProjection_ = followCamera_->GetViewProjection();

		viewProjection_.UpdateMatrix();

		// デバッグカメラ
		DebugCameraUpdate();

		// デバッグ描画
		colliderDebugDraw_->Update();

		//パーティクル
		particleManager_->Update(followCamera_->GetMatrix());

		// 影
		shadowManager_->Update();

		//ui
		ui->Update();
	}


	// ポーズ機能
	pause_->Update();

	if (player_->GetIsGoal()){
		requestSeneNo = kClear;
		Audio::GetInstance()->PlayWave(Audio::AudioHandleIndex::kGoal, false, 1.0f);
	}

	if (player_->GetExprosionNumInt() < 0) {
		requestSeneNo = kGameOver;
		sceneManager_->SetRespawnPosition(player_->GetInitialPosition());
		sceneManager_->SetRespawnItem(player_->GetNumCollectItem());
	}

	// タイトルへ行く
	GoToTheTitle();

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
	directionalLight_->Draw(dxCommon_->GetCommadList());
	/*3Dオブジェクトはここ*/
	boxManager_->Draw(viewProjection_);
	breakBoxManager_->Draw(viewProjection_);
	floorManager_->Draw(viewProjection_);
	checkPointManager_->Draw(viewProjection_);

	goal_->Draw(viewProjection_);
	
	recoveryItemManager_->Draw(viewProjection_);
	collectibleItemManager_->Draw(viewProjection_);
	player_->Draw(viewProjection_);
	enemyManager_->Draw(viewProjection_);
	shadowManager_->Draw(viewProjection_);

	skydome_->Draw(viewProjection_);

#ifdef _DEBUG

	// デバッグ描画
	colliderDebugDraw_->Draw(viewProjection_);

#endif // _DEBUG

	Model::PostDraw();

#pragma region パーティクル描画
	Model::PreParticleDraw(dxCommon_->GetCommadList(), viewProjection_);

	//光源
	directionalLight_->Draw(dxCommon_->GetCommadList());

	// パーティクルはここ
	particleManager_->Draw();

	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());


	//背景
	//前景スプライト描画

	//ui
	ui->Draw();

	pause_->Draw();

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

	goal_->DrawImgui();

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
			if (ImGui::TreeNode("敵生成")) {
				ImGui::DragFloat3("敵の座標", &firstEnemyTransform_.translate.x, 0.1f);
				ImGui::DragFloat3("敵の回転", &firstEnemyTransform_.rotate.x, 0.01f);
				ImGui::DragFloat3("敵の大きさ", &firstEnemyTransform_.scale.x, 0.1f, 0.1f, 999.0f, "%.1f");

				if (ImGui::Button("敵の追加")) {
					enemyManager_->AddEnemy(firstEnemyTransform_);
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
			if (ImGui::BeginMenu("敵一覧")) {
				enemyManager_->DrawImgui();
				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("ファイル関連")){
			for (size_t i = 0; i < stages_.size(); i++){
				if (ImGui::RadioButton(stages_[i].c_str(), &stageSelect_, static_cast<int>(i))) {
					stageName_ = stages_[stageSelect_].c_str();
				}
				
			}
			if (ImGui::Button("jsonファイルを作る")) {
				FilesSave(stages_);
			}
			if (ImGui::Button("上書きセーブ")) {
				FilesOverWrite(stageName_);
			}

			if (ImGui::Button("全ロード(手動)")) {
				FilesLoad(stages_,stageName_);
			}
			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
	ImGui::End();

	

	
#endif // _DEBUG

}

void GameScene::LoadStage(uint32_t stageIndex)
{

	stageName_ = stages_[stageIndex].c_str();

	FilesLoad(stages_, stageName_);

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

void GameScene::FilesSave(const std::vector<std::string>& stages){
	goal_->SaveFile(stages);
	enemyManager_->SaveFile(stages);
	/*floorManager_->SaveFile(stages);
	boxManager_->SaveFile(stages);
	breakBoxManager_->SaveFile(stages);
	checkPointManager_->SaveFile(stages);
	collectibleItemManager_->SaveFile(stages);
	recoveryItemManager_->SaveFile(stages);*/
	std::string message = std::format("{}.json created.", "all");
	MessageBoxA(nullptr, message.c_str(), "StagesObject", 0);
}

void GameScene::FilesOverWrite(const std::string& stage){
	enemyManager_->FileOverWrite(stage);
	floorManager_->FileOverWrite(stage);
	boxManager_->FileOverWrite(stage);
	breakBoxManager_->FileOverWrite(stage);
	checkPointManager_->FileOverWrite(stage);
	collectibleItemManager_->FileOverWrite(stage);
	recoveryItemManager_->FileOverWrite(stage);
	goal_->FileOverWrite(stage);
	std::string message = std::format("{}.json OverWrite.", "all");
	MessageBoxA(nullptr, message.c_str(), "StagesObject", 0);
}

void GameScene::FilesLoad(const std::vector<std::string>& stages, const std::string& stage){
	enemyManager_->LoadFiles(stage);
	floorManager_->LoadFiles(stage);
	boxManager_->LoadFiles(stage);
	breakBoxManager_->LoadFiles(stage);
	checkPointManager_->LoadFiles(stage);
	collectibleItemManager_->LoadFiles(stage);
	recoveryItemManager_->LoadFiles(stage);
	goal_->LoadFiles(stage);
	std::string message = std::format("{}.json loaded.", "all");
	//MessageBoxA(nullptr, message.c_str(), "StagesObject", 0);
}

void GameScene::GoToTheTitle()
{

	if (pause_->GoToTheSelect()) {
		requestSeneNo = kSelect;
	}

}

void GameScene::ModelCreate()
{
	// デバッグ描画
	colliderSphereModel_.reset(Model::Create("Resources/TD2_November/collider/sphere/", "sphere.obj", dxCommon_));
	colliderBoxModel_.reset(Model::Create("Resources/TD2_November/collider/box/", "box.obj", dxCommon_));
	//フロアマネージャー
	floorModel_.reset(Model::Create("Resources/AL4/floor/", "floor.obj", dxCommon_));
	//プレイヤー関連
	playerModels_.push_back(Model::Create("Resources/TD2_November/Player/playerBody", "playerBody.obj", dxCommon_));
	playerModels_.push_back(Model::Create("Resources/TD2_November/Player/playerLeftLeg", "playerLeftLeg.obj", dxCommon_));
	playerModels_.push_back(Model::Create("Resources/TD2_November/Player/playerRightLeg", "playerRightLeg.obj", dxCommon_));
	playerModels_.push_back(Model::Create("Resources/TD2_November/exprode/", "sphere.obj", dxCommon_));
	//ボックスマネージャー
	boxModel_.reset(Model::Create("Resources/TD2_November/block/", "block.obj", dxCommon_));
	//壊れるボックス生成
	breakBoxModel_.reset(Model::Create("Resources/TD2_November/breakBox/", "box.obj", dxCommon_));
	//回復アイテム生成
	recoveryItemModel_.reset(Model::Create("Resources/TD2_November/bombCherry/", "bombCherry.obj", dxCommon_));
	//収集アイテム生成
	collectibleItemModel_.reset(Model::Create("Resources/TD2_November/emerald/", "emerald.obj", dxCommon_));
	//チェックポイント生成
	checkPointModel_.reset(Model::Create("Resources/TD2_November/torch/", "torch.obj", dxCommon_));
	//エネミー関連
	enemyModels_.push_back(Model::Create("Resources/TD2_November/enemy/", "bombEnemy.obj", dxCommon_));
	//enemyModels_.push_back(Model::Create("Resources/AL4/enemy_Arm/", "enemy_Arm.obj", dxCommon_));
	//enemyModels_.push_back(Model::Create("Resources/AL4/enemy_Arm/", "enemy_Arm.obj", dxCommon_));
	//ゴール関連
	goalModel_.reset(Model::Create("Resources/TD2_November/goal/", "box.obj", dxCommon_));


	// パーティクルモデル
	particleUvcheckerModel_.reset(Model::Create("Resources/default/", "plane.obj", dxCommon_));
	particleCircleModel_.reset(Model::Create("Resources/Particle/", "plane.obj", dxCommon_));

	// 影モデル
	shadowModel_.reset(Model::Create("Resources/TD2_November/shadow/", "shadow.obj", dxCommon_));

	// スカイドーム
	skydomeModel_.reset(Model::Create("Resources/TD2_November/skydome/", "skydome.obj", dxCommon_));

}

void GameScene::MaterialCreate()
{
	// デバッグ描画
	colliderMaterial_.reset(Material::Create());
	//フロアマネージャー
	floorMaterial_.reset(Material::Create());
	//プレイヤー関連
	for (size_t i = 0; i < playerModels_.size(); i++) {
		playerMaterials_.push_back(Material::Create());
	}
	//ボックスマネージャー
	boxMaterial_.reset(Material::Create());
	//壊れるボックス生成
	breakBoxMaterial_.reset(Material::Create());
	//回復アイテム生成
	recoveryItemMaterial_.reset(Material::Create());
	//収集アイテム生成
	collectibleItemMaterial_.reset(Material::Create());
	//チェックポイント生成
	checkPointMaterial_.reset(Material::Create());
	//エネミー関連
	for (size_t i = 0; i < enemyModels_.size(); i++) {
		enemyMaterials_.push_back(Material::Create());
	}
	goalMaterial_.reset(Material::Create());

	// スカイドーム
	skydomeMaterial_.reset(Material::Create());

}

void GameScene::TextureLoad()
{

	// ポーズ
	pauseTextureHandles_ = {
		TextureManager::Load("Resources/TD2_November/pause/pausing.png", dxCommon_),
		TextureManager::Load("Resources/TD2_November/pause/goToSelect.png", dxCommon_),
		TextureManager::Load("Resources/TD2_November/pause/returnToGame.png", dxCommon_),
	};

	// ui
	uiTextureHandles_ = {
		TextureManager::Load("Resources/TD2_November/UI/buttonA.png", dxCommon_),
		TextureManager::Load("Resources/TD2_November/UI/stickL.png", dxCommon_),
		TextureManager::Load("Resources/TD2_November/UI/buttonPause.png", dxCommon_),
		TextureManager::Load("Resources/TD2_November/UI/stickR.png", dxCommon_),

		TextureManager::Load("Resources/TD2_November/UI/jumpOperation.png", dxCommon_),
		TextureManager::Load("Resources/TD2_November/UI/moveOperation.png", dxCommon_),
		TextureManager::Load("Resources/TD2_November/UI/pauseOperation.png", dxCommon_),
		TextureManager::Load("Resources/TD2_November/UI/cameraOperation.png", dxCommon_),

		TextureManager::Load("Resources/TD2_November/UI/remainingExplosion.png", dxCommon_),
		TextureManager::Load("Resources/TD2_November/UI/num.png", dxCommon_),
		TextureManager::Load("Resources/TD2_November/UI/num.png", dxCommon_),
		TextureManager::Load("Resources/TD2_November/UI/num.png", dxCommon_),

		TextureManager::Load("Resources/TD2_November/UI/emerald.png", dxCommon_),
		TextureManager::Load("Resources/TD2_November/UI/x.png", dxCommon_),
		TextureManager::Load("Resources/TD2_November/UI/num.png", dxCommon_),
		TextureManager::Load("Resources/TD2_November/UI/num.png", dxCommon_),

	};

}
