#pragma once
#include <cstdint>
#include "../base/BufferResource.h"
class ParticleManager
{

public: // メンバ関数

	// パーティクル最大数
	static uint32_t kNumInstanceMax_;

	static ParticleManager* GetInstance();

	void Initialize();

	void SRVCreate();

private: // メンバ変数

	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixBuff_;
	//書き込むためのアドレスを取得
	TransformationMatrix* transformationMatrixMap_{};

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_;

	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_;

};

