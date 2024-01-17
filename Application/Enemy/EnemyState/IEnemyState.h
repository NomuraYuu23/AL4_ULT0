#pragma once
#include <cstdint>
#include "../../../Engine/Input/Input.h"
#include "../../../Engine/Math/Vector3.h"
#include "../../../Engine/Math/Matrix4x4.h"

class Enemy;

// エネミーの状態名
enum EnemyState {
	kEnemyStateRoot, // 通常
	kEnemyStateDashSwingDown, // ダッシュ振り下ろし
	kEnemyStateOfCount // 数
};

/// <summary>
/// エネミーの状態基底クラス
/// </summary>
class IEnemyState
{

protected:

	// エネミーの状態番号
	static uint32_t enemyStateNo_;

	// エネミーのモーション番号
	static uint32_t enemyMotionNo_;

	// エネミー
	static Enemy* enemy_;

	//インスタンス
	static Input* input_;

	static Vector3Calc* v3Calc_;

	static Matrix4x4Calc* m4Calc_;

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~IEnemyState() = default;

	// プレイヤーの状態番号
	uint32_t GetEnemyStateNo() { return enemyStateNo_; }

	// プレイヤーのモーション番号
	uint32_t GetEnemyMotionNo() { return enemyMotionNo_; }

	// プレイヤーセット
	void SetEnemy(Enemy* enemy) { enemy_ = enemy; }



};

