#include "IBone.h"
#include "../Math/Ease.h"

void IBone::Initialize(Model* model)
{

	model_ = model;

	worldTransform_.Initialize();

	collider_ = std::make_unique<ColliderShape>();

}

void IBone::Update(uint32_t frameCount)
{

	Animation(frameCount);

	worldTransform_.UpdateMatrix();

}

void IBone::Draw(const ViewProjection& viewProjection)
{

	model_->Draw(worldTransform_, viewProjection);

}

void IBone::Draw(const ViewProjection& viewProjection, Material* material)
{

	model_->Draw(worldTransform_, viewProjection, material);

}

void IBone::Animation(uint32_t frameCount)
{

	BoneData start = {};
	BoneData end = {};
	float t = 0.0f;
	uint32_t currentFrame = 0u;
	uint32_t endFrame = 0u;

	for (uint32_t i = 0; i < animationTransforms_.size(); i++) {
		if (animationTransforms_[i].endFrame_ >= frameCount) {
			// アニメーショントランスフォーム
			if (i == 0) {
				start = animationTransforms_[0];
			}
			else {
				start = animationTransforms_[i - 1];
			}
			end = animationTransforms_[i];
			break;
		}
	}

	//フレームカウント
	endFrame = end.endFrame_ - start.endFrame_;
	currentFrame = frameCount - start.endFrame_;
	t = static_cast<float>(currentFrame) / static_cast<float>(endFrame);

	// イージング
	worldTransform_.transform_.scale = Ease::Easing(static_cast<Ease::EaseName>(end.easeType_), start.transform_.scale, end.transform_.scale, t);
	worldTransform_.transform_.rotate = Ease::Easing(static_cast<Ease::EaseName>(end.easeType_), start.transform_.rotate, end.transform_.rotate, t);
	worldTransform_.transform_.translate = Ease::Easing(static_cast<Ease::EaseName>(end.easeType_), start.transform_.translate, end.transform_.translate, t);

}

void IBone::animationTransformChange(const std::string& key)
{

	//各グループについて
	for (std::map<std::string, std::vector<BoneData>>::iterator itData = animationTransformDatas_.begin(); itData != animationTransformDatas_.end();
		++itData) {
		// グループ名を取得
		const std::string& dataKey = itData->first;
		if (key == dataKey) {
			// グループの参照を取得
			animationTransforms_.clear();
			animationTransforms_ = itData->second;
		}
	}

}
