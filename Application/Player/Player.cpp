#include "Player.h"
#include "../../Engine/Collider/Capsule/Capsule.h"

void Player::Initialize(const std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount>& models)
{

	// マテリアル
	material_.reset(Material::Create());

	// ワールドトランスフォーム
	worldTransform_.Initialize();

	// ステート
	playerState_; // 最初のステート
	playerState_->Initialize();

	// ステート番号
	stateNo_ = PlayerState::kPlayerStateStand; // 最初のステート
	requestStateNo_ = PlayerState::kPlayerStateStand; // 最初のステート

	// モデル
	models_ = models;

	// パーツ
	// モーション名をstd::vectorに
	std::vector<std::string> motionNames;
	for (uint32_t i = 0; i < PlayerMotionIndex::kPlayerMotionIndexOfCount; ++i) {
		motionNames.push_back(motionNames_[i]);
	}
	for (uint32_t i = 0; i < PlayerPartIndex::kPlayerPartIndexOfCount; ++i) {
		parts_[i] = std::make_unique<IBone>();
		parts_[i]->Initialize(models_[i], objectName_, partNames_[i], motionNames);
	}

	// コライダー
	ColliderInitialize();

	// アニメーションカウント
	animationCount_ = 0u;

}

void Player::ColliderInitialize()
{

	// コライダー
	for (uint32_t i = 0; i < PlayerColliderIndex::kPlayerColliderIndexOfCount; ++i) {
		colliders_[i] = std::make_unique<Capsule>();
		colliderRadiuses_[i] = 1.0f;
	}

	//計算用
	Segment segment = { 
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
	};
	Vector3 diff = {0.0f,0.0f,0.0f};
	
	// 頭
	diff = Vector3Calc::Subtract(parts_[kPlayerPartTorso]->GetWorldPosition(), parts_[kPlayerPartHead]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartHead]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderHead].get())->Initialize(segment, colliderRadiuses_[kPlayerColliderHead], this);
	
	// 胴
	diff = Vector3Calc::Subtract(parts_[kPlayerPartLowerBack]->GetWorldPosition(), parts_[kPlayerPartTorso]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartTorso]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderTorso].get())->Initialize(segment, colliderRadiuses_[kPlayerColliderTorso], this);
	
	// 左上腕
	diff = Vector3Calc::Subtract(parts_[kPlayerPartLeftForearm]->GetWorldPosition(), parts_[kPlayerPartLeftUpperArm]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartLeftUpperArm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftUpperArm].get())->Initialize(segment, colliderRadiuses_[kPlayerColliderLeftUpperArm], this);
	
	// 左前腕 
	diff = Vector3Calc::Subtract(parts_[kPlayerPartLeftHand]->GetWorldPosition(), parts_[kPlayerPartLeftForearm]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartLeftForearm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftForearm].get())->Initialize(segment, colliderRadiuses_[kPlayerColliderLeftForearm], this);
	
	// 右上腕
	diff = Vector3Calc::Subtract(parts_[kPlayerPartRightForearm]->GetWorldPosition(), parts_[kPlayerPartRightUpperArm]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartRightUpperArm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderRightUpperArm].get())->Initialize(segment, colliderRadiuses_[kPlayerColliderRightUpperArm], this);
	
	// 右前腕 
	diff = Vector3Calc::Subtract(parts_[kPlayerPartRightHand]->GetWorldPosition(), parts_[kPlayerPartRightForearm]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartRightForearm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderRightForearm].get())->Initialize(segment, colliderRadiuses_[kPlayerColliderRightForearm], this);
	
	// 左太もも
	diff = Vector3Calc::Subtract(parts_[kPlayerPartLeftShin]->GetWorldPosition(), parts_[kPlayerPartLeftThigh]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartLeftThigh]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftThigh].get())->Initialize(segment, colliderRadiuses_[kPlayerColliderLeftThigh], this);
	
	// 左すね
	diff = Vector3Calc::Subtract(parts_[kPlayerPartLeftAnkle]->GetWorldPosition(), parts_[kPlayerPartLeftShin]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartLeftShin]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftShin].get())->Initialize(segment, colliderRadiuses_[kPlayerColliderLeftShin], this);
	
	// 右太もも
	diff = Vector3Calc::Subtract(parts_[kPlayerPartRightShin]->GetWorldPosition(), parts_[kPlayerPartRightThigh]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartRightThigh]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderRightThigh].get())->Initialize(segment, colliderRadiuses_[kPlayerColliderRightThigh], this);
	
	// 右すね
	diff = Vector3Calc::Subtract(parts_[kPlayerPartRightAnkle]->GetWorldPosition(), parts_[kPlayerPartRightShin]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartRightShin]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderRightShin].get())->Initialize(segment, colliderRadiuses_[kPlayerColliderRightShin], this);


}
