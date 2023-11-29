#pragma once
#include "../../Engine/2D/Sprite.h"
#include "../../Engine/Input/Input.h"
#include <memory>
#include <array>
#include "UIData.h"

class Player;

class UI
{

public: // サブクラス

	// テクスチャハンドル番号
	enum kTextureHandleIndex {
		kJumpButton,
		kMoveButton,
		kPauseButton,
		kCameraButton,
		kJumpOperation,
		kMoveOperation,
		kPauseOperation,
		kCameraOperation,
		kRemainingExplosion,
		kNumHundred,
		kNumTen,
		kNumOne,
		kCountOfTextureHandleIndex
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

	/// <summary>
	/// グローバル変数
	/// </summary>
	void ApplyGlobalVariables();

private: // メンバ関数

	/// <summary>
	/// 操作説明初期化
	/// </summary>
	void OperationInitialize();

	/// <summary>
	/// 操作説明更新
	/// </summary>
	void OperationUpdate();

	/// <summary>
	/// 自爆回数初期化
	/// </summary>
	void RemainingExplosionInitialize();

	/// <summary>
	/// 自爆回数更新
	/// </summary>
	void RemainingExplosionUpdate();

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

