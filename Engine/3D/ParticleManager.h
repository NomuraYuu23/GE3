#pragma once
#include <cstdint>
#include "TransformationMatrix.h"
#include "../base/BufferResource.h"
#include "Particle.h"
#include "Emitter.h"
#include <list>
#include <memory>
#include "ParticleForGPU.h"

class Model;

class ParticleManager
{

public: // サブクラス

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
	/// 更新
	/// </summary>
	void Update(const Matrix4x4& cameraMatrix4x4);

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

	/// <summary>
	/// ビルボード更新
	/// </summary>
	/// <param name="cameraMatrix4x4"></param>
	void BillBoardUpdate(const Matrix4x4& cameraMatrix4x4);

	/// <summary>
	/// エミッタ生成
	/// </summary>
	/// <param name="transform"></param>
	/// <param name="lifeTime"></param>
	void EmitterCreate(const TransformStructure& transform, float lifeTime);

	/// <summary>
	/// エミッタ更新
	/// </summary>
	void EmitterUpdate();

	/// <summary>
	/// パーティクル追加
	/// </summary>
	void AddParticles(std::list<Particle*> particles);

	/// <summary>
	/// パーティクル更新
	/// </summary>
	void ParticlesUpdate();

	/// <summary>
	/// 死んでるのを削除
	/// </summary>
	void DeadDelete();

public: // アクセッサ

	D3D12_CPU_DESCRIPTOR_HANDLE GetInstancingSrvHandleCPU() { return instancingSrvHandleCPU_; }

	D3D12_GPU_DESCRIPTOR_HANDLE GetInstancingSrvHandleGPU() { return instancingSrvHandleGPU_; }

	ParticleForGPU* GetParticleForGPUMap() { return particleForGPUMap_; }

	ID3D12Resource* GetParticleForGPUBuff() { return particleForGPUBuff_.Get(); }

	uint32_t GetInstanceIndex() { return instanceIndex_; }

	Matrix4x4 GetBillBoardMatrix() { return billBoardMatrix_; }

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

	// ビルボード
	Matrix4x4 billBoardMatrix_;

	// エミッタ
	std::list<Emitter*> emitters_;

};

