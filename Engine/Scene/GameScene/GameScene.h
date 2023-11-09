#pragma once
#include "../IScene/IScene.h"
#include "../../Collider/ColliderDebugDraw/ColliderDebugDraw.h"// コライダーデバッグ描画

#include"../../../Program/FloorManager/FloorManager.h"
#include"../../../Program/Player/Player.h"
#include"../../../Program/CollisionManager/CollisionManager.h"
#include"../../../Engine/Camera/FollowCamera/FollowCamera.h"

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
	

public: //メンバ関数

	void DebugCameraUpdate();

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
	//床の生成のトランスフォームとか
	TransformStructure floorTransform_{};

	//フォローカメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	//自機
	std::unique_ptr<Player> player_;
	//自機のモデルとか
	std::vector<Model*> playerModels_;
	std::vector<Material*> playerMaterials_;

};
