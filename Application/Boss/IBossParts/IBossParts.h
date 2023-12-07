#pragma once
#include <vector>
#include "../../../Engine/Animation/IBone.h"

/// <summary>
/// ボスのパーツ基底クラス
/// </summary>
class IBossParts : public IBone
{

public:

	~IBossParts() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	virtual void Initialize(Model* model) override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

public:

	virtual void animationTransformChange();

protected: // アニメーション関数



};

