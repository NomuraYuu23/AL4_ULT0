#include "Player.h"
#include "../../Engine/Collider/Capsule/Capsule.h"
#include "../../Engine/2D/ImguiManager.h"

void Player::Initialize(const std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount>& models)
{

	// マテリアル
	material_.reset(Material::Create());

	// ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.transform_.translate.y = height_;
	worldTransform_.UpdateMatrix();

	// コマンド
	playerCommand_ = PlayerCommand::GetInstance();
	playerCommand_->Initialize();

	// コマンドを受け取るか
	receiveCommand_ = true;

	// ステート
	StateInitialize();

	// モデル
	models_ = models;

	// パーツ
	PartInitialize();

	// コライダー
	ColliderInitialize();

}

void Player::Update()
{

	if (receiveCommand_) {
		nextStateNo_ = playerCommand_->Command();
	}

	// ステート
	StateUpdate();
	worldTransform_.UpdateMatrix();

	// パーツ,アニメーション
	PartUpdate();

	// コライダー
	ColliderUpdate();

}

void Player::Draw(BaseCamera& camera)
{

	for (uint32_t i = 0; i < PlayerPartIndex::kPlayerPartIndexOfCount; ++i) {
		parts_[i]->Draw(camera, material_.get());
	}

}

void Player::ImGuiDraw()
{

	ImGui::Begin("Player");
	ImGui::Text("Position");
	ImGui::Text("X:%f, Y:%f, Z:%f", worldTransform_.worldMatrix_.m[3][0], worldTransform_.worldMatrix_.m[3][1], worldTransform_.worldMatrix_.m[3][2]);
	ImGui::Text("Direction");
	ImGui::Text("X:%f, Y:%f, Z:%f", worldTransform_.direction_.x, worldTransform_.direction_.y, worldTransform_.direction_.z);
	ImGui::End();

}

void Player::StateInitialize()
{

	// ステートファクトリー
	playerStateFactory_ = PlayerStateFactory::GetInstance();

	// ステート
	playerState_.reset(playerStateFactory_->CreatePlayerState(kPlayerStateRoot)); // 最初のステート
	playerState_->Initialize();

	// ステート番号
	currentStateNo_ = PlayerState::kPlayerStateRoot; // 最初のステート
	prevStateNo_ = PlayerState::kPlayerStateRoot; // 最初のステート
	nextStateNo_ = PlayerState::kPlayerStateRoot; // 最初のステート
	playerState_->SetPlayer(this); // プレイヤーセット

}

void Player::StateUpdate()
{

	// ステートのチェック
	prevStateNo_ = currentStateNo_;
	if (receiveCommand_) {
		currentStateNo_ = nextStateNo_;
	}
	else {
		currentStateNo_ = playerState_->GetPlaryerStateNo();
	}

	// ステートが変わったか
	if (prevStateNo_ != currentStateNo_) {
		//ステート変更（初期化）
		playerState_.reset(playerStateFactory_->CreatePlayerState(currentStateNo_));
		playerState_->Initialize();
	}

	// ステート更新
	playerState_->Update();

}

void Player::PartInitialize()
{

	// 現在のモーション番号
	currentMotionNo_ = PlayerMotionIndex::kPlayerMotionStand;

	// 前のモーション番号
	prevMotionNo_ = PlayerMotionIndex::kPlayerMotionStand;

	// アニメーションカウント
	animationCount_ = 0u;

	// アニメーションカウント上限
	animationCountLimit_ = 0u;

	// モーション名をstd::vectorに
	std::vector<std::string> motionNames;
	for (uint32_t i = 0; i < PlayerMotionIndex::kPlayerMotionIndexOfCount; ++i) {
		motionNames.push_back(motionNames_[i]);
	}
	for (uint32_t i = 0; i < PlayerPartIndex::kPlayerPartIndexOfCount; ++i) {
		parts_[i] = std::make_unique<IBone>();
		parts_[i]->Initialize(models_[i], objectName_, partNames_[i], motionNames);
		parts_[i]->animationTransformChange(motionNames[currentMotionNo_]);
	}

	// 親子付け
	PartParent();

}

void Player::PartUpdate()
{

	// モーションのチェック
	prevMotionNo_ = currentMotionNo_;
	currentMotionNo_ = playerState_->GetPlaryerMotionNo();

	// モーションが変わったか
	if (prevMotionNo_ != currentMotionNo_) {
		//モーション変更（初期化）
		for (uint32_t i = 0; i < PlayerPartIndex::kPlayerPartIndexOfCount; ++i) {
			animationCountLimit_ = parts_[i]->animationTransformChange(motionNames_[currentMotionNo_]);
			animationCount_ = 0u;
		}
	}

	// アニメーション
	std::vector<std::string> motionNames;
	for (uint32_t i = 0; i < PlayerMotionIndex::kPlayerMotionIndexOfCount; ++i) {
		motionNames.push_back(motionNames_[i]);
	}
	for (uint32_t i = 0; i < PlayerPartIndex::kPlayerPartIndexOfCount; ++i) {
		parts_[i]->Update(animationCount_, motionNames);
#ifdef _DEBUG
		parts_[i]->animationTransformChange(motionNames[currentMotionNo_]);
#endif // _DEBUG
	}

	// アニメーションカウント
	if (animationCountLimit_ > 0) {
		animationCount_ = (animationCount_ + 1) % animationCountLimit_;
	}

}

void Player::PartParent()
{

	parts_[kPlayerPartHead]->GetWorldTransformAdress()->SetParent(parts_[kPlayerPartTorso]->GetWorldTransformAdress());
	parts_[kPlayerPartTorso]->GetWorldTransformAdress()->SetParent(&worldTransform_);
	parts_[kPlayerPartLowerBack]->GetWorldTransformAdress()->SetParent(&worldTransform_);

	parts_[kPlayerPartLeftUpperArm]->GetWorldTransformAdress()->SetParent(parts_[kPlayerPartTorso]->GetWorldTransformAdress());
	parts_[kPlayerPartLeftForearm]->GetWorldTransformAdress()->SetParent(parts_[kPlayerPartLeftUpperArm]->GetWorldTransformAdress());
	parts_[kPlayerPartLeftHand]->GetWorldTransformAdress()->SetParent(parts_[kPlayerPartLeftForearm]->GetWorldTransformAdress());
	parts_[kPlayerPartRightUpperArm]->GetWorldTransformAdress()->SetParent(parts_[kPlayerPartTorso]->GetWorldTransformAdress());
	parts_[kPlayerPartRightForearm]->GetWorldTransformAdress()->SetParent(parts_[kPlayerPartRightUpperArm]->GetWorldTransformAdress());
	parts_[kPlayerPartRightHand]->GetWorldTransformAdress()->SetParent(parts_[kPlayerPartRightForearm]->GetWorldTransformAdress());

	parts_[kPlayerPartLeftThigh]->GetWorldTransformAdress()->SetParent(parts_[kPlayerPartLowerBack]->GetWorldTransformAdress());
	parts_[kPlayerPartLeftShin]->GetWorldTransformAdress()->SetParent(parts_[kPlayerPartLeftThigh]->GetWorldTransformAdress());
	parts_[kPlayerPartLeftAnkle]->GetWorldTransformAdress()->SetParent(parts_[kPlayerPartLeftShin]->GetWorldTransformAdress());
	parts_[kPlayerPartRightThigh]->GetWorldTransformAdress()->SetParent(parts_[kPlayerPartLowerBack]->GetWorldTransformAdress());
	parts_[kPlayerPartRightShin]->GetWorldTransformAdress()->SetParent(parts_[kPlayerPartRightThigh]->GetWorldTransformAdress());
	parts_[kPlayerPartRightAnkle]->GetWorldTransformAdress()->SetParent(parts_[kPlayerPartRightShin]->GetWorldTransformAdress());

	for (uint32_t i = 0; i < PlayerPartIndex::kPlayerPartIndexOfCount; ++i) {
		parts_[i]->GetWorldTransformAdress()->UpdateMatrix();
	}

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
	Vector3 diff = { 0.0f,0.0f,0.0f };

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

void Player::ColliderUpdate()
{

	//計算用
	Segment segment = {
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
	};
	Vector3 diff = { 0.0f,0.0f,0.0f };

	// 頭
	diff = Vector3Calc::Subtract(parts_[kPlayerPartTorso]->GetWorldPosition(), parts_[kPlayerPartHead]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartHead]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderHead].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kPlayerColliderHead].get())->radius_ = colliderRadiuses_[kPlayerColliderHead];
	static_cast<Capsule*>(colliders_[kPlayerColliderHead].get())->worldTransformUpdate();

	// 胴
	diff = Vector3Calc::Subtract(parts_[kPlayerPartLowerBack]->GetWorldPosition(), parts_[kPlayerPartTorso]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartTorso]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderTorso].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kPlayerColliderTorso].get())->radius_ = colliderRadiuses_[kPlayerColliderTorso];
	static_cast<Capsule*>(colliders_[kPlayerColliderTorso].get())->worldTransformUpdate();

	// 左上腕
	diff = Vector3Calc::Subtract(parts_[kPlayerPartLeftForearm]->GetWorldPosition(), parts_[kPlayerPartLeftUpperArm]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartLeftUpperArm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftUpperArm].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftUpperArm].get())->radius_ = colliderRadiuses_[kPlayerColliderLeftUpperArm];
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftUpperArm].get())->worldTransformUpdate();

	// 左前腕 
	diff = Vector3Calc::Subtract(parts_[kPlayerPartLeftHand]->GetWorldPosition(), parts_[kPlayerPartLeftForearm]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartLeftForearm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftForearm].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftForearm].get())->radius_ = colliderRadiuses_[kPlayerColliderLeftForearm];
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftForearm].get())->worldTransformUpdate();

	// 右上腕
	diff = Vector3Calc::Subtract(parts_[kPlayerPartRightForearm]->GetWorldPosition(), parts_[kPlayerPartRightUpperArm]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartRightUpperArm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderRightUpperArm].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kPlayerColliderRightUpperArm].get())->radius_ = colliderRadiuses_[kPlayerColliderRightUpperArm];
	static_cast<Capsule*>(colliders_[kPlayerColliderRightUpperArm].get())->worldTransformUpdate();

	// 右前腕 
	diff = Vector3Calc::Subtract(parts_[kPlayerPartRightHand]->GetWorldPosition(), parts_[kPlayerPartRightForearm]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartRightForearm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderRightForearm].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kPlayerColliderRightForearm].get())->radius_ = colliderRadiuses_[kPlayerColliderRightForearm];
	static_cast<Capsule*>(colliders_[kPlayerColliderRightForearm].get())->worldTransformUpdate();

	// 左太もも
	diff = Vector3Calc::Subtract(parts_[kPlayerPartLeftShin]->GetWorldPosition(), parts_[kPlayerPartLeftThigh]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartLeftThigh]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftThigh].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftThigh].get())->radius_ = colliderRadiuses_[kPlayerColliderLeftThigh];
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftThigh].get())->worldTransformUpdate();

	// 左すね
	diff = Vector3Calc::Subtract(parts_[kPlayerPartLeftAnkle]->GetWorldPosition(), parts_[kPlayerPartLeftShin]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartLeftShin]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftShin].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftShin].get())->radius_ = colliderRadiuses_[kPlayerColliderLeftShin];
	static_cast<Capsule*>(colliders_[kPlayerColliderLeftShin].get())->worldTransformUpdate();

	// 右太もも
	diff = Vector3Calc::Subtract(parts_[kPlayerPartRightShin]->GetWorldPosition(), parts_[kPlayerPartRightThigh]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartRightThigh]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderRightThigh].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kPlayerColliderRightThigh].get())->radius_ = colliderRadiuses_[kPlayerColliderRightThigh];
	static_cast<Capsule*>(colliders_[kPlayerColliderRightThigh].get())->worldTransformUpdate();

	// 右すね
	diff = Vector3Calc::Subtract(parts_[kPlayerPartRightAnkle]->GetWorldPosition(), parts_[kPlayerPartRightShin]->GetWorldPosition());
	segment.Initialize(parts_[kPlayerPartRightShin]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kPlayerColliderRightShin].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kPlayerColliderRightShin].get())->radius_ = colliderRadiuses_[kPlayerColliderRightShin];
	static_cast<Capsule*>(colliders_[kPlayerColliderRightShin].get())->worldTransformUpdate();

}
