#include "ParticleManager.h"
#include <d3d12.h>
#include "../base/TextureManager.h"
#include "../3D/Model.h"
#include "../Math/DeltaTime.h"
#include "../../Application/Particle/MakeEmitter.h"

uint32_t ParticleManager::kNumInstanceMax_ = 32768;

ParticleManager* ParticleManager::GetInstance()
{
	static ParticleManager instance;
	return &instance;
	
}

void ParticleManager::Initialize()
{

	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	//WVP用のリソースを作る。
	particleForGPUBuff_ = BufferResource::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(ParticleForGPU) * kNumInstanceMax_);
	//書き込むためのアドレスを取得
	particleForGPUBuff_->Map(0, nullptr, reinterpret_cast<void**>(&particleForGPUMap_));

	for (size_t i = 0; i < kNumInstanceMax_; i++) {
		particleForGPUMap_[i].World = matrix4x4Calc->MakeIdentity4x4();
		particleForGPUMap_[i].WVP = matrix4x4Calc->MakeIdentity4x4();
		particleForGPUMap_[i].color = {1.0f,1.0f,1.0f,1.0f};
	}

	SRVCreate();

	billBoardMatrix_ = matrix4x4Calc->MakeIdentity4x4();

	for (size_t i = 0; i < particleDatas_.size(); i++) {
		particleDatas_[i].instanceIndex_ = 0;
		//スタートインスタンス用のリソースを作る。
		particleDatas_[i].startInstanceIdBuff_ = BufferResource::CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(StartInstanceId));
		//書き込むためのアドレスを取得
		particleDatas_[i].startInstanceIdBuff_->Map(0, nullptr, reinterpret_cast<void**>(&particleDatas_[i].startInstanceIdMap_));
		particleDatas_[i].startInstanceIdMap_->num = 0;
	}

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

void ParticleManager::Update(const Matrix4x4& cameraMatrix4x4)
{

	DeadDelete();

	BillBoardUpdate(cameraMatrix4x4);

	EmitterUpdate();

	ParticlesUpdate();

}

void ParticleManager::Draw()
{

	for (uint32_t i = 0; i < kCountofParticleModelIndex; i++) {
		currentModel_ = i;
		particleDatas_[i].model_->ParticleDraw();
	}

}

void ParticleManager::Map(const ViewProjection& viewProjection)
{

	uint32_t instanceIndex = 0u;

	for (uint32_t i = 0; i < kCountofParticleModelIndex; i++) {
		particleDatas_[i].startInstanceIdMap_->num = instanceIndex;
		std::list<IParticle*>::iterator itr = particleDatas_[i].particles_.begin();
		for (; itr != particleDatas_[i].particles_.end(); ++itr) {
			IParticle* particle = *itr;
			particleForGPUMap_[instanceIndex] = particle->Map(viewProjection);
			instanceIndex++;
		}
	}

}

void ParticleManager::Finalize()
{
	for (uint32_t i = 0; i < kCountofParticleModelIndex; i++) {
		particleDatas_[i].particles_.remove_if([](IParticle* particle) {
			delete particle;
			return true;
			});
	}
	emitters_.remove_if([](IEmitter* emitter) {
		delete emitter;
		return true;
	});

}

void ParticleManager::ModelCreate(std::array<Model*, kCountofParticleModelIndex> model)
{

	for (uint32_t i = 0; i < kCountofParticleModelIndex; i++) {
		particleDatas_[i].model_ = model[i];
	}

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

void ParticleManager::MakeEmitter(const TransformStructure& transform, uint32_t instanceCount,
	float frequency, float lifeTime,
	uint32_t particleModelNum, uint32_t paeticleName, uint32_t emitterName)
{
	emitters_.push_back(MakeEmitter::GetInstance()->Run(transform, instanceCount,
		frequency, lifeTime,
		particleModelNum, paeticleName, emitterName));

}

void ParticleManager::EmitterUpdate()
{

	for (IEmitter* emitter : emitters_) {
		emitter->Update();
		if (emitter->GetToEmit()) {
			AddParticles(emitter->Emit(),emitter->GetParticleModelNum());
			emitter->SetToEmit(false);
		}
	}

}

void ParticleManager::AddParticles(std::list<IParticle*> particles, uint32_t particleModelNum)
{

	particleDatas_[particleModelNum].particles_.splice(particleDatas_[particleModelNum].particles_.end(), particles);
	particleDatas_[particleModelNum].instanceIndex_ = static_cast<uint32_t>(particleDatas_[particleModelNum].particles_.size());
}

void ParticleManager::ParticlesUpdate()
{

	for (uint32_t i = 0; i < kCountofParticleModelIndex; i++) {
		for (IParticle* particle : particleDatas_[i].particles_) {
			particle->Update(billBoardMatrix_);
		}
	}
}

void ParticleManager::DeadDelete()
{
	
	for (uint32_t i = 0; i < kCountofParticleModelIndex; i++) {
		particleDatas_[i].particles_.remove_if([=](IParticle* particle) {
			if (particle->IsDead()) {
				delete particle;
				particleDatas_[i].instanceIndex_--;
				return true;
			}
			return false;
			});
	}

	emitters_.remove_if([](IEmitter* emitter) {
		if (emitter->IsDead()) {
			delete emitter;
			return true;
		}
		return false;
	});

}
