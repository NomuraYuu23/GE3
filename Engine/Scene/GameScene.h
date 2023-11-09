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
#include"../../Program/CollisionManager/CollisionManager.h"
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
	
	//床の生成のトランスフォームとか
	TransformStructure floorTransform_{};

	//壊れるボックスの生成のトランスフォームとか
	TransformStructure breakBoxTransform_{};

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

};
