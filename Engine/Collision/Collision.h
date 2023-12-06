#pragma once

#include "../../Engine/Collider/Sphere/Sphere.h"
#include "../../Engine/Collider/Plane/Plane.h"
#include "../../Engine/Collider/Triangle/Triangle.h"
#include "../../Engine/Collider/AABB/AABB.h"
#include "../../Engine/Collider/OBB/OBB.h"
#include "../../Engine/Math/Matrix4x4.h"


class Collision
{

public: // メンバ関数

	/// <summary>
	/// 球と球
	/// </summary>
	/// <param name="s1"></param>
	/// <param name="s2"></param>
	/// <returns></returns>
	static bool IsCollision(const Sphere& sphere1, const Sphere& sphere2);

	/// <summary>
	/// 球と平面
	/// </summary>
	/// <param name="sphere"></param>
	/// <param name="plane"></param>
	/// <returns></returns>
	static bool IsCollision(const Sphere& sphere, const Plane& plane);

	/// <summary>
	/// AABBとAABB
	/// </summary>
	/// <param name="aabb1"></param>
	/// <param name="aabb2"></param>
	/// <returns></returns>
	static bool IsCollision(const AABB& aabb1, const AABB& aabb2);

	/// <summary>
	/// AABBと球
	/// </summary>
	/// <param name="aabb"></param>
	/// <param name="sphere"></param>
	/// <returns></returns>
	static bool IsCollision(const AABB& aabb, const Sphere& sphere);

	/// <summary>
	/// OBBと球
	/// </summary>
	/// <param name="obb"></param>
	/// <param name="sphere"></param>
	/// <returns></returns>
	static bool IsCollision(const OBB& obb, const Sphere& sphere);

	/// <summary>
	/// OBBとOBB
	/// </summary>
	/// <param name="obb"></param>
	/// <param name="obb2"></param>
	/// <returns></returns>
	static bool IsCollision(const OBB& obb, const OBB& obb2);

private: // メンバ変数

	static Vector3Calc* vector3Calc_;

	static Matrix4x4Calc* matrix4x4Calc_;

};

