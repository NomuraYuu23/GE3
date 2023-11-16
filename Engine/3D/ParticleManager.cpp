#include "ParticleManager.h"
#include <d3d12.h>
#include "../base/TextureManager.h"
#include "Model.h"

uint32_t ParticleManager::kNumInstanceMax_ = 100;

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
	
}

void ParticleManager::Initialize()
{

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	//WVP用のリソースを作る。Matrix4x4 1つ分のサイズを用意する
	particleForGPUBuff_ = BufferResource::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(TransformationMatrix) * kNumInstanceMax_);
	//書き込むためのアドレスを取得
	particleForGPUBuff_->Map(0, nullptr, reinterpret_cast<void**>(&particleForGPUMap_));

	for (size_t i = 0; i < kNumInstanceMax_; i++) {
		particleForGPUMap_[i].World = matrix4x4Calc->MakeIdentity4x4();
		particleForGPUMap_[i].WVP = matrix4x4Calc->MakeIdentity4x4();
		particleForGPUMap_[i].color = {1.0f,1.0f,1.0f,1.0f};
	}

	SRVCreate();

	billBoardMatrix_ = matrix4x4Calc->MakeIdentity4x4();

}

void ParticleManager::SRVCreate()
{

	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = kNumInstanceMax_;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(TransformationMatrix);
	instancingSrvHandleCPU_ = TextureManager::GetInstance()->StaticGetCPUDescriptorHandle(1);
	instancingSrvHandleGPU_ = TextureManager::GetInstance()->StaticGetGPUDescriptorHandle(1);
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(particleForGPUBuff_.Get(), &instancingSrvDesc, instancingSrvHandleCPU_);

}

void ParticleManager::ParticleCreate(uint32_t numInstance)
{

	// インスタンス数確認
	assert(numInstance > 0 && numInstance + indexNextMap_ < kNumInstanceMax_);

	Particle* particle = new Particle();

	particle->Initialize();

	particles_.push_back(particle);

}

void ParticleManager::Update(const Matrix4x4& cameraMatrix4x4)
{

	BillBoardUpdate(cameraMatrix4x4);

	std::list<Particle*>::iterator itr = particles_.begin();
	for (; itr != particles_.end(); ++itr) {
		Particle* particle = *itr;
		particle->Update();
	}

}

void ParticleManager::Draw(const ViewProjection& viewProjection)
{
	
	model_->ParticleDraw(viewProjection);

}

void ParticleManager::Map(const ViewProjection& viewProjection)
{

	instanceIndex_ = 0u;

	std::list<Particle*>::iterator itr = particles_.begin();
	for (; itr != particles_.end(); ++itr) {
		Particle* particle = *itr;
		particleForGPUMap_[instanceIndex_] = particle->Map(viewProjection);
		instanceIndex_++;
	}

}

void ParticleManager::Finalize()
{

	particles_.remove_if([](Particle* particle) {
		delete particle;
		return true;
	});

}

void ParticleManager::ModelCreate()
{

	model_.reset(Model::Create("Resources/Particle/", "plane.obj", DirectXCommon::GetInstance()));

}

void ParticleManager::BillBoardUpdate(const Matrix4x4& cameraMatrix4x4)
{

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	Matrix4x4 backToFrontMatrix = matrix4x4Calc->MakeRotateXYZMatrix({ 0.0f,0.0f,0.0f });
	billBoardMatrix_ = matrix4x4Calc->Multiply(backToFrontMatrix, cameraMatrix4x4);
	billBoardMatrix_.m[3][0] = 0.0f;
	billBoardMatrix_.m[3][1] = 0.0f;
	billBoardMatrix_.m[3][2] = 0.0f;

}
