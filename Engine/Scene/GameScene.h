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

//オブジェクト


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

};
