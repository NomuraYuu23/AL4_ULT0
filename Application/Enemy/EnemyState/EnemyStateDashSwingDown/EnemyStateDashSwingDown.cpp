#include "EnemyStateDashSwingDown.h"
#include "../../Enemy.h" 
#include "../../../Player/Player.h"
#include "../../../../Engine/Math/Ease.h"

const EnemyStateDashSwingDown::ConstAttack EnemyStateDashSwingDown::kConstAttak = {
		{ 10, 20, 30, 10, 35}, { 0.0f, 6.0f, 0.0f, 0.2f, 0.0f} 
};

void EnemyStateDashSwingDown::Initialize()
{

	targetDirection_ = { 0.0f, 0.0f, 0.0f };

	targetAngleT_ = 0.1f;

	// あたり判定コライダー
	attackCollider_ = std::make_unique<Capsule>();

	// あたり判定ワールドトランスフォーム
	attackWorldTransform_.Initialize();
	attackWorldTransform_.parent_ = enemy_->GetPart(kEnemyPartRightHand)->GetWorldTransformAdress();
	attackWorldTransform_.UpdateMatrix();

	// 攻撃球の半径
	attackRadius_ = 30.0f;

	// 攻撃球と手の距離
	attackLength_ = { 0.0f, 0.0f, 10.0f };

	// 攻撃球のプレイヤーからのローカル位置
	attackCenter_ = { -10000.0f,-10000.0f,-10000.0f };

	// 前フレームの攻撃球
	prevAttackCenter_ = { -10000.0f,-10000.0f,-10000.0f };

	// 攻撃
	enemyAttack_ = std::make_unique<EnemyAttack>();

	// あたり判定コライダー初期化
	attackCollider_->Initialize(Segment{ attackCenter_ , {0.0f,0.0f,0.0f} }, attackRadius_, enemyAttack_.get());
	attackCollider_->SetCollisionAttribute(0x00000002);
	attackCollider_->SetCollisionMask(0xfffffffd);

	// あたり判定を取るか
	isAttackJudgment_ = false;

	// 媒介変数
	parameter_ = 0.0f;

	// フレーム数
	periodFrame_ = 10;

	// フェーズ
	inPhase_ = 0;

	AttackInitialize();

	enemyStateNo_ = EnemyState::kEnemyStateDashSwingDown;
	
	enemyAttack_->ClearContactRecord();

}

void EnemyStateDashSwingDown::Update()
{

	// 1フレームでのパラメータ加算値
	const float step = 1.0f / static_cast<float>(periodFrame_);

	// パラメータを1ステップ分加算
	parameter_ += step;

	Attack();

	AttackPhaseFinished();

	enemy_->SetReceiveCommand(false);

}

void EnemyStateDashSwingDown::AttackInitialize()
{

	// 攻撃行動用の媒介変数
	parameter_ = 0.0f;
	DontAttack();

	attackCollider_->worldTransformUpdate();

	periodFrame_ = kConstAttak.time_[inPhase_];

	//モーションネーム
	enemyMotionNo_ = kEnemyMotionDashSwingDown;

}

void EnemyStateDashSwingDown::Attack()
{

	Move();

	// コライダー更新
	if (inPhase_ == static_cast<uint32_t>(ComboPhase::kSwing) ||
		(inPhase_ == static_cast<uint32_t>(ComboPhase::kRecovery) && parameter_ <= 0.4f) ) {
		attackWorldTransform_.parent_ = enemy_->GetPart(kEnemyPartRightHand)->GetWorldTransformAdress();
		attackWorldTransform_.transform_.translate = attackLength_;
		attackWorldTransform_.UpdateMatrix();
		if (attackCenter_.x <= -10000.0f) {
			prevAttackCenter_ = attackWorldTransform_.GetWorldPosition();
		}
		else {
			prevAttackCenter_ = attackCenter_;
		}
		attackCenter_ = attackWorldTransform_.GetWorldPosition();
		Segment segment;
		segment.origin_ = attackCenter_;
		segment.diff_ = v3Calc_->Subtract(prevAttackCenter_, attackCenter_);
		attackCollider_->segment_ = segment;
		attackCollider_->radius_ = attackRadius_;
	}
	else {
		DontAttack();
	}

}

void EnemyStateDashSwingDown::DontAttack()
{

	isAttackJudgment_ = false;
	attackCenter_ = { -10000.0f,-10000.0f,-10000.0f };
	prevAttackCenter_ = { -10000.0f,-10000.0f,-10000.0f };
	attackCollider_->segment_.origin_ = attackCenter_;
	attackCollider_->segment_.diff_ = { 0.0f, 0.0f, 0.0f };
	attackCollider_->worldTransformUpdate();

}

void EnemyStateDashSwingDown::Move()
{

	WorldTransform* worldTransform = enemy_->GetWorldTransformAdress();

	worldTransform->usedDirection_ = true;

	Player* player = enemy_->GetPlayer();
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };

	WorldTransform* playerWorldTransform = player->GetWorldTransformAdress();

	Vector3 direction = worldTransform->direction_;
	// 向き
	if (inPhase_ <= kCharge) {
		direction = v3Calc_->Normalize(v3Calc_->Subtract(playerWorldTransform->GetWorldPosition(), worldTransform->GetWorldPosition()));
		Vector3 directionCheck = {
			std::fabsf(std::fabsf(direction.x) - std::fabsf(worldTransform->direction_.x)),
			std::fabsf(std::fabsf(direction.y) - std::fabsf(worldTransform->direction_.y)),
			std::fabsf(std::fabsf(direction.z) - std::fabsf(worldTransform->direction_.z)) };
		if (directionCheck.x > directionLimit.x) {
			if (direction.x < 0.0f) {
				direction.x = -directionLimit.x;
			}
			else {
				direction.x = directionLimit.x;
			}
		}
		if (directionCheck.y > directionLimit.y) {
			if (direction.y < 0.0f) {
				direction.y = -directionLimit.y;
			}
			else {
				direction.y = directionLimit.y;
			}
		}
		if (directionCheck.z > directionLimit.z) {
			if (direction.z < 0.0f) {
				direction.z = -directionLimit.z;
			}
			else {
				direction.z = directionLimit.z;
			}
		}
	}

	// 移動量
	Vector3 move = { 0.0f, 0.0f, 1.0f };

	// 移動量に速さを反映
	move = v3Calc_->Multiply(kConstAttak.speed_[inPhase_], v3Calc_->Normalize(move));

	Matrix4x4 rotateMatrix = m4Calc_->DirectionToDirection(Vector3{ 0.0f,0.0f,1.0f }, direction);

	// 移動ベクトルをカメラの角度だけ回転する
	move = m4Calc_->TransformNormal(move, rotateMatrix);

	// 移動
	velocity.x = move.x;
	velocity.z = move.z;

	// 移動方向に見た目を合わせる(Y軸)
	// あたん
	targetDirection_.x = v3Calc_->Normalize(move).x;
	targetDirection_.z = v3Calc_->Normalize(move).z;

	// 角度補間
	worldTransform->direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform->direction_, targetDirection_, targetAngleT_);

	worldTransform->transform_.translate = v3Calc_->Add(worldTransform->transform_.translate, velocity);

}

void EnemyStateDashSwingDown::AttackPhaseFinished()
{

	if (parameter_ >= 1.0f) {
		inPhase_++;
		AttackInitialize();
		if (inPhase_ == static_cast<uint32_t>(ComboPhase::kCountOfComboPhase)) {
			enemyStateNo_ = EnemyState::kEnemyStateRoot;
			DontAttack();
		}
		else if (inPhase_ >= static_cast<uint32_t>(ComboPhase::kSwing)) {
			isAttackJudgment_ = true;
		}
		else {
			DontAttack();
		}
	}

}
