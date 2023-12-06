#pragma once
#include "../../../Engine/SceneTransition/ISceneTransition/ISceneTransition.h"
#include <memory>
#include "../../../Engine/2D/Sprite.h"

class SceneTransitionBlackOut : public ISceneTransition{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private:

	std::unique_ptr<Sprite> sprite_;
	uint32_t textureHandle_;
	Vector4 color_;

};

