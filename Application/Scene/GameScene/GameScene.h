#pragma once

#include "../../../Engine/Scene/IScene/IScene.h"
#include "../../../Engine/Collider/ColliderDebugDraw/ColliderDebugDraw.h"// コライダーデバッグ描画
#include"../../../Engine/Collision/CollisionManager.h"
#include "../../Pause/Pause.h"
#include "../../../Engine/base/ITextureHandleManager.h"
#include "../../AudioManager/GameAudioManager.h"

// プレイヤー
#include "../../Player/Player.h"
// 追従カメラ
#include "../../Camera/FollowCamera/FollowCamera.h"
// スカイドーム
#include "../../Skydome/Skydome.h"
// 地面
#include "../../Ground/Ground.h"
// エネミー
#include "../../Enemy/Enemy.h"

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

	/// <summary>
	/// コリジョンマネージャー更新
	/// </summary>
	void CollisonUpdate();

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

	//ライトデータ
	DirectionalLightData directionalLightData_;

	// オーディオマネージャー
	std::unique_ptr<GameAudioManager> audioManager_;

	// コリジョンマネージャー
	std::unique_ptr<CollisionManager> collisionManager_;

	// プレイヤー
	std::unique_ptr<Player> player_;
	// プレイヤーモデル
	std::array<std::unique_ptr<Model>, PlayerPartIndex::kPlayerPartIndexOfCount> playerModels_;

	// 追加カメラ
	std::unique_ptr<FollowCamera> followCamera_;

	// スカイドーム
	std::unique_ptr<Skydome> skyDome_;
	// スカイドームモデル
	std::unique_ptr<Model> skyDomeModel_;

	// 地面
	std::unique_ptr<Ground> ground_;
	// 地面モデル
	std::unique_ptr<Model> groundModel_;

	// エネミー
	std::unique_ptr<Enemy> enemy_;
	// エネミーモデル
	std::array<std::unique_ptr<Model>, EnemyPartIndex::kEnemyPartIndexOfCount> enemyModels_;
	std::unique_ptr<Model> enemyWeaponModel_;
};
