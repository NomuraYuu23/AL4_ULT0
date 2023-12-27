#include "PlayerStateFactory.h"
#include "../PlayerState/PlayerStateRoot/PlayerStateRoot.h"
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
	case kPlayerStateRoot:  // 立っている状態
		newPlayerState = new PlayerStateRoot();
		break;
	case kPlayerStateOfCount: // 使用不可
	default:
		assert(0);
		break;
	}

	return newPlayerState;
}
