#include "DirectionalLight.h"
#include "TextureManager.h"
#include "WinApp.h"
#include <cassert>

#include <fstream>
#include <sstream>
#include <format>
#include "BufferResource.h"

using namespace DirectX;
using namespace Microsoft::WRL;

// デバイス
ID3D12Device* DirectionalLight::sDevice = nullptr;
// コマンドリスト
ID3D12GraphicsCommandList* DirectionalLight::sCommandList = nullptr;

/// <summary>
/// 静的初期化
/// </summary>
/// <param name="device">デバイス</param>
void DirectionalLight::StaticInitialize(
	ID3D12Device* device) {

	assert(device);

	sDevice = device;

}


/// <summary>
/// 光源生成
/// </summary>
/// <returns></returns>
DirectionalLight* DirectionalLight::Create() {

	DirectionalLight* directionalLight = new DirectionalLight();
	assert(directionalLight);

	//初期化
	directionalLight->Initialize();

	return directionalLight;

}

/// <summary>
/// 初期化
/// </summary>
void DirectionalLight::Initialize() {

	//平行光源リソースを作る
	directionalLightBuff_ = BufferResource::CreateBufferResource(sDevice, sizeof(DirectionalLight));

	//書き込むためのアドレスを取得
	directionalLightBuff_->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightMap));

	//デフォルト値
	directionalLightMap->color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightMap->direction = { 0.0f, -1.0f, 0.0f };
	directionalLightMap->intencity = 1.0f;

}

/// <summary>
/// 更新
/// </summary>
void DirectionalLight::Update(const DirectionalLightData& directionalLightData) {

	directionalLightMap->color = directionalLightData.color;
	directionalLightMap->direction = directionalLightData.direction;
	directionalLightMap->intencity = directionalLightData.intencity;

}

/// <summary>
/// 描画
/// </summary>
void DirectionalLight::Draw(ID3D12GraphicsCommandList* cmdList) {

	assert(sCommandList == nullptr);

	sCommandList = cmdList;

	//光源
	sCommandList->SetGraphicsRootConstantBufferView(3, directionalLightBuff_->GetGPUVirtualAddress());

	// コマンドリストを解除
	sCommandList = nullptr;

}

