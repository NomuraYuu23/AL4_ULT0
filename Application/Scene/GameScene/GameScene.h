#pragma once

#include "../../../Engine/Scene/IScene/IScene.h"
#include "../../../Engine/Collider/ColliderDebugDraw/ColliderDebugDraw.h"// コライダーデバッグ描画
#include"../../../Engine/Collision/CollisionManager.h"
#include "../../Pause/Pause.h"
#include "../../../Engine/base/ITextureHandleManager.h"
#include "../../SampleBone/SampleBone.h"

class GameScene : public IScene
{

public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// imgui描画処理
	/// </summary>
	void ImguiDraw();

public: //メンバ関数

	/// <summary>
	/// デバッグカメラ更新
	/// </summary>
	void DebugCameraUpdate();

	/// <summary>
	/// タイトルへ行く
	/// </summary>
	void GoToTheTitle();

private: // メンバ関数

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	void ModelCreate() override;

	/// <summary>
	/// マテリアルクリエイト
	/// </summary>
	void MaterialCreate() override;

	/// <summary>
	/// テクスチャロード
	/// </summary>
	void TextureLoad() override;

private:

	// デバッグ描画
	std::unique_ptr<ColliderDebugDraw> colliderDebugDraw_ = nullptr;
	std::unique_ptr<Model> colliderSphereModel_ = nullptr;
	std::unique_ptr<Model> colliderBoxModel_ = nullptr;
	std::unique_ptr<Material> colliderMaterial_ = nullptr;

	// ポーズ
	// テクスチャハンドル
	std::array<uint32_t, Pause::PauseTextureNo::kCountOfPauseTextureNo> pauseTextureHandles_;
	std::unique_ptr<Pause> pause_ = nullptr;

	// パーティクルマネージャー
	ParticleManager* particleManager_ = nullptr;
	std::unique_ptr<Model> particleUvcheckerModel_ = nullptr;
	std::unique_ptr<Model> particleCircleModel_ = nullptr;

	// モデル
	std::unique_ptr<Model> model_ = nullptr;
	std::unique_ptr<Material> material_ = nullptr;
	WorldTransform worldTransform_;
	Vector3 direction = { 1.0f, -1.0f, 0.0f};
	float intencity = 1.0f;

	// サンプルボーン
	std::unique_ptr<SampleBone> sampleBone_;

};
