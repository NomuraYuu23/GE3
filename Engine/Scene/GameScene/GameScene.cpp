#include "GameScene.h"
#include "../../base/WinApp.h"
#include "../../base/TextureManager.h"
#include "../../2D/ImguiManager.h"
#include "../../base/D3DResourceLeakChecker.h"

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {


	//ModelCreate();
	//MaterialCreate();
	//TextureLoad();

	//// デバッグ描画
	//colliderDebugDraw_ = std::make_unique<ColliderDebugDraw>();
	//std::vector<Model*> colliderModels = { colliderSphereModel_.get(),colliderBoxModel_.get(),colliderBoxModel_.get() };
	//colliderDebugDraw_->Initialize(colliderModels, colliderMaterial_.get());

	//pause_ = std::make_unique<Pause>();
	//pause_->Initialize(pauseTextureHandles_);

	// ビュープロジェクション
	viewProjection_.transform_.translate = { 0.0f,23.0f,-35.0f };
	viewProjection_.transform_.rotate = { 0.58f,0.0f,0.0f };

	////パーティクル
	//particleManager_ = ParticleManager::GetInstance();
	//std::array<Model*, ParticleManager::ParticleModel::kCountofParticleModel> particleModel;
	//particleModel[ParticleManager::ParticleModel::kUvChecker] = particleUvcheckerModel_.get();
	//particleModel[ParticleManager::ParticleModel::kCircle] = particleCircleModel_.get();
	//particleManager_->ModelCreate(particleModel);
	//TransformStructure emitter = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{-3.0f,0.0f,0.0f} };
	//particleManager_->EmitterCreate(emitter,300.0f, ParticleManager::ParticleModel::kUvChecker);
	//emitter.translate.x = 3.0f;
	//particleManager_->EmitterCreate(emitter, 300.0f, ParticleManager::ParticleModel::kCircle);

	isDebugCameraActive_ = true;

	// MT4
	Vector3Calc* vector3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();
	

	q1 = {2.0f, 3.0f, 4.0f, 1.0f};
	q2 = {1.0f, 3.0f, 5.0f, 2.0f};
	identity = QuaternionCalc::IdentityQuaternion();
	conj = QuaternionCalc::Conjugate(q1);
	inv = QuaternionCalc::Inverse(q1);
	normal = QuaternionCalc::Normalize(q1);
	mul1 = QuaternionCalc::Multiply(q1,q2);
	mul2 = QuaternionCalc::Multiply(q2, q1);
	norm = QuaternionCalc::Norm(q1);

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
	directionalLight_->Update(directionalLightData);

	viewProjection_.UpdateMatrix();

	// デバッグカメラ
	DebugCameraUpdate();
	
	//// デバッグ描画
	//colliderDebugDraw_->Update();
	//
	////パーティクル
	//particleManager_->Update(debugCamera_->GetMatrix());

	//// ポーズ機能
	//pause_->Update();

	//// タイトルへ行く
	//GoToTheTitle();

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
	//3Dオブジェクトはここ

#ifdef _DEBUG

	// デバッグ描画
	//colliderDebugDraw_->Draw(viewProjection_);

#endif // _DEBUG

	Model::PostDraw();

#pragma region パーティクル描画
	Model::PreParticleDraw(dxCommon_->GetCommadList(), viewProjection_);

	//光源
	directionalLight_->Draw(dxCommon_->GetCommadList());

	// パーティクルはここ
	//particleManager_->Draw();

	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());
	

	//背景
	//前景スプライト描画
	//pause_->Draw();


	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::ImguiDraw(){
#ifdef _DEBUG
#endif // _DEBUG

	//MT4
	ImGui::Begin("MT01_03確認課題");
	ImGui::Text("%7.2f %7.2f %7.2f %7.2f	:Identity",identity.x, identity.y, identity.z, identity.w);
	ImGui::Text("%7.2f %7.2f %7.2f %7.2f	:Conjugate",conj.x, conj.y, conj.z, conj.w);
	ImGui::Text("%7.2f %7.2f %7.2f %7.2f	:Inverse", inv.x, inv.y, inv.z, inv.w);
	ImGui::Text("%7.2f %7.2f %7.2f %7.2f	:Normalize", normal.x, normal.y, normal.z, normal.w);
	ImGui::Text("%7.2f %7.2f %7.2f %7.2f	:Multiply(q1,q2)", mul1.x, mul1.y, mul1.z, mul1.w);
	ImGui::Text("%7.2f %7.2f %7.2f %7.2f	:Multiply(q2,q1)", mul2.x, mul2.y, mul2.z, mul2.w);
	ImGui::Text("%7.2f										:Norm", norm);
	ImGui::End();

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

void GameScene::GoToTheTitle()
{

	//if (pause_->GoToTheTitle()) {
	//	sceneNo = kTitle;
	//}

}

void GameScene::ModelCreate()
{

	//colliderSphereModel_.reset(Model::Create("Resources/TD2_November/collider/sphere/", "sphere.obj", dxCommon_));
	//colliderBoxModel_.reset(Model::Create("Resources/TD2_November/collider/box/", "box.obj", dxCommon_));
	//particleUvcheckerModel_.reset(Model::Create("Resources/default/", "plane.obj", dxCommon_));
	//particleCircleModel_.reset(Model::Create("Resources/Particle/", "plane.obj", dxCommon_));

}

void GameScene::MaterialCreate()
{

	//colliderMaterial_.reset(Material::Create());

}

void GameScene::TextureLoad()
{

	// ポーズ
	//pauseTextureHandles_ = {
	//	TextureManager::Load("Resources/TD2_November/pause/pausing.png", dxCommon_),
	//	TextureManager::Load("Resources/TD2_November/pause/goToTitle.png", dxCommon_),
	//	TextureManager::Load("Resources/TD2_November/pause/returnToGame.png", dxCommon_),
	//};

}
