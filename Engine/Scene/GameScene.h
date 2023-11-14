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
#include"../../Program/BoxManager/BoxManager.h"
#include"../../Program/BreakBoxManager/BreakBoxManager.h"
#include"../../Program/Player/Player.h"
#include"../../Program/EnemyManager/EnemyManager.h"
#include"../../Program/CollisionManager/CollisionManager.h"
#include"../../Program/RecoveryItemManager/RecoveryItemManager.h"
#include"../../Program/CollectibleItemManager/CollectibleItemManager.h"
#include"../../Engine/Camera/FollowCamera/FollowCamera.h"

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

	//コリジョンマネージャー
	std::unique_ptr<CollisionManager> collisionManager_;

	//床全体
	std::unique_ptr<FloorManager> floorManager_;
	std::unique_ptr<Model> floorModel_;
	std::unique_ptr<Material> floorMaterial_;
	bool isFloorMove_ = false;
	bool isVertical_ = false;

	//壁とかのブロック
	std::unique_ptr<BoxManager> boxManager_;
	std::unique_ptr<Model> boxModel_;
	std::unique_ptr<Material> boxMaterial_;
	bool isBoxMove_ = false;
	bool isBoxVertical_ = false;

	//壊れるブロック
	std::unique_ptr<BreakBoxManager> breakBoxManager_;
	std::unique_ptr<Model> breakBoxModel_;
	std::unique_ptr<Material> breakBoxMaterial_;
	bool isBreakBoxMove_ = false;
	bool isBreakBoxVertical_ = false;
	//爆発回数回復アイテム
	std::unique_ptr<RecoveryItemManager> recoveryItemManager_;
	std::unique_ptr<Model> recoveryItemModel_;
	std::unique_ptr<Material> recoveryItemMaterial_;
	int recoveryValue_ = 1;
	//コイン系収集アイテム
	std::unique_ptr<CollectibleItemManager> collectibleItemManager_;
	std::unique_ptr<Model> collectibleItemModel_;
	std::unique_ptr<Material> collectibleItemMaterial_;
	bool isCollectibleItemFall_ = false;
	
	//床の生成のトランスフォーム
	TransformStructure floorTransform_{};

	//ボックスのトランスフォーム
	TransformStructure firstBoxTrnasform_{
		.scale = {30.0f,3.0f,30.0f},
		.rotate = {0.0f,0.0f,0.0f},
		.translate = {0.0f,-5.0,35.0f}
	};

	//壊れるボックスの生成のトランスフォーム
	TransformStructure breakBoxTransform_{};

	//回復アイテムの生成トランスフォーム
	TransformStructure recoveryItemTransform_{};

	//収集アイテムの生成トランスフォーム
	TransformStructure collectibleItemTransform_{};

	//敵生成トランスフォーム
	TransformStructure firstEnemyTransform_{};
	TransformStructure enemyTransform_{};

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	bool isDebugCameraActive_;
	//フォローカメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	// デバッグ描画
	std::unique_ptr<ColliderDebugDraw> colliderDebugDraw_ = nullptr;
	std::unique_ptr<Model> colliderSphereModel_ = nullptr;
	std::unique_ptr<Model> colliderBoxModel_ = nullptr;
	std::unique_ptr<Material> colliderMaterial_ = nullptr;

	//光源
	std::unique_ptr<DirectionalLight> directionalLight;

	//自機
	std::unique_ptr<Player> player_;
	//自機のモデルとか
	std::vector<Model*> playerModels_;
	std::vector<Material*> playerMaterials_;

	//エネミー関連
	//エネミー
	std::unique_ptr<EnemyManager> enemyManager_;
	//エネミーのモデルとか
	std::vector<Model*> enemyModels_;
	std::vector<Material*> enemyMaterials_;
};
