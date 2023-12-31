#include "PlayerStateAttack.h"
#include "../../Player.h"
#include "../../../../Engine/Math/Ease.h"

// コンボ定数表
const std::array<PlayerStateAttack::ConstAttack, PlayerStateAttack::kComboNum> PlayerStateAttack::kConstAttaks = {
	{
		{ { 16, 16, 10, 10}, { 2.0f, 0.0f, 0.0f, 0.0f} },
		{ { 16, 16, 10, 10}, { 2.0f, 0.0f, 0.0f, 0.0f} },
	}
};

void PlayerStateAttack::Initialize()
{

	targetDirection_ = { 0.0f, 0.0f, 0.0f };

	targetAngleT_ = 0.1f;

	// あたり判定コライダー
	attackCollider_ = std::make_unique<Capsule>();

	// 攻撃球の半径
	attackRadius_ = 1.0f;

	// 攻撃球のプレイヤーからのローカル位置
	attackCenter_ = { -10000.0f,-10000.0f,-10000.0f };

	// 前フレームの攻撃球
	prevAttackCenter_ = { -10000.0f,-10000.0f,-10000.0f };

	// あたり判定コライダー初期化
	attackCollider_->Initialize(Segment{ attackCenter_ , {0.0f,0.0f,0.0f} }, attackRadius_, nullptr);

	// あたり判定を取るか
	isAttackJudgment_ = false;

	// 媒介変数
	parameter_ = 0.0f;

	// フレーム数
	periodFrame_ = 10;

	// コンボインデックス
	comboIndex_ = 0;

	// コンボフェーズ
	inComboPhase_ = 0;

	// コンボネクスト
	comboNext_ = false;

	AttackInitialize();

	playerStateNo_ = PlayerState::kPlayerStateAttack;

}

void PlayerStateAttack::Update()
{

	// 1フレームでのパラメータ加算値
	const float step = 1.0f / static_cast<float>(periodFrame_);

	// パラメータを1ステップ分加算
	parameter_ += step;

	if (comboIndex_ == 0) {
		AttackCombo1st();
	}
	else {
		AttackCombo2nd();
	}

	if (parameter_ > step) {
		AttackComboContinuationJudgment();
	}
	AttackComboPhaseFinished();

	player_->SetReceiveCommand(false);

}

void PlayerStateAttack::AttackInitialize()
{

	// 攻撃行動用の媒介変数
	parameter_ = 0.0f;
	DontAttack();

	attackCollider_->worldTransformUpdate();

	periodFrame_ = kConstAttaks[comboIndex_].time_[inComboPhase_];

	//モーションネーム
	if (comboIndex_ == 0) {
		playerMotionNo_ = kPlayerMotionAttack1st;
	}
	else {
		playerMotionNo_ = kPlayerMotionAttack2nd;
	}

}

void PlayerStateAttack::AttackComboContinuationJudgment()
{

	Input* input = Input::GetInstance();

	// ここに追加でスタミナ処理
	if (input->TriggerJoystick(kJoystickButtonRB)) {
		comboNext_ = true;
	}

}

void PlayerStateAttack::AttackComboPhaseFinished()
{

	if (parameter_ >= 1.0f) {
		inComboPhase_++;
		AttackInitialize();
		if (inComboPhase_ == static_cast<uint32_t>(ComboPhase::kCountOfComboPhase)) {
			AttackComboFinished();
			DontAttack();
		}
		else if (inComboPhase_ >= static_cast<uint32_t>(ComboPhase::kSwing)) {
			isAttackJudgment_ = true;
		}
		else {
			DontAttack();
		}
	}

}

void PlayerStateAttack::AttackComboFinished()
{

	inComboPhase_ = 0;
	// コンボ継続か
	if (comboNext_) {
		//フラグリセット
		comboNext_ = false;
		if (comboIndex_ < kComboNum - 1) {
			comboIndex_++;
		}
		else {
			comboIndex_ = 0;
		}
		AttackInitialize();
	}
	else {
		playerStateNo_ = PlayerState::kPlayerStateRoot;
	}

}

void PlayerStateAttack::AttackCombo1st()
{

	Move();

	// コライダー更新

}

void PlayerStateAttack::AttackCombo2nd()
{

	Move();

	// コライダー更新

}

void PlayerStateAttack::DontAttack()
{

	isAttackJudgment_ = false;
	attackCenter_ = { -10000.0f,-10000.0f,-10000.0f };
	prevAttackCenter_ = { -10000.0f,-10000.0f,-10000.0f };
	attackCollider_->segment_.origin_ = attackCenter_;
	attackCollider_->segment_.diff_ = { 0.0f, 0.0f, 0.0f };
	attackCollider_->worldTransformUpdate();

}

void PlayerStateAttack::Move()
{

	WorldTransform* worldTransform = player_->GetWorldTransformAdress();

	worldTransform->usedDirection_ = true;

	BaseCamera* camera = player_->GetCamera();
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };

	// 移動量
	Vector3 move = { 0.0f, 0.0f, 1.0f };

	// 移動量に速さを反映
	move = v3Calc_->Multiply(kConstAttaks[comboIndex_].speed_[inComboPhase_], v3Calc_->Normalize(move));

	// カメラの角度から回転行列を計算する
	Matrix4x4 rotateMatrix = m4Calc_->MakeRotateXYZMatrix(camera->GetTransform().rotate);

	// 移動ベクトルをカメラの角度だけ回転する
	move = m4Calc_->TransformNormal(move, rotateMatrix);

	//　親がいれば
	if (worldTransform->parent_) {
		rotateMatrix = m4Calc_->Inverse(worldTransform->parent_->rotateMatrix_);
		move = m4Calc_->TransformNormal(move, rotateMatrix);
	}

	// 移動
	velocity.x = move.x;
	velocity.z = move.z;

	// 移動方向に見た目を合わせる(Y軸)
	// あたん
	targetDirection_.x = v3Calc_->Normalize(move).x;
	targetDirection_.z = v3Calc_->Normalize(move).z;

	worldTransform->transform_.translate = v3Calc_->Add(worldTransform->transform_.translate, velocity);

}
