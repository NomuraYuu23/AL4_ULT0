#include "PlayerStateFactory.h"

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
		newPlayerState;
		break;
	case kPlayerStateOfCount: // 使用不可
		break;
	default:
		break;
	}

	return newPlayerState;
}
