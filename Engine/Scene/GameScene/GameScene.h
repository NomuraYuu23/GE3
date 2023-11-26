#pragma once
#include "../IScene/IScene.h"
#include "../../Collider/ColliderDebugDraw/ColliderDebugDraw.h"// コライダーデバッグ描画

#include"../../../Program/FloorManager/FloorManager.h"
#include"../../../Program/Player/Player.h"
#include"../../../Program/CollisionManager/CollisionManager.h"
#include"../../../Engine/Camera/FollowCamera/FollowCamera.h"
#include "../../Pause/Pause.h"
#include "../../../Program/Skydome/Skydome.h"
#include "../../../Program/EnemyManager/EnemyManager.h"
#include "../../../Program/LockOn/LockOn.h"

class GameScene : public IScene
{

public:

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

	/// <summary>
	/// デバッグカメラ更新
	/// </summary>
	void DebugCameraUpdate();

	/// <summary>
	/// タイトルへ行く
	/// </summary>
	void GoToTheTitle();

	/// <summary>
	/// リスタート
	/// </summary>
	void Restart();

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

private:

	// デバッグ描画
	std::unique_ptr<ColliderDebugDraw> colliderDebugDraw_ = nullptr;
	std::unique_ptr<Model> colliderSphereModel_ = nullptr;
	std::unique_ptr<Model> colliderBoxModel_ = nullptr;
	std::unique_ptr<Material> colliderMaterial_ = nullptr;

	// ポーズ
	// テクスチャハンドル
	std::array<uint32_t, Pause::PauseTextureNo::kCountOfPauseTextureNo> pauseTextureHandles_;
	std::unique_ptr<Pause> pause_ = nullptr;

	// パーティクルマネージャー
	ParticleManager* particleManager_ = nullptr;
	std::unique_ptr<Model> particleUvcheckerModel_ = nullptr;
	std::unique_ptr<Model> particleCircleModel_ = nullptr;

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
	std::unique_ptr<EnemyManager> enemyManager_;

	std::unique_ptr<Model> enemyBodyModel_ = nullptr;
	std::unique_ptr<Model> enemyL_armModel_ = nullptr;
	std::unique_ptr<Model> enemyR_armModel_ = nullptr;
	std::unique_ptr<Material> enemyBodyMaterial_ = nullptr;
	std::unique_ptr<Material> enemyL_armMaterial_ = nullptr;
	std::unique_ptr<Material> enemyR_armMaterial_ = nullptr;

	// 衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

	// ロックオン
	std::unique_ptr<LockOn> lockOn_;
	uint32_t lockOnTextureHandle_;

};
