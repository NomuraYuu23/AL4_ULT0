#pragma once
#include "../../Engine/Animation/IBone.h"
#include "../../Engine/Collider/Collider.h"
#include "EnemyState/IEnemyState.h"
#include "EnemyState/EnemyStateFactory.h"
#include "EnemyCommand/EnemyCommand.h"
#include "../Collider/ColliderParentObject.h"
#include "../../Engine/Collision/CollisionData.h"
#include "../../Engine/Collider/ColliderShape.h"

/// <summary>
/// エネミーの部位一覧
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
/// エネミーのコライダー一覧
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
/// エネミーのモーション一覧
/// </summary>
enum EnemyMotionIndex {
	kEnemyMotionStand, // 通常時
	kEnemyMotionDashSwingDown, // ダッシュ振り下ろし
	kEnemyMotionMowingDown, // 薙ぎ払い
	kEnemyMotionIndexOfCount // 数
};

class Enemy
{

public: // ベースのメンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::array<Model*, EnemyPartIndex::kEnemyPartIndexOfCount>& models, Model* weaponModel);

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

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="colliderPartner"></param>
	/// <param name="collisionData"></param>
	void OnCollision(ColliderParentObject colliderPartner, const CollisionData& collisionData);

private: // ベースのメンバ変数

	// マテリアル
	std::unique_ptr<Material> material_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// コマンド
	EnemyCommand* enemyCommand_;

	// コマンドを受け取るか
	bool receiveCommand_;

	//衝突属性(自分)
	uint32_t collisionAttribute_ = 0x00000002;

	// 衝突マスク(相手)
	uint32_t collisionMask_ = 0xfffffffd;

	// シリアルナンバー
	uint32_t serialNumber_ = 0;

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
	std::array<std::unique_ptr<Capsule>, EnemyColliderIndex::kEnemyColliderIndexOfCount> colliders_;

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

	// 武器モデル
	Model* weaponModel_ = nullptr;

	// 武器ワールドトランスフォーム
	WorldTransform weaponWorldTransfrom_;

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
		"Stand",
		"DashSwingDown",
		"MowingDown"
	};

private: // エネミーデータ

	// 高さ
	float height_ = 30.0f;

	// 幅
	float width_ = 20.0f;

	// プレイヤー
	Player* player_ = nullptr;

	//hp
	int32_t hp_;
	uint32_t initHp_;

	// 死んだか
	bool isDead_;

public:

	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="damage"></param>
	void Damage(uint32_t damage);

	/// <summary>
	/// HPの割合
	/// </summary>
	/// <returns></returns>
	float RatioHP();

private: // 衝突処理

	void OnCollisionPlayerAttack(ColliderParentObject colliderPartner, const CollisionData& collisionData);

public: // アクセッサ

	WorldTransform* GetWorldTransformAdress() { return &worldTransform_; }

	void SetReceiveCommand(bool receiveCommand) { receiveCommand_ = receiveCommand; }

	void SetHeight(float height) { height_ = height; }

	float GetWidth() { return width_; }

	std::array<ColliderShape, EnemyColliderIndex::kEnemyColliderIndexOfCount> GetCollider();

	void SetPlayer(Player* player) { player_ = player; }

	Player* GetPlayer() { return player_; }

	IEnemyState* GetEnemyState() { return enemyState_.get(); }

	uint32_t GetCurrentStateNo() { return currentStateNo_; }

	uint32_t GetSerialNumber() { return serialNumber_; }

	IBone* GetPart(uint32_t num) { return parts_[num].get(); }

	bool GetIsDead() { return isDead_; }

	int32_t GetHp() { return hp_; }

	uint32_t GetInitHp() { return initHp_; }

};

