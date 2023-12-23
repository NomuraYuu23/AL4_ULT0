#include "PlayerStateFactory.h"
#include "../PlayerState/PlayerStateStand/PlayerStateStand.h"
#include "../PlayerState/PlayerStateRun/PlayerStateRun.h"
#include <cassert>

PlayerStateFactory* PlayerStateFactory::GetInstance()
{
	static PlayerStateFactory instance;
	return &instance;
}

IPlayerState* PlayerStateFactory::CreatePlayerState(uint32_t playerStateName)
{
	// 次のシーンを生成
	IPlayerState* newPlayerState = nullptr;

	switch (playerStateName)
	{
	case kPlayerStateStand:  // 立っている状態
		newPlayerState = new PlayerStateStand();
		break;
	case kPlayerStateRun:  // 走っている状態
		newPlayerState = new PlayerStateRun();
		break;
	case kPlayerStateOfCount: // 使用不可
	default:
		assert(0);
		break;
	}

	return newPlayerState;
}
