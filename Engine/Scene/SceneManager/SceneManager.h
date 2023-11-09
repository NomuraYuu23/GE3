#pragma once

#include "../IScene/IScene.h"
#include "../TitleScene/TitleScene.h"
#include "../GameScene/GameScene.h"
#include "../SceneFactory/SceneFactory.h"

/// <summary>
/// シーンマネージャー
/// </summary>
class SceneManager
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ変数

	// シーンファクトリー	
	SceneFactory* sceneFacyory_ = nullptr;

	// シーンを保持するメンバ変数
	std::unique_ptr<IScene> scene_;

	// どのステージを呼び出すかを管理する変数
	int currentSceneNo_; // 現在のシーン
	int prevSceneNo_; // 前のシーン

};

