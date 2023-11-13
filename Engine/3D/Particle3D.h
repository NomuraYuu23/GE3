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

	struct Data
	{

		//トランスフォーム
		TransformStructure transform_{ {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

		//ワールド行列
		Matrix4x4 worldMatrix_;

		// 回転行列
		Matrix4x4 rotateMatrix_;

		// 方向ベクトルで回転行列
		bool usedDirection_;

		// 方向ベクトル
		Vector3 direction_ = { 0.0f,0.0f,1.0f };

		// スケールを考えない
		Matrix4x4 parentMatrix_;

	};

	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixBuff_;
	//書き込むためのアドレスを取得
	TransformationMatrix* transformationMatrixMap_{};

	// インスタンス数
	uint32_t numInstance_ = 0u;

	// マップのインデックス
	uint32_t indexMap_ = 0u;

	// データ
	Data* data_{};

	~Particle3D();

	void Initialize(uint32_t numInstance, uint32_t indexMap);

	void UpdateMatrix();

	void Map(const ViewProjection& viewProjection);

};

