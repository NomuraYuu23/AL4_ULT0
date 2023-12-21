#pragma once
#include "../../Engine/Animation/IBone.h"
#include "../../Engine/Collider/Collider.h"
#include "PlayerState/IPlayerState.h"

/// <summary>
/// プレイヤーの部位一覧
/// </summary>
enum PlayerPartIndex {
	kPlayerPartHead, // 頭
	kPlayerPartTorso, // 胴
	kPlayerPartLowerBack, // 腰

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
	kPlayerMotionNormal, // 通常時
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

private: // ベースのメンバ変数

	// マテリアル
	std::unique_ptr<Material> material_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// ステート
	std::unique_ptr<IPlayerState> playerState_;

	// ステート番号
	uint32_t stateNo_;
	uint32_t requestStateNo_;

private: // パーツ構成関数

	/// <summary>
	/// コライダー初期化
	/// </summary>
	void ColliderInitialize();

private: // パーツ構成変数

	// 骨
	std::array<std::unique_ptr<IBone>, PlayerPartIndex::kPlayerPartIndexOfCount> parts_;

	// モデル
	std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount> models_;

	// コライダー (ダメージを受ける側、位置が被らない用)
	std::array<std::unique_ptr<Collider>, PlayerColliderIndex::kPlayerColliderIndexOfCount> colliders_;

	// コライダー用
	std::array<float, PlayerColliderIndex::kPlayerColliderIndexOfCount> colliderRadiuses_;


private: // アニメーション

	// オブジェクト名
	const std::string objectName_ = "Player";

	// パーツ名
	const std::array<const std::string, PlayerPartIndex::kPlayerPartIndexOfCount> partNames_ = {
		"Head",
		"Torso",
		"LowerBack",
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
		"Normal",
	};

	// アニメーションカウント
	uint32_t animationCount_;


};
