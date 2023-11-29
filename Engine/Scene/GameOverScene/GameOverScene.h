#pragma once
#include "../IScene/IScene.h"
#include "../../../Program/UI/UIData.h"

class GameOverScene : public IScene
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

	void SelectChange();

	void Decision();

	void Animation();

private: // メンバ変数

	UIData backGround_;

	UIData goToSelect_;

	UIData respawn_;

	UIData respawnItem_;
	UIData respawnTen_;
	UIData respawnOne_;

	UIData haveRespawnItem_;
	UIData haveRespawnTen_;
	UIData haveRespawnOne_;

	UIData getItem_;

	// 操作説明スプライト
	UIData decisionOperation_;
	UIData moveOperation_;

	UIData decisionButton_;
	UIData moveButton_;

	bool isRespawn_ = false;
	
	// 選択クールタイム
	float selectCooltime_ = 0.3f;

	// 選択クールタイム
	float selectElapsedCooltime_ = 0.0f;

	// リスポーンの必要数
	uint32_t respawnConditions = 20;

	// リスポーンできるか
	bool canRespawn = false;

	// アニメーションパラメータ
	float animationParameter_ = 0.0f;

	// アニメーションピリオド
	uint32_t animationPeriod_ = 60;

	bool isDecided_ = false;

};

