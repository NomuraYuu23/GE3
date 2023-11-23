#pragma once

#include "../../Engine/3D/WorldTransform.h"
#include "../../Engine/3D/ViewProjection.h"
#include "../../Engine/3D/Model.h"
#include <list>
#include <memory>

class ShadowManager
{

public: // サブクラス

	// 影
	struct Shadow{
		WorldTransform worldTransform_;
		bool isDraw_;
		WorldTransform* MakerWorldTransform_;
	};

	// 作る側
	struct Maker
	{
		WorldTransform* worldTransform_;
		Vector3 size_;
	};

	// 映す側
	struct Floor
	{
		WorldTransform* worldTransform_;
		Vector3 size_;
	};

public: // メンバ関数
	
	static ShadowManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 影を作る側追加
	/// </summary>
	/// <param name="worldTransform"></param>
	void AddMeker(WorldTransform* worldTransform, Vector3 size);

	/// <summary>
	/// 影を映すがわ追加
	/// </summary>
	/// <param name="worldTransform"></param>
	void AddFloor(WorldTransform* worldTransform, Vector3 size);

private: // メンバ関数

	/// <summary>
	/// 消えたか確認
	/// </summary>
	void CheckIfItsGone();

private: // メンバ変数

	ShadowManager() = default;
	~ShadowManager();
	ShadowManager(const ShadowManager&) = delete;
	const ShadowManager& operator=(const ShadowManager&) = delete;

	// 影
	std::list<Shadow*> shadows_;

	// 影をつくるobj
	std::list<Maker> makers_;

	// 影をうつすobj
	std::list<Floor> floors_;

	// モデル
	Model* model_ = nullptr;

};

