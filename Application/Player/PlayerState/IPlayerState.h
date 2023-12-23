#pragma once
#include <cstdint>

class Player;

//プレイヤーの状態名
enum PlayerState {
	kPlayerStateStand, // 立っている状態
	kPlayerStateRun, // 走っている状態
	kPlayerStateOfCount // 数
};

/// <summary>
/// プレイヤーの状態基底クラス
/// </summary>
class IPlayerState
{
	
protected:

	// プレイヤーの状態番号
	static uint32_t plaryerStateNo_;

	// プレイヤーのモーション番号
	static uint32_t plaryerMotionNo_;

	// プレイヤー
	static Player* player_;

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
	virtual ~IPlayerState() = default;

	// プレイヤーの状態番号
	uint32_t GetPlaryerStateNo() { return plaryerStateNo_; }

	// プレイヤーのモーション番号
	uint32_t GetPlaryerMotionNo() { return plaryerMotionNo_; }

	// プレイヤーセット
	void SetPlayer(Player* player) { player_ = player; }

};

