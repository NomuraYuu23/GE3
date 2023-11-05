#pragma once
#include "../base/DirectXCommon.h"
#include "../Audio/Audio.h"// サウンド再生
#include "../Input/Input.h"//入力デバイス
#include "../Camera/DebugCamera.h"//デバッグカメラ
#include "../Collider/ColliderDebugDraw/ColliderDebugDraw.h"// コライダーデバッグ描画

#include "../3D/WorldTransform.h"
#include "../3D/ViewProjection.h"
#include "../2D/Sprite.h"
#include "../3D/Model.h"
#include "../3D/Material.h"
#include "../3D/DirectionalLight.h"
#include"../../Program/FloorManager/FloorManager.h"
#include"../../Program/Player/Player.h"
#include"../../Program/CollisionManager/CollisionManager.h"
#include"../../Engine/Camera/FollowCamera/FollowCamera.h"
#include "../../Program/Skydome/Skydome.h"

class GameScene
{

public:

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// imgui描画処理
	/// </summary>
	void ImguiDraw();
	

public: //メンバ関数

	void DebugCameraUpdate();

	//テスト
private:

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	bool isDebugCameraActive_;

	// デバッグ描画
	std::unique_ptr<ColliderDebugDraw> colliderDebugDraw_ = nullptr;
	std::unique_ptr<Model> colliderSphereModel_ = nullptr;
	std::unique_ptr<Model> colliderBoxModel_ = nullptr;
	std::unique_ptr<Material> colliderMaterial_ = nullptr;

	//光源
	std::unique_ptr<DirectionalLight> directionalLight;

	//Player
	std::unique_ptr<Player> player_ = nullptr;
	//マテリアル
	std::unique_ptr<Material> playerMaterial_ = nullptr;
	//モデル
	std::unique_ptr<Model> playerBodyModel_ = nullptr;
	std::unique_ptr<Model> playerHeadModel_ = nullptr;
	std::unique_ptr<Model> playerL_armModel_ = nullptr;
	std::unique_ptr<Model> playerR_armModel_ = nullptr;
	std::unique_ptr<Model> playerWeaponModel_ = nullptr;

	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	//スカイドーム
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//マテリアル
	std::unique_ptr<Material> skydomeMaterial_ = nullptr;
	//モデル
	std::unique_ptr<Model> skydomeModel_ = nullptr;

	// フロアマネージャー
	std::unique_ptr<FloorManager> floorManager_ = nullptr;
	//マテリアル
	std::unique_ptr<Material> floorMaterial_ = nullptr;
	//モデル
	std::unique_ptr<Model> floorModel_ = nullptr;

	// ゴール
	std::unique_ptr<Goal> goal_ = nullptr;
	//マテリアル
	std::unique_ptr<Material> goalMaterial_ = nullptr;
	//モデル
	std::unique_ptr<Model> goalModel_ = nullptr;

	//エネミー
	std::unique_ptr<Enemy> enemy_;

	std::unique_ptr<Model> enemyBodyModel_ = nullptr;
	std::unique_ptr<Model> enemyL_armModel_ = nullptr;
	std::unique_ptr<Model> enemyR_armModel_ = nullptr;
	std::unique_ptr<Material> enemyBodyMaterial_ = nullptr;
	std::unique_ptr<Material> enemyL_armMaterial_ = nullptr;
	std::unique_ptr<Material> enemyR_armMaterial_ = nullptr;


	// 衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

};
