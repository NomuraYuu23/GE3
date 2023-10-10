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

	// TransformationMatrix用のリソースを作る。Matrix4x4 1つ分のサイズ
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixBuff_;
	//データを書き込む
	TransformationMatrix* transformationMatrixMap_ = nullptr;

	ViewProjection* viewProjection_;

	//親
	WorldTransform* parent_ = nullptr;

	void Initialize(ViewProjection* viewProjection);

	void UpdateMatrix();

};
