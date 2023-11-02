#pragma once
#include "../base/DirectXCommon.h"
#include "../Audio/Audio.h"// サウンド再生
#include "../Input/Input.h"//入力デバイス
#include "../Camera/DebugCamera.h"//デバッグカメラ

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

	//床の生成のトランスフォームとか
	TransformStructure floorTransform_{};

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//デバッグカメラ
	std::unique_ptr<DebugCamera> debugCamera_ = nullptr;
	//フォローカメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;

	//光源
	std::unique_ptr<DirectionalLight> directionalLight;

	//自機
	std::unique_ptr<Player> player_;
	//自機のモデルとか
	std::vector<Model*> playerModels_;
	std::vector<Material*> playerMaterials_;

};
