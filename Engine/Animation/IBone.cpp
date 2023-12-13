#include "IBone.h"
#include "../Math/Ease.h"
#include "../../externals/nlohmann/json.hpp"
#include <fstream>

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

//void IBone::RegistrationOfAnimation(const std::string& fileName, const std::string& groupName)
//{
//
//	// ファイルネームがオブジェクト 	
//	// キーがモーション名
//
//	// 読み込むJSONファイルのフルパスを合成する
//	std::string filePath = fileName + ".json";
//	// 読み込み用ファイルストリーム
//	std::ifstream ifs;
//	// ファイルを読み込み用に聞く
//	ifs.open(filePath);
//
//	// ファイルオープン失敗?
//	if (ifs.fail()) {
//		std::string message = "Failed open data file for write.";
//		MessageBoxA(nullptr, message.c_str(), "BoneAnimation", 0);
//		assert(0);
//		return;
//	}
//
//	nlohmann::json root;
//
//	// json文字列からjsonのデータ構造に展開
//	ifs >> root;
//	// ファイルを閉じる
//	ifs.close();
//
//	// グループを検索
//	nlohmann::json::iterator itGroup = root.find(groupName);
//
//	// 未登録チェック
//	assert(itGroup != root.end());
//
//	// 各アイテムについて
//	for (nlohmann::json::iterator itItem = itGroup->begin(); itItem != itGroup->end(); ++itItem) {
//
//		// アイテム名を取得
//		const std::string& itemName = itItem.key();
//		// float型のjson配列登録
//		Vector3 value = { itItem->at(0), itItem->at(1), itItem->at(2) };
//		//SetValue(groupName, itemName, value);
//
//		animationTransformDatas_[itemName];
//
//		animationTransformDatas_[itemName] = itItem.value();
//	
//	}
//}

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
			animationTransforms_.shrink_to_fit();
			animationTransforms_ = itData->second;
		}
	}

}
