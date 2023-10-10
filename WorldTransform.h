#pragma once
#include "TransformStructure.h"
#include "Matrix4x4.h"
#include <wrl.h>
#include "ViewProjection.h"
#include <d3d12.h>
#include "TransformationMatrix.h"

struct WorldTransform
{

	//トランスフォーム
	TransformStructure transform_{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };
	
	//ワールド行列
	Matrix4x4 worldMatrix_;

	//親
	WorldTransform* parent_ = nullptr;

	void Initialize();

	void UpdateMatrix();

};
