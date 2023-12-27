#include "PlayerStateRun.h"
#include "../../../../Engine/Input/Input.h"
#include "../../../../Engine/Math/Matrix4x4.h"
#include "../../Player.h"
#include "../../../../Engine/Math/Ease.h"

void PlayerStateRun::Initialize()
{

	speed_ = 5.0f;

	targetDirection_ = { 0.0f, 0.0f, 0.0f};

	targetAngleT_ = 0.1f;

	//plaryerMotionNo_ = kPlayerMotionRun;

}

void PlayerStateRun::Update()
{

	WorldTransform* worldTransform = player_->GetWorldTransformAdress();
	BaseCamera* camera = player_->GetCamera();
	Vector3 velocity = { 0.0f, 0.0f, 0.0f };

	worldTransform->usedDirection_ = true;

	//インスタンス
	Input* input = Input::GetInstance();
	Vector3Calc* v3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* m4Calc = Matrix4x4Calc::GetInstance();

	//移動
	if (input->GetJoystickConnected()) {

		const float kThreshold = 0.7f;
		bool isMoving = false;

		// 移動量
		Vector3 move = { input->GetLeftAnalogstick().x, 0.0f, -input->GetLeftAnalogstick().y };
		if (v3Calc->Length(move) > kThreshold) {
			isMoving = true;
		}

		if (isMoving) {

			// 移動量に速さを反映
			move = v3Calc->Multiply(speed_, v3Calc->Normalize(move));

			// カメラの角度から回転行列を計算する
			Matrix4x4 rotateMatrix = m4Calc->MakeRotateXYZMatrix(camera->GetTransform().rotate);

			// 移動ベクトルをカメラの角度だけ回転する
			move = m4Calc->TransformNormal(move, rotateMatrix);

			//　親がいれば
			if (worldTransform->parent_) {
				rotateMatrix = m4Calc->Inverse(worldTransform->parent_->rotateMatrix_);
				move = m4Calc->TransformNormal(move, rotateMatrix);
			}

			// 移動
			velocity.x = move.x;
			velocity.z = move.z;

			// 移動方向に見た目を合わせる(Y軸)
			// あたん
			targetDirection_.x = v3Calc->Normalize(move).x;
			targetDirection_.z = v3Calc->Normalize(move).z;

		}
		else {
			// 移動
			velocity.x = 0.0f;
			velocity.z = 0.0f;
		}

		// 角度補間
		worldTransform->direction_ = Ease::Easing(Ease::EaseName::Lerp, worldTransform->direction_, targetDirection_, targetAngleT_);
	}

	worldTransform->transform_.translate = v3Calc->Add(worldTransform->transform_.translate, velocity);

}
