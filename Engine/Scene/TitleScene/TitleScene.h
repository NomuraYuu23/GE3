#pragma once
#include "../IScene/IScene.h"
#include <memory>

#include "../../../Program/Skydome/Skydome.h"

class TitleScene : public IScene
{

public: // サブクラス

	struct StartAnimationVariables
	{
		// 秒数
		uint32_t animationTime_;
		// カウント
		float animationParameter_;
		// 切り替え秒数
		float switchingTime_;
		// 切り替え追加秒数
		float switchingAddTime_;
		// アニメーションするか
		bool isAnimation_;
	};

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() override;

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

private:  // メンバ関数

	/// <summary>
	/// スタートアニメーション
	/// </summary>
	void StartAnimation();

private: // メンバ変数

	std::unique_ptr<Sprite> titleNameSprite_ = nullptr;
	uint32_t titleNameTextureHandle_ = 0u;
	Vector2 titleNamePosition_;
	Vector4 titleNameColor_;

	std::unique_ptr<Sprite> startSprite_ = nullptr;
	uint32_t startTextureHandle_ = 0u;
	Vector2 startPosition_;
	Vector4 startColor_;

	StartAnimationVariables startAnimationVariables_;

	// スカイドーム
	std::unique_ptr<Skydome> skydome_ = nullptr;
	std::unique_ptr<Model> skydomeModel_ = nullptr;
	std::unique_ptr<Material> skydomeMaterial_ = nullptr;

	// プレイヤー
	WorldTransform playerWorldTransform_;
	std::unique_ptr<Model> playerModel_ = nullptr;
	std::unique_ptr<Material> playerMaterial_ = nullptr;


	bool end_ = false;

};

