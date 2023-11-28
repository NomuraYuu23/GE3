#pragma once
#include "../../Engine/2D/Sprite.h"
#include "../../Engine/Input/Input.h"
#include <memory>
#include <array>

class Player;

class UI
{

public: // サブクラス

	// テクスチャハンドル番号
	enum kTextureHandleIndex {
		kJumpButton,
		kMoveButton,
		kPauseButton,
		kJumpOperation,
		kMoveOperation,
		kPauseOperation,
		kRemainingExplosion,
		kCountOfTextureHandleIndex
	};

	// UI情報
	struct UIData
	{
		// スプライト
		std::unique_ptr<Sprite> sprite_;
		// テクスチャハンドル
		uint32_t textureHandle_;
		// 位置
		Vector2 position_;
		// 色
		Vector4 color_;

		void Initialize(uint32_t textureHandle);

	};

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::array<uint32_t, kCountOfTextureHandleIndex>& textureHandles);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// セットプレイヤー
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }

private: // メンバ変数

	// プレイヤー情報
	Player* player_ = nullptr;

	// 入力情報
	Input* input_ = nullptr;

	//// 操作関連
	//// 爆発ジャンプ
	//UIData jumpOperation_;
	//UIData moveOperation_;
	//UIData pauseOperation_;

	//// プレイヤー関連
	//UIData remainingExplosion_;

	// UI情報
	std::array<UIData, kCountOfTextureHandleIndex> uiDatas_;

};

