#include "Collider.h"

void Collider::Initialize(void* parentObject)
{

	worldTransform_.Initialize();

	parentObject_ = parentObject;

}
