#include "EnemyStateFactory.h"
#include <cassert>

EnemyStateFactory* EnemyStateFactory::GetInstance()
{
	static EnemyStateFactory instance;
	return &instance;
}

IEnemyState* EnemyStateFactory::CreateEnemyState(uint32_t enemyStateName)
{
	// 次のシーンを生成
	IEnemyState* newEnemyState = nullptr;

	switch (enemyStateName)
	{
	case kEnemyStateRoot: // 通常
		//newEnemyState = new ();
		break;
	case kEnemyStateOfCount: // 使用不可
	default:
		assert(0);
		break;
	}

	return newEnemyState;
}
