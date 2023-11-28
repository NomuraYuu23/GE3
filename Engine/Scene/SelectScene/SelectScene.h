#pragma once
#include "../IScene/IScene.h"

class SceneManager;

class SelectScene : public IScene{

public:

	// UI情報
	struct SelectUIData
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

		void Update();

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

private: // メンバ関数

	void SpriteUpdate();

	void SelectChange();

	void SelectDecision();

	void SelectReturn();

	void BackGroundInitialize();

	void SelectionSquareInitialize();

	void SelectionSquareUpdate();

	void OperationInitialize();

public:  // メンバ関数

	void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

private: // メンバ変数

	Input* input_ = Input::GetInstance();

	// 選択してるステージ
	uint32_t selectionSquareNum_;
	static const uint32_t selectionSquareMax_ = 4;

	// 背景スプライト
	SelectUIData background_;
	
	// 選択マススプライト
	SelectUIData selectionSquare_[selectionSquareMax_];

	// 操作説明スプライト
	SelectUIData decisionOperation_;
	SelectUIData moveOperation_;
	SelectUIData returnOperation_;

	SelectUIData decisionButton_;
	SelectUIData moveButton_;
	SelectUIData returnButton_;

	// 選択クールタイム
	float selectCooltime_ = 0.3f;
	
	// 選択クールタイム
	float selectElapsedCooltime_ = 0.0f;

	// シーンマネージャー
	SceneManager* sceneManager_;

};

