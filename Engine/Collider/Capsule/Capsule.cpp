#include "Capsule.h"

void Capsule::Initialize(const Segment& segment, float radius, void* parentObject)
{

	Collider::Initialize(parentObject);

	segment_ = segment;
	radius_ = radius;

}

void Capsule::worldTransformUpdate()
{

	// とりあえず放置

}
