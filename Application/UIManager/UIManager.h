#pragma once
#include "../../Engine/2D/Sprite.h"

enum UIIndex {
	kUIIndexAttack, // 攻撃
	kUIIndexRolling,// 回避
	kUIIndexOfCount,
};

class UIManager
{

public:

	// 初期化
	void Initialize(const std::array<uint32_t, kUIIndexOfCount>& textureHandles);

	// 更新
	void Update();

	// 描画
	void Draw();

private:

	std::unique_ptr<Sprite> attackSprite_;
	std::unique_ptr<Sprite> rollingSprite_;

	std::array<uint32_t, kUIIndexOfCount> textureHandles_;

};

