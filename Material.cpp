#include "Material.h"
#include <cassert>

#include <fstream>
#include <sstream>
#include <format>

using namespace DirectX;
using namespace Microsoft::WRL;

// デバイス
ID3D12Device* Material::sDevice = nullptr;

/// <summary>
/// 静的初期化
/// </summary>
/// <param name="device">デバイス</param>
void Material::StaticInitialize(ID3D12Device* device) {

	assert(device);

	sDevice = device;

}

/// <summary>
/// マテリアル生成
/// </summary>
/// <returns></returns>
Material* Material::Create() {

	// 3Dオブジェクトのインスタンスを生成
	Material* material = new Material();
	assert(material);

	// 初期化
	material->Initialize();

	return material;

}

//Resource作成関数化
Microsoft::WRL::ComPtr<ID3D12Resource>  Material::CreateBufferResource(Microsoft::WRL::ComPtr<ID3D12Device> device, const size_t& sizeInBytes) {

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
void Material::Initialize() {

	//Sprite用のマテリアルリソースを作る
	materialBuff_ = CreateBufferResource(sDevice, sizeof(Material));
	//書き込むためのアドレスを取得
	materialBuff_->Map(0, nullptr, reinterpret_cast<void**>(&materialMap));
	//白を書き込んでみる
	materialMap->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	//SpriteはLightingしないのでfalseを設定する
	materialMap->enableLighting = None;	//UVTransfome初期化
	materialMap->uvTransform = Matrix4x4Calc::GetInstance()->MakeIdentity4x4();

	uvTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

}

/// <summary>
/// 更新
/// </summary>
void Material::Update(const TransformStructure& transform, const Vector4& color, int enableLighting) {

	uvTransform = transform;

	//UVTransfome用
	Matrix4x4 uvTransformMatrix = Matrix4x4Calc::GetInstance()->MakeAffineMatrix(uvTransform.scale, uvTransform.rotate, uvTransform.translate);
	materialMap->uvTransform = uvTransformMatrix;

	materialMap->color = color;

	materialMap->enableLighting = enableLighting;

}
