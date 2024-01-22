#include "Enemy.h"
#include "../../Engine/Collider/Capsule/Capsule.h"
#include "../../Engine/2D/ImguiManager.h"

void Enemy::Initialize(const std::array<Model*, EnemyPartIndex::kEnemyPartIndexOfCount>& models, Model* weaponModel)
{

	// マテリアル
	material_.reset(Material::Create());

	// ワールドトランスフォーム
	worldTransform_.Initialize();
	worldTransform_.transform_.translate.y = height_;
	worldTransform_.UpdateMatrix();

	// コマンド
	enemyCommand_ = EnemyCommand::GetInstance();
	enemyCommand_->Initialize();

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

	// 武器
	weaponModel_ = weaponModel;
	weaponWorldTransfrom_.Initialize();
	weaponWorldTransfrom_.parent_ = parts_[kEnemyPartRightHand]->GetWorldTransformAdress();
	weaponWorldTransfrom_.transform_.rotate.x = 1.57f;
	weaponWorldTransfrom_.transform_.rotate.z = 1.57f;
	weaponWorldTransfrom_.UpdateMatrix();

}

void Enemy::Update()
{

	if (receiveCommand_) {
		nextStateNo_ = enemyCommand_->Command();
	}

	// ステート
	StateUpdate();

	worldTransform_.transform_.translate.y = height_;
	worldTransform_.UpdateMatrix();

	// パーツ,アニメーション
	PartUpdate();

	// コライダー
	ColliderUpdate();

	// 武器
	weaponWorldTransfrom_.UpdateMatrix();

}

void Enemy::Draw(BaseCamera& camera)
{

	for (uint32_t i = 0; i < EnemyPartIndex::kEnemyPartIndexOfCount; ++i) {
		parts_[i]->Draw(camera, material_.get());
	}

	weaponModel_->Draw(weaponWorldTransfrom_, camera);

}

void Enemy::ImGuiDraw()
{
}

void Enemy::OnCollision(ColliderParentObject colliderPartner, CollisionData collisionData)
{


}

void Enemy::StateInitialize()
{

	// ステートファクトリー
	enemyStateFactory_ = EnemyStateFactory::GetInstance();

	// ステート
	enemyState_.reset(enemyStateFactory_->CreateEnemyState(kEnemyStateRoot)); // 最初のステート
	enemyState_->Initialize();

	// ステート番号
	currentStateNo_ = EnemyState::kEnemyStateRoot; // 最初のステート
	prevStateNo_ = EnemyState::kEnemyStateRoot; // 最初のステート
	nextStateNo_ = EnemyState::kEnemyStateRoot; // 最初のステート
	enemyState_->SetEnemy(this); // エネミーセット

}

void Enemy::StateUpdate()
{

	// ステートのチェック
	prevStateNo_ = currentStateNo_;
	if (receiveCommand_) {
		currentStateNo_ = nextStateNo_;
	}
	else {
		currentStateNo_ = enemyState_->GetEnemyStateNo();
	}

	// ステートが変わったか
	if (prevStateNo_ != currentStateNo_) {
		//ステート変更（初期化）
		enemyState_.reset(enemyStateFactory_->CreateEnemyState(currentStateNo_));
		enemyState_->Initialize();
	}

	// ステート更新
	enemyState_->Update();

}

void Enemy::PartInitialize()
{

	// 現在のモーション番号
	currentMotionNo_ = EnemyMotionIndex::kEnemyMotionStand;

	// 前のモーション番号
	prevMotionNo_ = EnemyMotionIndex::kEnemyMotionStand;

	// アニメーションカウント
	animationCount_ = 0u;

	// アニメーションカウント上限
	animationCountLimit_ = 0u;

	// モーション名をstd::vectorに
	std::vector<std::string> motionNames;
	for (uint32_t i = 0; i < EnemyMotionIndex::kEnemyMotionIndexOfCount; ++i) {
		motionNames.push_back(motionNames_[i]);
	}
	for (uint32_t i = 0; i < EnemyPartIndex::kEnemyPartIndexOfCount; ++i) {
		parts_[i] = std::make_unique<IBone>();
		parts_[i]->Initialize(models_[i], objectName_, partNames_[i], motionNames);
		parts_[i]->animationTransformChange(motionNames[currentMotionNo_]);
	}

	// 親子付け
	PartParent();

}

void Enemy::PartUpdate()
{

	// モーションのチェック
	prevMotionNo_ = currentMotionNo_;
	currentMotionNo_ = enemyState_->GetEnemyMotionNo();

	// モーションが変わったか
	if (prevMotionNo_ != currentMotionNo_) {
		//モーション変更（初期化）
		for (uint32_t i = 0; i < EnemyPartIndex::kEnemyPartIndexOfCount; ++i) {
			animationCountLimit_ = parts_[i]->animationTransformChange(motionNames_[currentMotionNo_]);
			animationCount_ = 0u;
		}
	}

	// アニメーション
	std::vector<std::string> motionNames;
	for (uint32_t i = 0; i < EnemyMotionIndex::kEnemyMotionIndexOfCount; ++i) {
		motionNames.push_back(motionNames_[i]);
	}
	for (uint32_t i = 0; i < EnemyPartIndex::kEnemyPartIndexOfCount; ++i) {
		parts_[i]->Update(animationCount_, motionNames);
#ifdef _DEBUG
		parts_[i]->animationTransformChange(motionNames[currentMotionNo_]);
#endif // _DEBUG
	}

	// アニメーションカウント
	if (animationCountLimit_ > 0) {
		animationCount_ = (animationCount_ + 1) % (animationCountLimit_ + 1);
		//でばっぐ
		//animationCount_ = 70;
	}

}

void Enemy::PartParent()
{

	parts_[kEnemyPartHead]->GetWorldTransformAdress()->SetParent(parts_[kEnemyPartTorso]->GetWorldTransformAdress());
	parts_[kEnemyPartTorso]->GetWorldTransformAdress()->SetParent(&worldTransform_);
	parts_[kEnemyPartLowerBack]->GetWorldTransformAdress()->SetParent(&worldTransform_);

	parts_[kEnemyPartLeftUpperArm]->GetWorldTransformAdress()->SetParent(parts_[kEnemyPartTorso]->GetWorldTransformAdress());
	parts_[kEnemyPartLeftForearm]->GetWorldTransformAdress()->SetParent(parts_[kEnemyPartLeftUpperArm]->GetWorldTransformAdress());
	parts_[kEnemyPartLeftHand]->GetWorldTransformAdress()->SetParent(parts_[kEnemyPartLeftForearm]->GetWorldTransformAdress());
	parts_[kEnemyPartRightUpperArm]->GetWorldTransformAdress()->SetParent(parts_[kEnemyPartTorso]->GetWorldTransformAdress());
	parts_[kEnemyPartRightForearm]->GetWorldTransformAdress()->SetParent(parts_[kEnemyPartRightUpperArm]->GetWorldTransformAdress());
	parts_[kEnemyPartRightHand]->GetWorldTransformAdress()->SetParent(parts_[kEnemyPartRightForearm]->GetWorldTransformAdress());

	parts_[kEnemyPartLeftThigh]->GetWorldTransformAdress()->SetParent(parts_[kEnemyPartLowerBack]->GetWorldTransformAdress());
	parts_[kEnemyPartLeftShin]->GetWorldTransformAdress()->SetParent(parts_[kEnemyPartLeftThigh]->GetWorldTransformAdress());
	parts_[kEnemyPartLeftAnkle]->GetWorldTransformAdress()->SetParent(parts_[kEnemyPartLeftShin]->GetWorldTransformAdress());
	parts_[kEnemyPartRightThigh]->GetWorldTransformAdress()->SetParent(parts_[kEnemyPartLowerBack]->GetWorldTransformAdress());
	parts_[kEnemyPartRightShin]->GetWorldTransformAdress()->SetParent(parts_[kEnemyPartRightThigh]->GetWorldTransformAdress());
	parts_[kEnemyPartRightAnkle]->GetWorldTransformAdress()->SetParent(parts_[kEnemyPartRightShin]->GetWorldTransformAdress());

	for (uint32_t i = 0; i < EnemyPartIndex::kEnemyPartIndexOfCount; ++i) {
		parts_[i]->GetWorldTransformAdress()->UpdateMatrix();
	}

}

void Enemy::ColliderInitialize()
{

	// コライダー
	for (uint32_t i = 0; i < EnemyColliderIndex::kEnemyColliderIndexOfCount; ++i) {
		colliders_[i] = std::make_unique<Capsule>();
		colliders_[i]->SetCollisionAttribute(collisionAttribute_);
		colliders_[i]->SetCollisionMask(collisionMask_);
		colliderRadiuses_[i] = 1.0f;
	}

	//計算用
	Segment segment = {
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
	};
	Vector3 diff = { 0.0f,0.0f,0.0f };

	// 頭
	diff = Vector3Calc::Subtract(parts_[kEnemyPartTorso]->GetWorldPosition(), parts_[kEnemyPartHead]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartHead]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderHead].get())->Initialize(segment, colliderRadiuses_[kEnemyColliderHead], this);

	// 胴
	diff = Vector3Calc::Subtract(parts_[kEnemyPartLowerBack]->GetWorldPosition(), parts_[kEnemyPartTorso]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartTorso]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderTorso].get())->Initialize(segment, colliderRadiuses_[kEnemyColliderTorso], this);

	// 左上腕
	diff = Vector3Calc::Subtract(parts_[kEnemyPartLeftForearm]->GetWorldPosition(), parts_[kEnemyPartLeftUpperArm]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartLeftUpperArm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftUpperArm].get())->Initialize(segment, colliderRadiuses_[kEnemyColliderLeftUpperArm], this);

	// 左前腕 
	diff = Vector3Calc::Subtract(parts_[kEnemyPartLeftHand]->GetWorldPosition(), parts_[kEnemyPartLeftForearm]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartLeftForearm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftForearm].get())->Initialize(segment, colliderRadiuses_[kEnemyColliderLeftForearm], this);

	// 右上腕
	diff = Vector3Calc::Subtract(parts_[kEnemyPartRightForearm]->GetWorldPosition(), parts_[kEnemyPartRightUpperArm]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartRightUpperArm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderRightUpperArm].get())->Initialize(segment, colliderRadiuses_[kEnemyColliderRightUpperArm], this);

	// 右前腕 
	diff = Vector3Calc::Subtract(parts_[kEnemyPartRightHand]->GetWorldPosition(), parts_[kEnemyPartRightForearm]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartRightForearm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderRightForearm].get())->Initialize(segment, colliderRadiuses_[kEnemyColliderRightForearm], this);

	// 左太もも
	diff = Vector3Calc::Subtract(parts_[kEnemyPartLeftShin]->GetWorldPosition(), parts_[kEnemyPartLeftThigh]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartLeftThigh]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftThigh].get())->Initialize(segment, colliderRadiuses_[kEnemyColliderLeftThigh], this);

	// 左すね
	diff = Vector3Calc::Subtract(parts_[kEnemyPartLeftAnkle]->GetWorldPosition(), parts_[kEnemyPartLeftShin]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartLeftShin]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftShin].get())->Initialize(segment, colliderRadiuses_[kEnemyColliderLeftShin], this);

	// 右太もも
	diff = Vector3Calc::Subtract(parts_[kEnemyPartRightShin]->GetWorldPosition(), parts_[kEnemyPartRightThigh]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartRightThigh]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderRightThigh].get())->Initialize(segment, colliderRadiuses_[kEnemyColliderRightThigh], this);

	// 右すね
	diff = Vector3Calc::Subtract(parts_[kEnemyPartRightAnkle]->GetWorldPosition(), parts_[kEnemyPartRightShin]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartRightShin]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderRightShin].get())->Initialize(segment, colliderRadiuses_[kEnemyColliderRightShin], this);

}

void Enemy::ColliderUpdate()
{

	//計算用
	Segment segment = {
		{ 0.0f,0.0f,0.0f },
		{ 0.0f,0.0f,0.0f },
	};
	Vector3 diff = { 0.0f,0.0f,0.0f };

	// 頭
	diff = Vector3Calc::Subtract(parts_[kEnemyPartTorso]->GetWorldPosition(), parts_[kEnemyPartHead]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartHead]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderHead].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kEnemyColliderHead].get())->radius_ = colliderRadiuses_[kEnemyColliderHead];
	static_cast<Capsule*>(colliders_[kEnemyColliderHead].get())->worldTransformUpdate();

	// 胴
	diff = Vector3Calc::Subtract(parts_[kEnemyPartLowerBack]->GetWorldPosition(), parts_[kEnemyPartTorso]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartTorso]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderTorso].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kEnemyColliderTorso].get())->radius_ = colliderRadiuses_[kEnemyColliderTorso];
	static_cast<Capsule*>(colliders_[kEnemyColliderTorso].get())->worldTransformUpdate();

	// 左上腕
	diff = Vector3Calc::Subtract(parts_[kEnemyPartLeftForearm]->GetWorldPosition(), parts_[kEnemyPartLeftUpperArm]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartLeftUpperArm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftUpperArm].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftUpperArm].get())->radius_ = colliderRadiuses_[kEnemyColliderLeftUpperArm];
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftUpperArm].get())->worldTransformUpdate();

	// 左前腕 
	diff = Vector3Calc::Subtract(parts_[kEnemyPartLeftHand]->GetWorldPosition(), parts_[kEnemyPartLeftForearm]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartLeftForearm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftForearm].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftForearm].get())->radius_ = colliderRadiuses_[kEnemyColliderLeftForearm];
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftForearm].get())->worldTransformUpdate();

	// 右上腕
	diff = Vector3Calc::Subtract(parts_[kEnemyPartRightForearm]->GetWorldPosition(), parts_[kEnemyPartRightUpperArm]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartRightUpperArm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderRightUpperArm].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kEnemyColliderRightUpperArm].get())->radius_ = colliderRadiuses_[kEnemyColliderRightUpperArm];
	static_cast<Capsule*>(colliders_[kEnemyColliderRightUpperArm].get())->worldTransformUpdate();

	// 右前腕 
	diff = Vector3Calc::Subtract(parts_[kEnemyPartRightHand]->GetWorldPosition(), parts_[kEnemyPartRightForearm]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartRightForearm]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderRightForearm].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kEnemyColliderRightForearm].get())->radius_ = colliderRadiuses_[kEnemyColliderRightForearm];
	static_cast<Capsule*>(colliders_[kEnemyColliderRightForearm].get())->worldTransformUpdate();

	// 左太もも
	diff = Vector3Calc::Subtract(parts_[kEnemyPartLeftShin]->GetWorldPosition(), parts_[kEnemyPartLeftThigh]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartLeftThigh]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftThigh].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftThigh].get())->radius_ = colliderRadiuses_[kEnemyColliderLeftThigh];
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftThigh].get())->worldTransformUpdate();

	// 左すね
	diff = Vector3Calc::Subtract(parts_[kEnemyPartLeftAnkle]->GetWorldPosition(), parts_[kEnemyPartLeftShin]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartLeftShin]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftShin].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftShin].get())->radius_ = colliderRadiuses_[kEnemyColliderLeftShin];
	static_cast<Capsule*>(colliders_[kEnemyColliderLeftShin].get())->worldTransformUpdate();

	// 右太もも
	diff = Vector3Calc::Subtract(parts_[kEnemyPartRightShin]->GetWorldPosition(), parts_[kEnemyPartRightThigh]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartRightThigh]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderRightThigh].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kEnemyColliderRightThigh].get())->radius_ = colliderRadiuses_[kEnemyColliderRightThigh];
	static_cast<Capsule*>(colliders_[kEnemyColliderRightThigh].get())->worldTransformUpdate();

	// 右すね
	diff = Vector3Calc::Subtract(parts_[kEnemyPartRightAnkle]->GetWorldPosition(), parts_[kEnemyPartRightShin]->GetWorldPosition());
	segment.Initialize(parts_[kEnemyPartRightShin]->GetWorldPosition(), diff);
	static_cast<Capsule*>(colliders_[kEnemyColliderRightShin].get())->segment_ = segment;
	static_cast<Capsule*>(colliders_[kEnemyColliderRightShin].get())->radius_ = colliderRadiuses_[kEnemyColliderRightShin];
	static_cast<Capsule*>(colliders_[kEnemyColliderRightShin].get())->worldTransformUpdate();

}

std::array<ColliderShape, EnemyColliderIndex::kEnemyColliderIndexOfCount> Enemy::GetCollider()
{

	std::array<ColliderShape, EnemyColliderIndex::kEnemyColliderIndexOfCount> result;

	for (uint32_t i = 0; i < EnemyColliderIndex::kEnemyColliderIndexOfCount; ++i) {
		result[i] = colliders_[i].get();
	}

	return result;

}
