#include "Skydome.h"
#include <cassert>

/// <summary>
/// 初期化
/// </summary>
/// <param name="model">モデル</param>
void Skydome::Initialize(Model* model, Material* material) {

	// nullポインタチェック
	assert(model);

	model_ = model;

	material_ = material;

	// ワールド変換データの初期化
	worldTransform_.Initialize();
}

/// <summary>
/// 更新
/// </summary>
void Skydome::Update() {

	worldTransform_.UpdateMatrix();

}

/// <summary>
/// 描画
/// </summary>
/// <param name="viewProjection">ビュープロジェクション</param>
void Skydome::Draw(BaseCamera& camera) {

	model_->Draw(worldTransform_, camera);

}
