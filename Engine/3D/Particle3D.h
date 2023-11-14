#pragma once
#include "TransformStructure.h"
#include "../Math/Matrix4x4.h"

#include "TransformationMatrix.h"
#include <wrl.h>
#include <d3d12.h>
#include "ViewProjection.h"

class Particle3D
{

public:

	// インスタンス数
	uint32_t numInstance_ = 0u;

	// マップのインデックス
	uint32_t indexMap_ = 0u;

	//トランスフォーム
	TransformStructure transform_[10];

	//ワールド行列
	Matrix4x4 worldMatrix_[10];

	~Particle3D();

	void Initialize(uint32_t numInstance, uint32_t indexMap);

	void UpdateMatrix();

	void Map(const ViewProjection& viewProjection);

};

