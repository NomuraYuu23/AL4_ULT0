#pragma once
#include "../../Engine/Animation/IBone.h"
#include "../../Engine/Collider/Collider.h"
#include "EnemyState/IEnemyState.h"
#include "EnemyState/EnemyStateFactory.h"
#include "EnemyCommand/EnemyCommand.h"

/// <summary>
/// プレイヤーの部位一覧
/// </summary>
enum EnemyPartIndex {
	kEnemyPartTorso, // 胴
	kEnemyPartLowerBack, // 腰
	kEnemyPartHead, // 頭

	kEnemyPartLeftUpperArm, // 左上腕
	kEnemyPartLeftForearm, // 左前腕 
	kEnemyPartLeftHand, // 左手
	kEnemyPartRightUpperArm, // 右上腕
	kEnemyPartRightForearm, // 右前腕 
	kEnemyPartRightHand, // 右手

	kEnemyPartLeftThigh, // 左太もも
	kEnemyPartLeftShin, // 左すね
	kEnemyPartLeftAnkle, // 左足首
	kEnemyPartRightThigh, // 右太もも
	kEnemyPartRightShin, // 右すね
	kEnemyPartRightAnkle, // 右足首

	kEnemyPartIndexOfCount // 数
};

/// <summary>
/// プレイヤーのコライダー一覧
/// </summary>
enum EnemyColliderIndex {

	kEnemyColliderHead, // 頭
	kEnemyColliderTorso, // 胴

	kEnemyColliderLeftUpperArm, // 左上腕
	kEnemyColliderLeftForearm, // 左前腕 
	kEnemyColliderRightUpperArm, // 右上腕
	kEnemyColliderRightForearm, // 右前腕 

	kEnemyColliderLeftThigh, // 左太もも
	kEnemyColliderLeftShin, // 左すね
	kEnemyColliderRightThigh, // 右太もも
	kEnemyColliderRightShin, // 右すね

	kEnemyColliderIndexOfCount // 数
};

/// <summary>
/// プレイヤーのモーション一覧
/// </summary>
enum EnemyMotionIndex {
	kEnemyMotionStand, // 通常時
	kEnemyMotionIndexOfCount // 数
};

class Enemy
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::array<Model*, EnemyPartIndex::kEnemyPartIndexOfCount>& models);

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
	EnemyCommand* enemyCommand_;

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
	std::unique_ptr<IEnemyState> enemyState_;

	// 現在のステート番号
	uint32_t currentStateNo_;

	// 前のステート番号
	uint32_t prevStateNo_;

	// 次のステート番号
	uint32_t nextStateNo_;

	// ステートファクトリー
	EnemyStateFactory* enemyStateFactory_;

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
	std::array<std::unique_ptr<IBone>, EnemyPartIndex::kEnemyPartIndexOfCount> parts_;

	// モデル
	std::array<Model*, EnemyPartIndex::kEnemyPartIndexOfCount> models_;

	// コライダー (ダメージを受ける側、位置が被らない用)
	std::array<std::unique_ptr<Collider>, EnemyColliderIndex::kEnemyColliderIndexOfCount> colliders_;

	// コライダー用半径
	std::array<float, EnemyColliderIndex::kEnemyColliderIndexOfCount> colliderRadiuses_;

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
	const std::string objectName_ = "Enemy";

	// パーツ名
	const std::array<const std::string, EnemyPartIndex::kEnemyPartIndexOfCount> partNames_ = {
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
	const std::array<const std::string, EnemyMotionIndex::kEnemyMotionIndexOfCount> motionNames_ = {
		"Stand"
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

	void SetHeight(float height) { height_ = height; }

};

