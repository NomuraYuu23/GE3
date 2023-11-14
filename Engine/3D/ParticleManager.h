#pragma once
#include <cstdint>
#include "TransformationMatrix.h"
#include "../base/BufferResource.h"
#include "Particle3D.h"

class ParticleManager
{
public: // 静的メンバ変数

	// パーティクル最大数
	static uint32_t kNumInstanceMax_;

public: // メンバ関数

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static ParticleManager* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// SRVを作る
	/// </summary>
	void SRVCreate();

	/// <summary>
	/// パーティクル作成
	/// </summary>
	/// <param name="numInstance">インスタンス数</param>
	/// <returns></returns>
	Particle3D ParticleCreate(uint32_t numInstance);

	/// <summary>
	/// パーティクル終了時呼び出し関数
	/// </summary>
	void ParticleDelete(uint32_t numInstance, uint32_t indexMap);

public: // アクセッサ

	D3D12_CPU_DESCRIPTOR_HANDLE GetInstancingSrvHandleCPU() { return instancingSrvHandleCPU_; }

	D3D12_GPU_DESCRIPTOR_HANDLE GetInstancingSrvHandleGPU() { return instancingSrvHandleGPU_; }

	TransformationMatrix* GetTransformationMatrixMap() { return transformationMatrixMap_; }

	void SetTransformationMatrixMapWorld(Matrix4x4 matrix, uint32_t index);
	void SetTransformationMatrixMapWVP(Matrix4x4 matrix, uint32_t index);

	ID3D12Resource* GetTransformationMatrixBuff() { return transformationMatrixBuff_.Get(); }

private: // メンバ変数

	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixBuff_;
	//書き込むためのアドレスを取得
	TransformationMatrix* transformationMatrixMap_{};

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_;

	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_;

	//次に使うディスクリプタヒープの番号
	uint32_t indexNextMap_ = 0u;

};

