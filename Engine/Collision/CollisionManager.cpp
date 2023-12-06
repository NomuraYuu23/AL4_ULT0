#include "CollisionManager.h"
#include "Collision.h"

void CollisionManager::Initialize()
{

	v3Calc = Vector3Calc::GetInstance();
	m4Calc = Matrix4x4Calc::GetInstance();

	ListClear();

}

void CollisionManager::ListClear()
{
	colliders_.clear();
}

void CollisionManager::ListRegister(ColliderShape collider)
{

	colliders_.push_back(collider);

}

void CollisionManager::CheakAllCollision()
{

	// リスト内のペアを総当たり
	std::list<ColliderShape>::iterator itrA = colliders_.begin();
	for (; itrA != colliders_.end(); ++itrA) {
		// イテレータAからコライダーAを取得する
		ColliderShape colliderA = *itrA;
		// イテレータBはイテレータAの次の要素から回す(重複判定を回避)
		std::list<ColliderShape>::iterator itrB = itrA;
		itrB++;

		for (; itrB != colliders_.end(); ++itrB) {
			// イテレータBからコライダーBを取得する
			ColliderShape colliderB = *itrB;

			// ペアの当たり判定
			CheckCollisionPair(colliderA, colliderB);
		}
	}

}

void CollisionManager::CheckCollisionPair(ColliderShape colliderA, ColliderShape colliderB)
{

	// 球と球
	if (std::holds_alternative<Sphere*>(colliderA) && std::holds_alternative<Sphere*>(colliderB)) {
		if (Filtering(std::get<Sphere*>(colliderA), std::get<Sphere*>(colliderB))) {
			return;
		}
		if (Collision::IsCollision(*std::get<Sphere*>(colliderA), *std::get<Sphere*>(colliderB))) {
			// 衝突処理
		}
	}
	// 球と平面
	else if (std::holds_alternative<Sphere*>(colliderA) && std::holds_alternative<Plane*>(colliderB)) {
		if (Filtering(std::get<Sphere*>(colliderA), std::get<Plane*>(colliderB))) {
			return;
		}
		if (Collision::IsCollision(*std::get<Sphere*>(colliderA), *std::get<Plane*>(colliderB))) {
			// 衝突処理
		}
	}
	// 平面と球
	else if (std::holds_alternative<Plane*>(colliderA) && std::holds_alternative<Sphere*>(colliderB)) {
		if (Filtering(std::get<Plane*>(colliderA), std::get<Sphere*>(colliderB))) {
			return;
		}
		if (Collision::IsCollision(*std::get<Sphere*>(colliderB), *std::get<Plane*>(colliderA))) {
			// 衝突処理
		}
	}
	// AABBとAABB
	else if (std::holds_alternative<AABB*>(colliderA) && std::holds_alternative<AABB*>(colliderB)) {
		if (Filtering(std::get<AABB*>(colliderA), std::get<AABB*>(colliderB))) {
			return;
		}
		if (Collision::IsCollision(*std::get<AABB*>(colliderA), *std::get<AABB*>(colliderB))) {
			// 衝突処理
		}
	}
	// AABBと球
	else if (std::holds_alternative<AABB*>(colliderA) && std::holds_alternative<Sphere*>(colliderB)) {
		if (Filtering(std::get<AABB*>(colliderA), std::get<Sphere*>(colliderB))) {
			return;
		}
		if (Collision::IsCollision(*std::get<AABB*>(colliderA), *std::get<Sphere*>(colliderB))) {
			// 衝突処理
		}
	}
	// 球とAABB
	else if (std::holds_alternative<Sphere*>(colliderA) && std::holds_alternative<AABB*>(colliderB)) {
		if (Filtering(std::get<Sphere*>(colliderA), std::get<AABB*>(colliderB))) {
			return;
		}
		if (Collision::IsCollision(*std::get<AABB*>(colliderB), *std::get<Sphere*>(colliderA))) {
			// 衝突処理
		}
	}
	// OBBと球
	else if (std::holds_alternative<OBB*>(colliderA) && std::holds_alternative<Sphere*>(colliderB)) {
		if (Filtering(std::get<OBB*>(colliderA), std::get<Sphere*>(colliderB))) {
			return;
		}
		if (Collision::IsCollision(*std::get<OBB*>(colliderA), *std::get<Sphere*>(colliderB))) {
			// 衝突処理
		}
	}
	// 球とOBB
	else if (std::holds_alternative<Sphere*>(colliderA) && std::holds_alternative<OBB*>(colliderB)) {
		if (Filtering(std::get<Sphere*>(colliderA), std::get<OBB*>(colliderB))) {
			return;
		}
		if (Collision::IsCollision(*std::get<OBB*>(colliderB), *std::get<Sphere*>(colliderA))) {
			// 衝突処理
		}
	}
	// OBBとOBB
	else if (std::holds_alternative<OBB*>(colliderA) && std::holds_alternative<OBB*>(colliderB)) {
		if (Filtering(std::get<OBB*>(colliderA), std::get<OBB*>(colliderB))) {
			return;
		}
		if (Collision::IsCollision(*std::get<OBB*>(colliderA), *std::get<OBB*>(colliderB))) {
			// 衝突処理
		}
	}

}

bool CollisionManager::Filtering(Collider* colliderA, Collider* colliderB)
{

	// 衝突フィルタリング
	if (!(colliderA->GetCollisionAttribute() & colliderB->GetCollisionMask()) ||
		!(colliderB->GetCollisionAttribute() & colliderA->GetCollisionMask())) {
		return true;
	}

	return false;

}
