#pragma once
#include "../../Engine/Animation/IBone.h"
#include "../../Engine/Collider/Collider.h"
#include "PlayerState/IPlayerState.h"
#include "PlayerState/PlayerStateFactory.h"
#include "PlayerCommand/PlayerCommand.h"

/// <summary>
/// プレイヤーの部位一覧
/// </summary>
enum PlayerPartIndex {
	kPlayerPartTorso, // 胴
	kPlayerPartLowerBack, // 腰
	kPlayerPartHead, // 頭

	kPlayerPartLeftUpperArm, // 左上腕
	kPlayerPartLeftForearm, // 左前腕 
	kPlayerPartLeftHand, // 左手
	kPlayerPartRightUpperArm, // 右上腕
	kPlayerPartRightForearm, // 右前腕 
	kPlayerPartRightHand, // 右手

	kPlayerPartLeftThigh, // 左太もも
	kPlayerPartLeftShin, // 左すね
	kPlayerPartLeftAnkle, // 左足首
	kPlayerPartRightThigh, // 右太もも
	kPlayerPartRightShin, // 右すね
	kPlayerPartRightAnkle, // 右足首

	kPlayerPartIndexOfCount // 数
};

/// <summary>
/// プレイヤーのコライダー一覧
/// </summary>
enum PlayerColliderIndex {

	kPlayerColliderHead, // 頭
	kPlayerColliderTorso, // 胴

	kPlayerColliderLeftUpperArm, // 左上腕
	kPlayerColliderLeftForearm, // 左前腕 
	kPlayerColliderRightUpperArm, // 右上腕
	kPlayerColliderRightForearm, // 右前腕 

	kPlayerColliderLeftThigh, // 左太もも
	kPlayerColliderLeftShin, // 左すね
	kPlayerColliderRightThigh, // 右太もも
	kPlayerColliderRightShin, // 右すね

	kPlayerColliderIndexOfCount // 数
};

/// <summary>
/// プレイヤーのモーション一覧
/// </summary>
enum PlayerMotionIndex {
	kPlayerMotionStand, // 通常時
	kPlayerMotionRun, // 走行時
	kPlayerMotionIndexOfCount // 数
};

/// <summary>
/// プレイヤー
/// </summary>
class Player
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount>& models);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection">ビュープロジェクション(参照渡し)</param>
	void Draw(BaseCamera& camera);

	/// <summary>
	/// ImGui描画
	/// </summary>
	void ImGuiDraw();

private: // ベースのメンバ変数

	// マテリアル
	std::unique_ptr<Material> material_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// コマンド
	PlayerCommand* playerCommand_;

	// コマンドを受け取るか
	bool receiveCommand_;

private: // ステート関数

	/// <summary>
	/// ステート初期化
	/// </summary>
	void StateInitialize();
	
	/// <summary>
	/// ステート更新
	/// </summary>
	void StateUpdate();

private: // ステート変数

	// ステート
	std::unique_ptr<IPlayerState> playerState_;

	// 現在のステート番号
	uint32_t currentStateNo_;

	// 前のステート番号
	uint32_t prevStateNo_;

	// 次のステート番号
	uint32_t nextStateNo_;
	
	// ステートファクトリー
	PlayerStateFactory* playerStateFactory_;

private: // パーツ構成関数

	/// <summary>
	/// パーツ初期化
	/// </summary>
	void PartInitialize();

	/// <summary>
	/// パーツ更新
	/// </summary>
	void PartUpdate();

	/// <summary>
	/// パーツの親子付け
	/// </summary>
	void PartParent();

	/// <summary>
	/// コライダー初期化
	/// </summary>
	void ColliderInitialize();

	/// <summary>
	/// コライダー更新
	/// </summary>
	void ColliderUpdate();

private: // パーツ,アニメーション変数

	// 骨
	std::array<std::unique_ptr<IBone>, PlayerPartIndex::kPlayerPartIndexOfCount> parts_;

	// モデル
	std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount> models_;

	// コライダー (ダメージを受ける側、位置が被らない用)
	std::array<std::unique_ptr<Collider>, PlayerColliderIndex::kPlayerColliderIndexOfCount> colliders_;

	// コライダー用半径
	std::array<float, PlayerColliderIndex::kPlayerColliderIndexOfCount> colliderRadiuses_;

	// 現在のモーション番号
	uint32_t currentMotionNo_;

	// 前のモーション番号
	uint32_t prevMotionNo_;

	// アニメーションカウント
	uint32_t animationCount_;

	// アニメーションカウント上限
	uint32_t animationCountLimit_;

private:  // パーツ,アニメーション定数

	// オブジェクト名
	const std::string objectName_ = "Player";

	// パーツ名
	const std::array<const std::string, PlayerPartIndex::kPlayerPartIndexOfCount> partNames_ = {
		"Torso",
		"LowerBack",
		"Head",
		"LeftUpperArm",
		"LeftForearm",
		"LeftHand",
		"RightUpperArm",
		"RightForearm",
		"RightHand",
		"LeftThigh",
		"LeftShin",
		"LeftAnkle",
		"RightThigh",
		"RightShin",
		"RightAnkle"
	};

	// モーション名
	const std::array<const std::string, PlayerMotionIndex::kPlayerMotionIndexOfCount> motionNames_ = {
		"Stand",
		"Run",
	};

private: // プレイヤーデータ

	// 高さ
	float height_ = 20.0f;

	// カメラ
	BaseCamera* camera_ = nullptr;

public: // アクセッサ

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	void SetCamera(BaseCamera* camera) { camera_ = camera; }

	BaseCamera* GetCamera() { return camera_; }

	void SetReceiveCommand(bool receiveCommand) { receiveCommand_ = receiveCommand; }

};
