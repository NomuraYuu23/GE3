#include "DirectionalLight.h"
#include "TextureManager.h"
#include "WinApp.h"
#include <cassert>

#include <fstream>
#include <sstream>
#include <format>

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

//Resource作成関数化
Microsoft::WRL::ComPtr<ID3D12Resource> DirectionalLight::CreateBufferResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const size_t& sizeInBytes) {

	//頂点リソース用のヒープの設定
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;//UploadHeapを使う
	//頂点リソースの設定
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	//バッファリソース。テクスチャの場合はまた別の設定をする
	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	vertexResourceDesc.Width = sizeInBytes;//リソースのサイズ。今回はVector4を3頂点分
	//バッファの場合はこれらは1にする決まり
	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;
	//バッファの場合はこれにする決まり
	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//実際に頂点リソースを作る
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResource = nullptr;
	HRESULT hr = device->CreateCommittedResource(&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr,
		IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));

	return vertexResource;

}


/// <summary>
/// 初期化
/// </summary>
void DirectionalLight::Initialize() {

	//平行光源リソースを作る
	directionalLightBuff_ = CreateBufferResource(sDevice, sizeof(DirectionalLight));

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

