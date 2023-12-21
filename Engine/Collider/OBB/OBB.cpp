#include "OBB.h"

void OBB::Initialize(const Vector3& center, const Vector3& otientatuons1, const Vector3& otientatuons2, const Vector3& otientatuons3, const Vector3& size, ColliderParentObject parentObject)
{

	Collider::Initialize(parentObject);

	center_ = center;//中心
	otientatuons_[0] = otientatuons1;//座標軸
	otientatuons_[1] = otientatuons2;//座標軸
	otientatuons_[2] = otientatuons3;//座標軸
	size_ = size;//座標軸方向の長さの半分

}

void OBB::SetOtientatuons(const Vector3& otientatuons1, const Vector3& otientatuons2, const Vector3& otientatuons3)
{

	otientatuons_[0] = otientatuons1;//座標軸
	otientatuons_[1] = otientatuons2;//座標軸
	otientatuons_[2] = otientatuons3;//座標軸

}

void OBB::worldTransformUpdate()
{

	Vector3Calc* v3Calc = Vector3Calc::GetInstance();

	worldTransform_.transform_.translate = center_;
	worldTransform_.transform_.scale = size_;
	worldTransform_.UpdateMatrix();

}
