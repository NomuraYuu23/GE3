#pragma once
#include <cstdint>
#include "TransformationMatrix.h"
#include "../base/BufferResource.h"
#include "Particle.h"
#include <list>
#include <memory>
#include "ParticleForGPU.h"

class Model;

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
	void ParticleCreate(uint32_t numInstance);

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
	/// マッピング
	/// </summary>
	/// <param name="viewProjection"></param>
	void Map(const ViewProjection& viewProjection);

	/// <summary>
	/// 後処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// モデル作成
	/// </summary>
	void ModelCreate();

public: // アクセッサ

	D3D12_CPU_DESCRIPTOR_HANDLE GetInstancingSrvHandleCPU() { return instancingSrvHandleCPU_; }

	D3D12_GPU_DESCRIPTOR_HANDLE GetInstancingSrvHandleGPU() { return instancingSrvHandleGPU_; }

	ParticleForGPU* GetParticleForGPUMap() { return particleForGPUMap_; }

	void SetParticleForGPUMap(const ParticleForGPU& particleForGPU, uint32_t index);

	ID3D12Resource* GetParticleForGPUBuff() { return particleForGPUBuff_.Get(); }

	uint32_t GetInstanceIndex() { return instanceIndex_; }

private: // メンバ変数

	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	Microsoft::WRL::ComPtr<ID3D12Resource> particleForGPUBuff_;
	//書き込むためのアドレスを取得
	ParticleForGPU* particleForGPUMap_{};

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_;

	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_;

	//次に使うディスクリプタヒープの番号
	uint32_t indexNextMap_ = 0u;

	// パーティクル
	std::list<Particle*> particles_;

	// モデル
	std::unique_ptr<Model> model_;

	// 描画するインスタンス数
	uint32_t instanceIndex_;

};

