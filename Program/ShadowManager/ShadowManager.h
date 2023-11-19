#pragma once

#include "../../Engine/3D/WorldTransform.h"
#include "../../Engine/3D/ViewProjection.h"
#include "../../Engine/3D/Model.h"
#include <list>
#include <memory>

class ShadowManager
{

public: // サブクラス

	struct Shadow{
		WorldTransform worldTransform_;
		bool isDraw_;
	};

public: // メンバ関数

	static void Initialize(Model* model);

	static void Update();

	static void Draw(const ViewProjection& viewProjection);

	static void AddMeker(WorldTransform* worldTransform);

	static void AddFloor(WorldTransform* worldTransform);

private: // メンバ関数

	static void CheckIfItsGone();

private: // メンバ変数

	// 影
	static std::list<Shadow> shadows_;

	// 影をつくるobj
	static std::list<WorldTransform*> makerWorldTransforms_;

	// 影をうつすobj
	static std::list<WorldTransform*> floorWorldTransforms_;

	// モデル
	static Model* model_;

};

