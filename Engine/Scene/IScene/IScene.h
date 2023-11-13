#pragma once
#include "../../base/DirectXCommon.h"
#include "../../Audio/Audio.h"// サウンド再生
#include "../../Input/Input.h"//入力デバイス
#include "../../Camera/DebugCamera.h"//デバッグカメラ

#include "../../3D/WorldTransform.h"
#include "../../3D/ViewProjection.h"
#include "../../2D/Sprite.h"
#include "../../3D/Model.h"
#include "../../3D/Material.h"
#include "../../3D/DirectionalLight.h"
#include "../../3D/ParticleManager.h"

// シーン名
enum SceneName {
	kTitle, // タイトル
	kSelect,// セレクト
	kGame,  // ゲーム
	kClear, // クリア
	kCountOfSceneName, // カウント用。使用不可
};

/// <summary>
/// シーンの元になるクラス
/// </summary>
class IScene
{

protected: // 静的メンバ変数

	// シーン番号
	static int sceneNo;

	// 入力マネージャー
	static DirectXCommon* dxCommon_;
	static Input* input_;
	static Audio* audio_;

	//ビュープロジェクション
	static ViewProjection viewProjection_;

	//デバッグカメラ
	static std::unique_ptr<DebugCamera> debugCamera_;
	static bool isDebugCameraActive_;

	//光源
	static std::unique_ptr<DirectionalLight> directionalLight_;

public: // メンバ関数

	/// <summary>
	/// 静的初期化
	/// </summary>
	void StaticInitialize();

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	//デストラクタ
	virtual ~IScene();

	// シーン番号のゲッター
	int GetSceneNo();

protected:  // メンバ関数

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	virtual void ModelCreate();

	/// <summary>
	/// マテリアルクリエイト
	/// </summary>
	virtual void MaterialCreate();

	/// <summary>
	/// テクスチャロード
	/// </summary>
	virtual void TextureLoad();

};

