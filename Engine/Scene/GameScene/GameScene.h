#pragma once
#include "../IScene/IScene.h"
#include "../../Collider/ColliderDebugDraw/ColliderDebugDraw.h"// コライダーデバッグ描画

#include"../../../Program/FloorManager/FloorManager.h"
#include"../../../Program/BoxManager/BoxManager.h"
#include"../../../Program/BreakBoxManager/BreakBoxManager.h"
#include"../../../Program/Player/Player.h"
#include"../../../Program/EnemyManager/EnemyManager.h"
#include"../../../Program/CollisionManager/CollisionManager.h"
#include"../../../Program/RecoveryItemManager/RecoveryItemManager.h"
#include"../../../Program/CollectibleItemManager/CollectibleItemManager.h"
#include"../../Camera/FollowCamera/FollowCamera.h"
#include "../../Pause/Pause.h"
#include"../../../Program/CheckPointManager/CheckPointManager.h"
#include"../../../Program/Goal/Goal.h"
#include "../../../Program/ShadowManager/ShadowManager.h"
#include "../../../Program/UI/UI.h"
#include "../../../Program/Skydome/Skydome.h"

class GameScene : public IScene
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

	/// <summary>
	/// ステージロード
	/// </summary>
	/// <param name="stageIndex"></param>
	void LoadStage(uint32_t stageIndex);

public: //メンバ関数

	/// <summary>
	/// デバッグカメラ更新
	/// </summary>
	void DebugCameraUpdate();

	/// <summary>
	/// タイトルへ行く
	/// </summary>
	void GoToTheTitle();

private: // メンバ関数

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	void ModelCreate() override;

	/// <summary>
	/// マテリアルクリエイト
	/// </summary>
	void MaterialCreate() override;

	/// <summary>
	/// テクスチャロード
	/// </summary>
	void TextureLoad() override;

	void FilesSave(const std::vector<std::string>& stages);

	void FilesOverWrite(const std::string& stage);

	void FilesLoad(const std::vector<std::string>& stages, const std::string& stage);

private:
	std::vector<std::string> stages_;

	std::string stageName_;

	int stageSelect_;

	//テスト
private:

	// デバッグ描画
	std::unique_ptr<ColliderDebugDraw> colliderDebugDraw_ = nullptr;
	std::unique_ptr<Model> colliderSphereModel_ = nullptr;
	std::unique_ptr<Model> colliderBoxModel_ = nullptr;
	std::unique_ptr<Material> colliderMaterial_ = nullptr;


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

	//チェックポイント
	std::unique_ptr<CheckPointManager> checkPointManager_;
	std::unique_ptr<Model> checkPointModel_;
	std::unique_ptr<Material> checkPointMaterial_;

	//床の生成のトランスフォーム
	TransformStructure floorTransform_{};

	//ボックスのトランスフォーム
	TransformStructure firstBoxTrnasform_{
		.scale = {30.0f,3.0f,30.0f},
		.rotate = {0.0f,0.0f,0.0f},
		.translate = {0.0f,-5.0,35.0f}
	};

	//壊れるボックスの生成のトランスフォーム
	TransformStructure breakBoxTransform_{ {1.0f,1.0f,1.0f} };

	//回復アイテムの生成トランスフォーム
	TransformStructure recoveryItemTransform_{ {1.0f,1.0f,1.0f} };

	//収集アイテムの生成トランスフォーム
	TransformStructure collectibleItemTransform_{ {1.0f,1.0f,1.0f} };

	//チェックポイントの生成トランスフォーム
	TransformStructure checkPointTransform_{ {1.0f,1.0f,1.0f} };
	TransformStructure firstCheckPointTransform_{
		.scale = {2.0f,2.0f,2.0f},
		.rotate = {0.0f,0.0f,0.0f},
		.translate = {0.0f,2.0f,75.0f}
	};

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

	//自機
	std::unique_ptr<Player> player_;
	//自機のモデルとか
	std::vector<Model*> playerModels_;
	std::vector<Material*> playerMaterials_;
	// ポーズ
	// テクスチャハンドル
	std::array<uint32_t, Pause::PauseTextureNo::kCountOfPauseTextureNo> pauseTextureHandles_;
	std::unique_ptr<Pause> pause_ = nullptr;

	// パーティクルマネージャー
	ParticleManager* particleManager_ = nullptr;
	std::unique_ptr<Model> particleUvcheckerModel_ = nullptr;
	std::unique_ptr<Model> particleCircleModel_ = nullptr;

	//エネミー関連
	//エネミー
	std::unique_ptr<EnemyManager> enemyManager_;
	//エネミーのモデルとか
	std::vector<Model*> enemyModels_;
	std::vector<Material*> enemyMaterials_;

	/*ゴール*/
	std::unique_ptr<Goal> goal_;
	//ゴールのモデル
	std::unique_ptr<Model> goalModel_;
	std::unique_ptr<Material> goalMaterial_;



	//影マネージャー
	ShadowManager* shadowManager_;
	std::unique_ptr<Model> shadowModel_ = nullptr;

	// UI
	std::unique_ptr<UI> ui;
	std::array<uint32_t, UI::kTextureHandleIndex::kCountOfTextureHandleIndex> uiTextureHandles_;

	// スカイドーム
	std::unique_ptr<Skydome> skydome_;
	std::unique_ptr<Model> skydomeModel_;
	std::unique_ptr<Material> skydomeMaterial_;

};
