#include "PlayerStateAvoidance.h"
#include "../../Player.h"
#include "../../../../Engine/Math/Ease.h"

void PlayerStateAvoidance::Initialize()
{

	spped_ = 20.0f;

	frame_ = 5;

	frameCount_ = 0;

	playerStateNo_ = PlayerState::kPlayerStateAvoidance;

}

void PlayerStateAvoidance::Update()
{

	Vector3 velocity = { 0.0f, 0.0f, 0.0f };

	WorldTransform* worldTransform = player_->GetWorldTransformAdress();

	Vector3 move = { 0.0f,0.0f,1.0f };

	// 移動量に速さを反映
	move = v3Calc_->Multiply(spped_, v3Calc_->Normalize(move));

	// 移動ベクトルをカメラの角度だけ回転する
	move = m4Calc_->TransformNormal(move, worldTransform->rotateMatrix_);

	// 移動
	velocity.x = move.x;
	velocity.z = move.z;

	worldTransform->transform_.translate = v3Calc_->Add(worldTransform->transform_.translate, velocity);

	if (++frameCount_ >= frame_) {
		playerStateNo_ = PlayerState::kPlayerStateRoot;
	}
	player_->SetReceiveCommand(false);

}
