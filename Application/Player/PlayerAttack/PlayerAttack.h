#pragma once
#include "../../../Engine/Collider/Capsule/Capsule.h"
#include "../../../Engine/Collision/CollisionData.h"

class PlayerAttack
{

public: 

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

private:

};

