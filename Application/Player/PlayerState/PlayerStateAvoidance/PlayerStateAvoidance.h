#pragma once
#include "../IPlayerState.h"
#include "../../../../Engine/Math/Vector3.h"
#include "../../../../Engine/3D/WorldTransform.h"

class PlayerStateAvoidance : public IPlayerState
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

	float spped_;

	uint32_t frame_;

	uint32_t frameCount_;

};

