#pragma once
#include "../IScene/IScene.h"
#include "../../../Program/UI/UIData.h"

class SceneManager;

class SelectScene : public IScene{

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

private: // メンバ関数

	void SelectChange();

	void SelectDecision();

	void SelectReturn();

	void BackGroundInitialize();

	void BackGroundUpdate();

	void SelectionSquareInitialize();

	void SelectionSquareUpdate();

	void OperationInitialize();

private: // メンバ変数

	Input* input_ = Input::GetInstance();

	// 選択してるステージ
	uint32_t selectionSquareNum_;
	static const uint32_t selectionSquareMax_ = 4;

	// 背景スプライト
	UIData background_;
	
	// 選択マススプライト
	UIData selectionSquare_[selectionSquareMax_];

	// 操作説明スプライト
	UIData decisionOperation_;
	UIData moveOperation_;
	UIData returnOperation_;

	UIData decisionButton_;
	UIData moveButton_;
	UIData returnButton_;

	// 名前スプライト
	UIData name_;

	// 選択クールタイム
	float selectCooltime_ = 0.3f;

	float t[selectionSquareMax_] = { 0,0,0,0 };

	const float easeSpeed_ = 0.05f;

	float magnification = 1.0f;

	const Vector2 baseSize_ = { 128.0f,128.0f };

	Vector2 nowSize_[selectionSquareMax_] = {};

	const Vector2 maxSize_ = { 192.0f,192.0f };
	
	// 選択クールタイム
	float selectElapsedCooltime_ = 0.0f;

};

