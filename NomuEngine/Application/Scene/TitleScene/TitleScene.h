#pragma once
#include "../../../Engine/Scene/IScene/IScene.h"
class TitleScene : public IScene
{

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


	/// <summary>
	/// スプライトImgui
	/// </summary>
	void SpriteImgui();

private:

	// スプライト
	std::unique_ptr<Sprite> sprite_;
	// テクスチャハンドル
	uint32_t textureHandle_;
	uint32_t textureHandle2_;

	// 色
	Vector4 spriteColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	// 反転X
	bool spriteIsFlipX_ = false;
	// 反転Y
	bool spriteIsFlipY_ = false;
	// 表示をしないか
	bool spriteIsInvisible_ = false;
	// 位置
	Vector2 spritePosition_;
	// 回転
	float spriteRotate_;
	// 大きさ
	Vector2 spriteSize_;
	// 左上
	Vector2 spriteTextureLeftTop_;
	// テクスチャサイズ
	Vector2 spriteTextureSize_;
	
	// 説明スプライト
	std::unique_ptr<Sprite> operationSprite_;

};

