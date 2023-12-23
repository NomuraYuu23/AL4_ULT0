#pragma once
#include "../IPlayerState.h"
#include "../../../../Engine/Math/Vector3.h"
class PlayerStateRun : public IPlayerState
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: // メンバ変数

	float speed_;

	Vector3 targetDirection_;

	float targetAngleT_;

};

