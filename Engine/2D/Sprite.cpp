#include "Sprite.h"
#include "../base/TextureManager.h"
#include "../base/WinApp.h"
#include <cassert>
#include "../base/BufferResource.h"

using namespace DirectX;
using namespace Microsoft::WRL;

// デバイス
ID3D12Device* Sprite::sDevice = nullptr;
// ディスクリプタサイズ
UINT Sprite::sDescriptorHandleIncrementSize;
// コマンドリスト
ID3D12GraphicsCommandList* Sprite::sCommandList = nullptr;
// ルートシグネチャ
ComPtr<ID3D12RootSignature> Sprite::sRootSignature;
// パイプラインステートオブジェクト
ComPtr<ID3D12PipelineState> Sprite::sPipelineState;
//計算
Matrix4x4Calc* Sprite::matrix4x4Calc = nullptr;

/// <summary>
/// 静的初期化
/// </summary>
/// <param name="device">デバイス</param>
void Sprite::StaticInitialize(
	ID3D12Device* device,
	ComPtr<ID3D12RootSignature> rootSignature,
	ComPtr<ID3D12PipelineState> pipelineState) {

	assert(device);
	
	sDevice = device;

	sDescriptorHandleIncrementSize = sDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	matrix4x4Calc = Matrix4x4Calc::GetInstance();

	sRootSignature = rootSignature;
	sPipelineState = pipelineState;

}

/// <summary>
/// 静的前処理
/// </summary>
/// <param name="cmdList">描画コマンドリスト</param>
void Sprite::PreDraw(ID3D12GraphicsCommandList* cmdList) {

	assert(Sprite::sCommandList == nullptr);

	sCommandList = cmdList;

	//RootSignatureを設定。
	sCommandList->SetPipelineState(sPipelineState.Get());//PS0を設定
	sCommandList->SetGraphicsRootSignature(sRootSignature.Get());
	//形状を設定。PS0に設定しているものとは別。
	sCommandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

}

/// <summary>
/// 描画後処理
/// </summary>
void Sprite::PostDraw() {
	//コマンドリストを解除
	Sprite::sCommandList = nullptr;

}

/// <summary>
/// スプライト生成
/// </summary>
/// <param name="textureHandle">テクスチャハンドル</param>
/// <param name="position">座標</param>
/// <param name="color">色</param>
/// <param name="anchorpoint">アンカーポイント</param>
/// <param name="isFlipX">左右反転</param>
/// <param name="isFlipY">上下反転</param>
/// <returns>生成されたスプライト</returns>
Sprite* Sprite::Create(
	uint32_t textureHandle, const TransformStructure& transform, Material* material) {

	// 仮サイズ
	Vector2 size = { 100.0f, 100.0f };

	// テクスチャ情報取得
	const D3D12_RESOURCE_DESC& resDesc = TextureManager::GetInstance()->GetResourceDesc(textureHandle);
	// スプライトのサイズをテクスチャのサイズに設定
	size = { (float)resDesc.Width, (float)resDesc.Height };

	// Spriteのインスタンスを生成
	Sprite* sprite = new Sprite(textureHandle, transform, size, material);
	if (sprite == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!sprite->Initialize()) {
		delete sprite;
		assert(0);
		return nullptr;
	}

	return sprite;

}

/// <summary>
/// コンストラクタ
/// </summary>
Sprite::Sprite() {}

/// <summary>
/// コンストラクタ
/// </summary>
Sprite::Sprite(
	uint32_t textureHandle, const TransformStructure& transform, const Vector2& size, Material* material) {


	textureHandle_ = textureHandle;
	//CPUで動かす用のTransformを作る
	transform_ = transform;
	//大きさ
	size_ = size;

	// マテリアル
	material_ = material;

}

/// <summary>
/// 初期化
/// </summary>
/// <returns>成否</returns>
bool Sprite::Initialize() {

	assert(sDevice);

	//HRESULT hr;

	resourceDesc_ = TextureManager::GetInstance()->GetResourceDesc(textureHandle_);

	//Sprite用の頂点リソースを作る
	vertBuff_ = BufferResource::CreateBufferResource(sDevice, sizeof(VertexData) * 6);

	//リソースの先頭のアドレスから使う
	vbView_.BufferLocation = vertBuff_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vbView_.SizeInBytes = sizeof(VertexData) * 6;
	//1頂点あたりのサイズ
	vbView_.StrideInBytes = sizeof(VertexData);

	//書き込むためのアドレスを取得
	vertBuff_->Map(0, nullptr, reinterpret_cast<void**>(&vertMap));

	//インデックスリソースを作る
	indexBuff_ = BufferResource::CreateBufferResource(sDevice, sizeof(uint32_t) * 6);

	//インデックスバッファビュー
	//リソースの先頭のアドレスから使う
	ibView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	//使用するリソースのサイズはインデックス6つ分のサイズ
	ibView_.SizeInBytes = sizeof(uint32_t) * 6;
	//インデックスはuint32_tとする
	ibView_.Format = DXGI_FORMAT_R32_UINT;

	//インデックスリソースにデータを書き込む
	indexBuff_->Map(0, nullptr, reinterpret_cast<void**>(&indexMap));

	//一枚目の三角形
	vertMap[0].position = { 0.0f, size_.y, 0.0f, 1.0f };//左下
	vertMap[0].texcoord = { 0.0f, 1.0f };
	vertMap[0].normal = { 0.0f, 0.0f, -1.0f };
	vertMap[1].position = { 0.0f, 0.0f, 0.0f, 1.0f };//左上
	vertMap[1].texcoord = { 0.0f, 0.0f };
	vertMap[1].normal = { 0.0f, 0.0f, -1.0f };
	vertMap[2].position = { size_.x, size_.y, 0.0f, 1.0f };//右下
	vertMap[2].texcoord = { 1.0f, 1.0f };
	vertMap[2].normal = { 0.0f, 0.0f, -1.0f };
	//ニ枚目の三角形
	vertMap[3].position = { size_.x, 0.0f, 0.0f, 1.0f };//右上
	vertMap[3].texcoord = { 1.0f, 0.0f };
	vertMap[3].normal = { 0.0f, 0.0f, -1.0f };
	vertMap[4].position = { 0.0f, 0.0f, 0.0f, 1.0f };//左上
	vertMap[4].texcoord = { 0.0f, 0.0f };
	vertMap[4].normal = { 0.0f, 0.0f, -1.0f };
	vertMap[5].position = { size_.x, size_.y, 0.0f, 1.0f };//右下
	vertMap[5].texcoord = { 1.0f, 1.0f };
	vertMap[5].normal = { 0.0f, 0.0f, -1.0f };

	//インデックスリソースにデータを書き込む
	indexMap[0] = 0;
	indexMap[1] = 1;
	indexMap[2] = 2;
	indexMap[3] = 1;
	indexMap[4] = 3;
	indexMap[5] = 2;

	//Sprite用のTransformationMatrix用のリソースを作る。Matrix4x4 1つ分のサイズ
	transformationMatrixBuff_ = BufferResource::CreateBufferResource(sDevice, sizeof(TransformationMatrix));

	//書き込むためのアドレスを取得
	transformationMatrixBuff_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixMap));
	//単位行列を書き込んでおく
	transformationMatrixMap->World = matrix4x4Calc->MakeIdentity4x4();
	transformationMatrixMap->WVP = matrix4x4Calc->MakeIdentity4x4();

	//CPUで動かす用のTransformを作る
	transform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

	return true;

}

/// <summary>
/// 更新
/// </summary>
void Sprite::Update(const TransformStructure& transform) {

	transform_ = transform;

	//Sprite用のWorldViewProjectionMatrixを作る
	Matrix4x4 WorldMatrixSprite = matrix4x4Calc->MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	Matrix4x4 viewMatrixSprite = matrix4x4Calc->MakeIdentity4x4();
	Matrix4x4 projectionMatrixSprite = matrix4x4Calc->MakeOrthographicMatrix(0.0f, 0.0f, float(WinApp::kWindowWidth), float(WinApp::kWindowHeight), 0.0f, 100.0f);
	Matrix4x4 worldViewProjectionMatrixSprite = matrix4x4Calc->Multiply(WorldMatrixSprite, matrix4x4Calc->Multiply(viewMatrixSprite, projectionMatrixSprite));
	transformationMatrixMap->WVP = worldViewProjectionMatrixSprite;
	transformationMatrixMap->World = WorldMatrixSprite;

}

/// <summary>
/// テクスチャハンドルの設定
/// </summary>
/// <param name="textureHandle"></param>
void Sprite::SetTextureHandle(uint32_t textureHandle) {

	textureHandle_ = textureHandle;
	resourceDesc_ = TextureManager::GetInstance()->GetResourceDesc(textureHandle_);

}



/// <summary>
/// 描画
/// </summary>
void Sprite::Draw() {

	// 頂点バッファの設定
	sCommandList->IASetVertexBuffers(0, 1, &vbView_);
	//IBVを設定
	sCommandList->IASetIndexBuffer(&ibView_);

	//TransformationMatrixCBufferの場所を設定
	sCommandList->SetGraphicsRootConstantBufferView(1, transformationMatrixBuff_->GetGPUVirtualAddress());

	//マテリアルCBufferの場所を設定
	sCommandList->SetGraphicsRootConstantBufferView(0, material_->GetMaterialBuff()->GetGPUVirtualAddress());

	// シェーダーリソースビューをセット
	TextureManager::GetInstance()->SetGraphicsRootDescriptorTable(sCommandList, 2, textureHandle_);

	//描画
	sCommandList->DrawIndexedInstanced(kVertNum, 1, 0, 0, 0);

}
