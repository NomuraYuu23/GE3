#pragma once
#include "../../Model.h"
#include "../../Material.h"
#include "../../WorldTransform.h"
#include "../Floor/Floor.h"

#include <vector>

class FloorManager
{

public: // メンバ関数

	/// <summary>
	/// デストラクタ
	/// </summary>
	~FloorManager();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, Material* material);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 床追加
	/// </summary>
	void AddFloor(Vector3 position, Vector3 rotate, bool isMoving);

public: // アクセッサ

	/// <summary>
	/// 床リストゲッター
	/// </summary>
	/// <returns></returns>
	std::vector<Floor*> GetFloors() { return floors_; }

private:

	// モデル
	Model* model_ = nullptr;
	// マテリアル
	Material* material_ = nullptr;

	// 床リスト
	std::vector<Floor*> floors_;

};
