#include "BloadParticle.h"
#include <random>
#include "../../../Engine/Math/Ease.h"

void BloadParticle::Initialize(const Vector3& position, const Vector3& size)
{

	Vector3Calc* vector3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	transform_.scale = size;
	transform_.rotate = { 0.0f,0.0f,0.0f };
	transform_.translate = position;

	worldMatrix_ = matrix4x4Calc->MakeIdentity4x4();

	color_ = redColor_;

	lifeTime_ = 0.2f;

	currentTime_ = 0.0f;

	useBillBoard_ = true;

	billBoardName_ = kBillBoardNameIndexAllAxis;

	UpdateMatrix(matrix4x4Calc->MakeIdentity4x4());

	isDead_ = false;

	Vector3 velocityMax = { 3.0f, 3.0f, 3.0f };
	Vector3 velocityMin = { -3.0f, -3.0f, -3.0f };
	std::uniform_real_distribution<float> velocityX(velocityMin.x, velocityMax.x);
	std::uniform_real_distribution<float> velocityY(velocityMin.y, velocityMax.y);
	std::uniform_real_distribution<float> velocityZ(velocityMin.z, velocityMax.z);

	velocity_ = { velocityX(randomEngine), velocityY(randomEngine), velocityZ(randomEngine) };

	float accelerationValue = 0.01f;

	if (velocity_.x > 0.0f) {
		acceleration_.x *= -1.0f;
	}
	if (velocity_.y > 0.0f) {
		acceleration_.y *= -1.0f;
	}
	if (velocity_.z > 0.0f) {
		acceleration_.z *= -1.0f;
	}


}

void BloadParticle::Update(const Matrix4x4& billBoardMatrix)
{

	Vector3Calc* vector3Calc = Vector3Calc::GetInstance();
	Matrix4x4Calc* matrix4x4Calc = Matrix4x4Calc::GetInstance();

	velocity_ = vector3Calc->Subtract(velocity_, acceleration_);
	if (std::fabsf(velocity_.x) <= 0.1f) {
		velocity_.x = 0.0f;
	}
	if (std::fabsf(velocity_.y) <= 0.1f) {
		velocity_.y = 0.0f;
	}
	if (std::fabsf(velocity_.z) <= 0.1f) {
		velocity_.z = 0.0f;
	}

	transform_.translate = vector3Calc->Add(transform_.translate, velocity_);

	IParticle::Update(billBoardMatrix);

}
