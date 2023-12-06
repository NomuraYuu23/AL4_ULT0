#include "Collision.h"
#include <algorithm>

Vector3Calc* Collision::vector3Calc_ = Vector3Calc::GetInstance();

Matrix4x4Calc* Collision::matrix4x4Calc_ = Matrix4x4Calc::GetInstance();

bool Collision::IsCollision(const Sphere& sphere1, const Sphere& sphere2)
{

	//2つの球の中心点間の距離を求める
	float distance = vector3Calc_->Length(vector3Calc_->Subtract(sphere1.center_, sphere2.center_));
	//半径の合計よりも短ければ衝突
	if (distance <= sphere1.radius_ + sphere2.radius_) {
		return true;
	}

	return false;

}

bool Collision::IsCollision(const Sphere& sphere, const Plane& plane)
{

	//1.平面と球の中心点との距離を求める
	float distance = std::fabsf(vector3Calc_->Dot(plane.normal_, sphere.center_) - plane.distance_);
	//2.1の距離 <= 球の半径なら衝突
	if (distance <= sphere.radius_) {
		return true;
	}

	return false;

}

bool Collision::IsCollision(const AABB& aabb1, const AABB& aabb2)
{

	if ((aabb1.min_.x <= aabb2.max_.x && aabb1.max_.x >= aabb2.min_.x) &&
		(aabb1.min_.y <= aabb2.max_.y && aabb1.max_.y >= aabb2.min_.y) &&
		(aabb1.min_.z <= aabb2.max_.z && aabb1.max_.z >= aabb2.min_.z)) {
		return true;
	}

	return false;

}

bool Collision::IsCollision(const AABB& aabb, const Sphere& sphere)
{

	//最近接点を求める
	Vector3 closestPoint{ std::clamp(sphere.center_.x, aabb.min_.x, aabb.max_.x),
		std::clamp(sphere.center_.y, aabb.min_.y, aabb.max_.y) ,
		std::clamp(sphere.center_.z, aabb.min_.z, aabb.max_.z) };

	//最近接点と球の中心との距離を求める
	float distance = vector3Calc_->Length(vector3Calc_->Subtract(closestPoint, sphere.center_));
	//距離が半径よりも小さければ衝突
	if (distance <= sphere.radius_) {
		return true;
	}

	return false;

}

bool Collision::IsCollision(const OBB& obb, const Sphere& sphere)
{

	Matrix4x4 obbWorldMatrix = {
	obb.otientatuons_[0].x,obb.otientatuons_[1].x, obb.otientatuons_[2].x, 0,
	obb.otientatuons_[0].y,obb.otientatuons_[1].y, obb.otientatuons_[2].y, 0,
	obb.otientatuons_[0].z,obb.otientatuons_[1].z, obb.otientatuons_[2].z, 0,
	obb.center_.x, obb.center_.y, obb.center_.y, 1 };

	Matrix4x4 obbWorldMatrixInverse = matrix4x4Calc_->Inverse(obbWorldMatrix);

	Vector3 centerInOBBLocalSpace = matrix4x4Calc_->Transform(sphere.center_, obbWorldMatrixInverse);

	AABB aabbOBBLocal;
	aabbOBBLocal.Initialize(Vector3{ -obb.size_.x,-obb.size_.y, -obb.size_.z }, obb.size_, nullptr);

	Sphere sphereOBBLocal;
	sphereOBBLocal.Initialize(centerInOBBLocalSpace, sphere.radius_, nullptr);

	//ローカル空間で衝突
	return IsCollision(aabbOBBLocal, sphereOBBLocal);

}

bool Collision::IsCollision(const OBB& obb, const OBB& obb2)
{

	//軸
	Vector3 axis[15];

	//面法線
	axis[0] = obb.otientatuons_[0];
	axis[1] = obb.otientatuons_[1];
	axis[2] = obb.otientatuons_[2];
	axis[3] = obb2.otientatuons_[0];
	axis[4] = obb2.otientatuons_[1];
	axis[5] = obb2.otientatuons_[2];

	//クロス積
	axis[6] = vector3Calc_->Cross(obb.otientatuons_[0], obb2.otientatuons_[0]);
	axis[7] = vector3Calc_->Cross(obb.otientatuons_[0], obb2.otientatuons_[1]);
	axis[8] = vector3Calc_->Cross(obb.otientatuons_[0], obb2.otientatuons_[2]);
	axis[9] = vector3Calc_->Cross(obb.otientatuons_[1], obb2.otientatuons_[0]);
	axis[10] = vector3Calc_->Cross(obb.otientatuons_[1], obb2.otientatuons_[1]);
	axis[11] = vector3Calc_->Cross(obb.otientatuons_[1], obb2.otientatuons_[2]);
	axis[12] = vector3Calc_->Cross(obb.otientatuons_[2], obb2.otientatuons_[0]);
	axis[13] = vector3Calc_->Cross(obb.otientatuons_[2], obb2.otientatuons_[1]);
	axis[14] = vector3Calc_->Cross(obb.otientatuons_[2], obb2.otientatuons_[2]);

	//頂点

	//回転させる
	Vector3 obbVertex[8];

	obbVertex[0] = {
		+obb.size_.x,
		+obb.size_.y,
		+obb.size_.z };

	obbVertex[1] = {
		+obb.size_.x,
		+obb.size_.y,
		-obb.size_.z };

	obbVertex[2] = {
		+obb.size_.x,
		-obb.size_.y,
		+obb.size_.z };

	obbVertex[3] = {
		+obb.size_.x,
		-obb.size_.y,
		-obb.size_.z };

	obbVertex[4] = {
		-obb.size_.x,
		+obb.size_.y,
		+obb.size_.z };

	obbVertex[5] = {
		-obb.size_.x,
		+obb.size_.y,
		-obb.size_.z };

	obbVertex[6] = {
		-obb.size_.x,
		-obb.size_.y,
		+obb.size_.z };

	obbVertex[7] = {
		-obb.size_.x,
		-obb.size_.y,
		-obb.size_.z };

	Matrix4x4 obbRotateMatrix = {
		obb.otientatuons_[0].x,obb.otientatuons_[0].y,obb.otientatuons_[0].z,0.0f,
		obb.otientatuons_[1].x,obb.otientatuons_[1].y,obb.otientatuons_[1].z,0.0f,
		obb.otientatuons_[2].x,obb.otientatuons_[2].y,obb.otientatuons_[2].z,0.0f,
		0.0f,0.0f,0.0f,1.0f };

	for (int i = 0; i < 8; i++) {

		obbVertex[i] = matrix4x4Calc_->Transform(obbVertex[i], obbRotateMatrix);
		obbVertex[i] = vector3Calc_->Add(obbVertex[i], obb.center_);

	}

	Vector3 obb2Vertex[8];

	obb2Vertex[0] = {
		+obb2.size_.x,
		+obb2.size_.y,
		+obb2.size_.z };

	obb2Vertex[1] = {
		+obb2.size_.x,
		+obb2.size_.y,
		-obb2.size_.z };

	obb2Vertex[2] = {
		+obb2.size_.x,
		-obb2.size_.y,
		+obb2.size_.z };

	obb2Vertex[3] = {
		+obb2.size_.x,
		-obb2.size_.y,
		-obb2.size_.z };

	obb2Vertex[4] = {
		 -obb2.size_.x,
		+obb2.size_.y,
		+obb2.size_.z };

	obb2Vertex[5] = {
		-obb2.size_.x,
		+obb2.size_.y,
		-obb2.size_.z };

	obb2Vertex[6] = {
		-obb2.size_.x,
		-obb2.size_.y,
		+obb2.size_.z };

	obb2Vertex[7] = {
		-obb2.size_.x,
		-obb2.size_.y,
		-obb2.size_.z };

	Matrix4x4 obb2RotateMatrix = {
	obb2.otientatuons_[0].x,obb2.otientatuons_[0].y,obb2.otientatuons_[0].z,0.0f,
	obb2.otientatuons_[1].x,obb2.otientatuons_[1].y,obb2.otientatuons_[1].z,0.0f,
	obb2.otientatuons_[2].x,obb2.otientatuons_[2].y,obb2.otientatuons_[2].z,0.0f,
	0.0f,0.0f,0.0f,1.0f };

	for (int i = 0; i < 8; i++) {

		obb2Vertex[i] = matrix4x4Calc_->Transform(obb2Vertex[i], obb2RotateMatrix);
		obb2Vertex[i] = vector3Calc_->Add(obb2Vertex[i], obb2.center_);

	}

	//1.頂点を軸に対して射影
	for (int a = 0; a < 15; a++) {
		float min1 = 0.0f;
		float max1 = 0.0f;
		float min2 = 0.0f;
		float max2 = 0.0f;

		for (int v = 0; v < 8; v++) {
			//一時保存
			float tmp = 0.0f;
			//obb
			tmp = vector3Calc_->Dot(vector3Calc_->Normalize(axis[a]), obbVertex[v]);
			//2.射影した点の最大値と最小値を求める
			if (v == 0 || min1 > tmp) {
				min1 = tmp;
			}
			if (v == 0 || max1 < tmp) {
				max1 = tmp;
			}

			//obb2
			tmp = vector3Calc_->Dot(vector3Calc_->Normalize(axis[a]), obb2Vertex[v]);
			//2.射影した点の最大値と最小値を求める
			if (v == 0 || min2 > tmp) {
				min2 = tmp;
			}
			if (v == 0 || max2 < tmp) {
				max2 = tmp;
			}
		}

		//3.差分の形状を分離軸に射影した長さ
		float L1 = max1 - min1;
		float L2 = max2 - min2;

		float sumSpan = L1 + L2;
		float LonSpan = (std::max)(max1, max2) - (std::min)(min1, min2);
		if (sumSpan < LonSpan) {
			//分離しているので分離軸
			return false;
		}

	}

	return true;

}
