#include "SampleBone.h"

void SampleBone::Initialize(Model* model)
{

	model_ = model;
	objectName_ = "SampleObject";
	partName_ = "SamplePart";

	motionNames_.push_back("SampleMotion1");
	motionNames_.push_back("SampleMotion2");

	bone_ = std::make_unique<IBone>();
	bone_->Initialize(model_, objectName_, partName_, motionNames_);

}

void SampleBone::Update()
{

	bone_->Update(0, motionNames_);

}

void SampleBone::Draw(BaseCamera& camera)
{

	bone_->Draw(camera);

}
