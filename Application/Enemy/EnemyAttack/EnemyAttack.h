#pragma once
#include "../../../Engine/Collider/Capsule/Capsule.h"
#include "../../../Engine/Collision/CollisionData.h"
#include "../../ContactRecord/ContactRecord.h"

class EnemyAttack
{

public:

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

	/// <summary>
	/// 接触履歴抹消
	/// </summary>
	void ClearContactRecord();

	/// <summary>
	/// 接触履歴取得
	/// </summary>
	/// <returns></returns>
	ContactRecord& GetContactRecord() { return contactRecord_; }

private:

	/// <summary>
	/// 衝突処理 プレイヤー
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollisionPlayer(ColliderParentObject colliderPartner, const CollisionData& collisionData);

private:

	// 接触履歴
	ContactRecord contactRecord_;

};

