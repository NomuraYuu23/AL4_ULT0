#include "EnemyAttack.h"
#include "../../Player/Player.h"

void EnemyAttack::OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	if (std::holds_alternative<Player*>(colliderPartner)) {

		OnCollisionPlayer(colliderPartner, collisionData);
	}

}

void EnemyAttack::ClearContactRecord()
{

	contactRecord_.Clear();

}

void EnemyAttack::OnCollisionPlayer(ColliderParentObject colliderPartner, const CollisionData& collisionData)
{

	Player* player = std::get<Player*>(colliderPartner);
	uint32_t serialNumber = player->GetSerialNumber();

	// プレイヤーが回避か
	if (player->GetCurrentStateNo() == kPlayerStateAvoidance) {
		return;
	}

	// 履歴確認
	if (contactRecord_.ConfirmHistory(serialNumber)) {
		return;
	}

	// 履歴登録
	contactRecord_.AddHistory(serialNumber);

	// 衝突処理
	player->Damage(damage_);

}
