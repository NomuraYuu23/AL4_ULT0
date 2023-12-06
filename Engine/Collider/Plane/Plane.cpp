#include "Plane.h"

void Plane::Initialize(const Vector3& normal, float distance, void* parentObject)
{

	Collider::Initialize(parentObject);

	normal_ = normal;//法線
	distance_ = distance;//距離

}

void Plane::worldTransformUpdate()
{
}
