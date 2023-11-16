#pragma once
#include "TransformStructure.h"
#include "../Math/Matrix4x4.h"

#include "TransformationMatrix.h"
#include <wrl.h>
#include <d3d12.h>
#include "ViewProjection.h"
#include "../Math/Vector4.h"

class Particle
{

public: // サブクラス

	struct Basic{
		// トランスフォーム
		TransformStructure transform_;
		// 行列
		Matrix4x4 worldMatrix_;
		// 速度
		Vector3 velocity_;
		// 色
		Vector4 color_;
		// 生存時間
		float lifeTime_;
		// 発生してからの経過時間
		float currentTime_;
	};

public:

	virtual ~Particle();

	virtual void Initialize(uint32_t numInstance);

	virtual void Update();

	void UpdateMatrix(uint32_t num);

	void Map(const ViewProjection& viewProjection, uint32_t indexMap);

	void TimeElapsed(uint32_t num);

	void GraduallyDisappear(uint32_t num);

public: // アクセッサ

	uint32_t GetNumInstance() { return numInstance_; }

protected: // メンバ変数

	// デルタタイム
	static const float kDeltaTime_;

	// インスタンス数
	uint32_t numInstance_ = 0u;

	//基本的な変数
	Basic* basic_;

};

