#pragma once
#include "../IScene/IScene.h"
#include "../../../Program/UI/UIData.h"

class ClearScene : public IScene
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

private: 

	void Animation();

private:

	UIData backGround_;

	UIData goToSelect_;

	// アニメーションパラメータ
	float animationParameter_ = 0.0f;

	// アニメーションピリオド
	uint32_t animationPeriod_ = 60;

};
