#include "UIManager.h"

void UIManager::Initialize(const std::array<uint32_t, kUIIndexOfCount>& textureHandles)
{

	textureHandles_ = textureHandles;

	Vector2 position = { 640.0f, 360.0f };
	Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };

	position = { 1280.0f - 160.0f - 20.0f, 720.0f - 40.0f - 20.0f - 80.0f};
	attackSprite_.reset(Sprite::Create(textureHandles_[kUIIndexAttack], position, color));
	position = { 1280.0f - 160.0f - 20.0f, 720.0f - 40.0f - 20.0f };
	rollingSprite_.reset(Sprite::Create(textureHandles_[kUIIndexRolling], position, color));

}

void UIManager::Update()
{



}

void UIManager::Draw()
{

	attackSprite_->Draw();
	rollingSprite_->Draw();

}
