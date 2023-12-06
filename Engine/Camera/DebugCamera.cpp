#include "DebugCamera.h"
#include "../Input/Input.h"

/// <summary>
/// 初期化
/// </summary>
void DebugCamera::Initialize() {

	BaseCamera::Initialize();

	moveSpeed_ = 0.1f;
	rotateSpeed_ = 0.01f;

}

/// <summary>
/// 更新
/// </summary>
void DebugCamera::Update() {

	Input* input = Input::GetInstance();
	
	if (input->PushKey(DIK_D)) {
		viewProjection_.transform_.translate.x += moveSpeed_;
	}
	if (input->PushKey(DIK_A)) {
		viewProjection_.transform_.translate.x -= moveSpeed_;
	}
	if (input->PushKey(DIK_W)) {
		viewProjection_.transform_.translate.y += moveSpeed_;
	}
	if (input->PushKey(DIK_S)) {
		viewProjection_.transform_.translate.y -= moveSpeed_;
	}
	if (input->PushKey(DIK_E)) {
		viewProjection_.transform_.translate.z += moveSpeed_;
	}
	if (input->PushKey(DIK_Q)) {
		viewProjection_.transform_.translate.z -= moveSpeed_;
	}

	if (input->PushKey(DIK_RIGHT)) {
		viewProjection_.transform_.rotate.y += rotateSpeed_;
	}
	if (input->PushKey(DIK_LEFT)) {
		viewProjection_.transform_.rotate.y -= rotateSpeed_;
	}
	if (input->PushKey(DIK_DOWN)) {
		viewProjection_.transform_.rotate.x += rotateSpeed_;
	}
	if (input->PushKey(DIK_UP)) {
		viewProjection_.transform_.rotate.x -= rotateSpeed_;
	}

	BaseCamera::Update();

}
