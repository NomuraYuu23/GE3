#pragma once
#include "TransformStructure.h"
#include "../Math/Matrix4x4.h"

#include "TransformationMatrix.h"
#include <wrl.h>
#include <d3d12.h>
#include "ViewProjection.h"

class Particle
{

public:

	~Particle();

	void Initialize(uint32_t numInstance);

	void Update();

	void UpdateMatrix();

	void Map(const ViewProjection& viewProjection, uint32_t indexMap);

public: // アクセッサ

	uint32_t GetNumInstance() { return numInstance_; }

private:

	// インスタンス数
	uint32_t numInstance_ = 0u;

	//トランスフォーム
	TransformStructure* transform_;

	//ワールド行列
	Matrix4x4* worldMatrix_;

};

