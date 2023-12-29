#include "PlayerCommand.h"
#include "../PlayerState/IPlayerState.h"

Input* PlayerCommand::input_ = Input::GetInstance();

PlayerCommand* PlayerCommand::GetInstance()
{
	static PlayerCommand instance;
	return &instance;
}

void PlayerCommand::Initialize()
{

	// 回避が押されてからフレーム
	avoidanceFrameCount_ = 0;

	// 回避できるフレーム
	canBeAvoidanceFrame_ = 10;

	// フレームカウントするか
	isAvoidanceFrameCount_ = false;

}

uint32_t PlayerCommand::Command()
{

	uint32_t resultState = PlayerState::kPlayerStateRoot;

	// 回避
	if (input_->ReleaseJoystick(JoystickButton::kJoystickButtonB)) {
		isAvoidanceFrameCount_ = false;
		if (avoidanceFrameCount_ <= canBeAvoidanceFrame_) {
			// ステートを回避に
			resultState = PlayerState::kPlayerStateAvoidance;
		}
		else {
			// 通常
			resultState = PlayerState::kPlayerStateRoot;
		}

	}
	// 回復
	else if(input_->ReleaseJoystick(JoystickButton::kJoystickButtonX)){
		resultState = PlayerState::kPlayerStateRecovery;
	}

	// 通常
	else{

		// 回避のフレームカウント開始
		if (input_->TriggerJoystick(JoystickButton::kJoystickButtonB) && !isAvoidanceFrameCount_) {
			isAvoidanceFrameCount_ = true;
			avoidanceFrameCount_ = 0u;
		}
		else if (isAvoidanceFrameCount_) {
			if (avoidanceFrameCount_ > canBeAvoidanceFrame_) {
				avoidanceFrameCount_ = canBeAvoidanceFrame_ + 1;
			}
			else {
				avoidanceFrameCount_++;
			}
		}

		resultState = PlayerState::kPlayerStateRoot;

	}

    return resultState;
}
