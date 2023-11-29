#pragma once
#include "../Input/Input.h"
#include "../2D/Sprite.h"
#include <array>

/// <summary>
/// ポーズ
/// </summary>
class Pause
{

public: // サブクラス

	// ポーズメニュー
	enum PauseMenu{
		kGoToSelect,       // Select
		kReturnToGame,    // ゲームに戻る
		kCountOfPauseMenu // 使用不可
	};

	// ポーズで使うテクスチャ番号
	enum PauseTextureNo{
		kPausingTextureNo,              // ポーズ中
		kGoToSelectTextureNo,			 // Select
		kReturnToGameTextureNo,         // ゲームに戻る
		kCountOfPauseTextureNo // 使用不可
	};

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::array<uint32_t, PauseTextureNo::kCountOfPauseTextureNo>& textureHandles);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ関数

	/// <summary>
	/// ポーズ画面の切り替え
	/// </summary>
	void PoseSwitching();

	/// <summary>
	/// ポーズメニュー操作
	/// </summary>
	void PauseMenuOperation();

	/// <summary>
	/// タイトルへ行く
	/// </summary>
	void PauseMenuGoToSelect();

	/// <summary>
	/// ゲームに戻る
	/// </summary>
	void PauseMenuReturnToGame();

public: // アクセッサ

	bool IsPause() { return isPause_; }

	bool GoToTheSelect() { return goToTheSelect_; }

private: // メンバ変数

	// 入力
	Input* input_ = nullptr;

	// ポーズしているか
	bool isPause_;
	// ポーズメニューでどこを選択しているか
	int pauseMenuSelect_;

	// タイトルへ戻るか
	bool goToTheSelect_;

	// テクスチャハンドル
	std::array<uint32_t, PauseTextureNo::kCountOfPauseTextureNo> textureHandles_;

private: // メンバ変数(スプライト)

	// ポーズ中
	std::unique_ptr<Sprite> pausingSprite_;
	// タイトルへ
	std::unique_ptr<Sprite> goToSelectSprite_;
	// ゲームに戻る
	std::unique_ptr<Sprite> returnToGameSprite_;

	// 選択クールタイム
	float selectCooltime_ = 0.3f;

	// 選択クールタイム
	float selectElapsedCooltime_ = 0.0f;

	// アニメーションパラメータ
	float animationParameter_ = 0.0f;

	// アニメーションピリオド
	uint32_t animationPeriod_ = 60;

};

