#include "ViewProjection.h"
#include "../base/WinApp.h"

void ViewProjection::Initialize() {

	transform_ = { { 1.0f, 1.0f, 1.0f},{ 0.0f, 0.0f, 0.0f},{ 0.0f, 0.0f, -10.0f} };

	fovY_ = 0.45f;

	aspectRatio_ = float(WinApp::kWindowWidth) / float(WinApp::kWindowHeight);

	nearClip_ = 0.1f;

	farClip_ = 1000.0f;

	UpdateMatrix();

}

void ViewProjection::UpdateMatrix() {

	Matrix4x4Calc* calc = Matrix4x4Calc::GetInstance();

	transformMatrix_ = calc->MakeAffineMatrix(transform_.scale, transform_.rotate, transform_.translate);
	viewMatrix_ = calc->Inverse(transformMatrix_);
	projectionMatrix_ = calc->MakePerspectiveFovMatrix(fovY_, aspectRatio_, nearClip_, farClip_);

	viewProjectionMatrix_ = calc->Multiply(viewMatrix_, projectionMatrix_);

}