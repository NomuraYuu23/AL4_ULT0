#pragma once
#include "../../Engine/2D/Sprite.h"

enum UIIndex {
	kUIIndexAttack, // 攻撃
	kUIIndexRolling,// 回避
	kUIIndexHP,// HP
	kUIIndexOfCount,
};

class UIManager
{

public:

	// 初期化
	void Initialize(const std::array<uint32_t, kUIIndexOfCount>& textureHandles);

	// 更新
	void Update(float playerRatioHP, float enemyRatioHP);

	// 描画
	void Draw();

private:
	
	// 操作UI
	std::unique_ptr<Sprite> attackSprite_;
	std::unique_ptr<Sprite> rollingSprite_;

	// HP
	std::unique_ptr<Sprite> playerHPSprite_;
	std::unique_ptr<Sprite> playerHPFrameSprite_;
	float playerHPSizeX_;
	std::unique_ptr<Sprite> enemyHPSprite_;
	std::unique_ptr<Sprite> enemyHPFrameSprite_;
	float enemyHPSizeX_;
	
	std::array<uint32_t, kUIIndexOfCount> textureHandles_;

};

