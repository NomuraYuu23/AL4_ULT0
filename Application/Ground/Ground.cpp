#include "Ground.h"

void Ground::Initialize(Model* model)
{

	// nullポインタチェック
	assert(model);

	model_ = model;

	material_.reset(Material::Create());

	// ワールド変換データの初期化
	worldTransform_.Initialize();
	worldTransform_.transform_.scale.x = 1000.0f;
	worldTransform_.transform_.scale.z = 1000.0f;
	worldTransform_.UpdateMatrix();

}

void Ground::Update()
{

	worldTransform_.UpdateMatrix();

}

void Ground::Draw(BaseCamera& camera)
{

	model_->Draw(worldTransform_, camera);

}
