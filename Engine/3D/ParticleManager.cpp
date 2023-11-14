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
	transformationMatrixBuff_ = BufferResource::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(TransformationMatrix) * kNumInstanceMax_);
	//書き込むためのアドレスを取得
	transformationMatrixBuff_->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixMap_));

	for (size_t i = 0; i < kNumInstanceMax_; i++) {
		transformationMatrixMap_[i].World = matrix4x4Calc->MakeIdentity4x4();
		transformationMatrixMap_[i].WVP = matrix4x4Calc->MakeIdentity4x4();
	}

	SRVCreate();

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
	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(transformationMatrixBuff_.Get(), &instancingSrvDesc, instancingSrvHandleCPU_);

}

void ParticleManager::ParticleCreate(uint32_t numInstance)
{

	// インスタンス数確認
	assert(numInstance > 0 && numInstance + indexNextMap_ < kNumInstanceMax_);

	Particle* particle = new Particle();

	particle->Initialize(numInstance);

	particles_.push_back(particle);

}

void ParticleManager::Update()
{

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
		particle->Map(viewProjection, instanceIndex_);
		instanceIndex_ += particle->GetNumInstance();
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

	model_.reset(Model::Create("Resources/default/", "plane.obj", DirectXCommon::GetInstance()));

}

void ParticleManager::SetTransformationMatrixMapWorld(Matrix4x4 matrix, uint32_t index)
{

	transformationMatrixMap_[index].World = matrix;

}

void ParticleManager::SetTransformationMatrixMapWVP(Matrix4x4 matrix, uint32_t index)
{

	transformationMatrixMap_[index].WVP = matrix;

}

