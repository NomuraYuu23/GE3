#pragma once
#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d12.h>
#include <string>
#include <wrl.h>
#include <dxcapi.h>

#pragma comment(lib, "dxcompiler.lib")

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4x4.h"

#include "MaterialData.h"
#include "TransformStructure.h"

class Material
{

public:

	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// 3Dモデル生成
	/// </summary>
	/// <returns></returns>
	static Material* Create();

private:

	// デバイス
	static ID3D12Device* sDevice;

public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update(const TransformStructure& transform, const Vector4& color, int enableLighting);

	Microsoft::WRL::ComPtr<ID3D12Resource> GetMaterialBuff() { return materialBuff_; }


private:

	//Sprite用のマテリアルリソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> materialBuff_;

	MaterialData* materialMap = nullptr;

	TransformStructure uvTransform;

};

