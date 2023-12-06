#include "Pause.h"

void Pause::Initialize(const std::array<uint32_t, PauseTextureNo::kCountOfPauseTextureNo>& textureHandles)
{

	input_ = Input::GetInstance();
	isPause_ = false;
	pauseMenuSelect_ = PauseMenu::kReturnToGame;
	goToTheTitle_ = false;
	textureHandles_ = textureHandles;

	// スプライト
	Vector4 color = {1.0f,1.0f,1.0f,1.0f};
	Vector2 position = { 640.0f, 180.0f };
	pausingSprite_.reset(Sprite::Create(textureHandles_[PauseTextureNo::kPausingTextureNo], position, color));
	position = { 640.0f, 360.0f };
	goToTitleSprite_.reset(Sprite::Create(textureHandles_[PauseTextureNo::kGoToTitleTextureNo], position, color));
	position = { 640.0f, 450.0f };
	returnToGameSprite_.reset(Sprite::Create(textureHandles_[PauseTextureNo::kReturnToGameTextureNo], position, color));

}

void Pause::Update()
{

	PoseSwitching();

	if (isPause_) {
		PauseMenuOperation();
	}

}

void Pause::Draw()
{

	if (isPause_) {
		pausingSprite_->Draw();
		goToTitleSprite_->Draw();
		returnToGameSprite_->Draw();
	}

}

void Pause::PoseSwitching()
{

	if (input_->TriggerKey(DIK_TAB)) {
		if (isPause_) {
			isPause_ = false;
		}
		else {
			isPause_ = true;
			goToTheTitle_ = false;
			pauseMenuSelect_ = PauseMenu::kReturnToGame;
		}
	}

}

void Pause::PauseMenuOperation()
{

	// メニュー移動(上)
	if (input_->TriggerKey(DIK_W) || input_->TriggerKey(DIK_UP)) {
		pauseMenuSelect_--;
		if (pauseMenuSelect_ < 0) {
			pauseMenuSelect_ = PauseMenu::kCountOfPauseMenu - 1;
		}
	}
	// メニュー移動(下)
	else if (input_->TriggerKey(DIK_S) || input_->TriggerKey(DIK_DOWN)) {
		pauseMenuSelect_++;
		if (pauseMenuSelect_ == PauseMenu::kCountOfPauseMenu) {
			pauseMenuSelect_ = 0;
		}
	}

	switch (pauseMenuSelect_)
	{
	case PauseMenu::kGoToTitle:
		PauseMenuGoToTitle();
		break;
	case PauseMenu::kReturnToGame:
		PauseMenuReturnToGame();
		break;
	default:
		break;
	}

}

void Pause::PauseMenuGoToTitle()
{

	// 選択している部分を色変更(黒)
	Vector4 black = { 0.0f,0.0f,0.0f,1.0f };
	Vector4 white = { 1.0f,1.0f,1.0f,1.0f };

	goToTitleSprite_->SetColor(black);
	returnToGameSprite_->SetColor(white);

	if (input_->TriggerKey(DIK_SPACE)) {
		goToTheTitle_ = true;
	}


}

void Pause::PauseMenuReturnToGame()
{

	// 選択している部分を色変更(黒)
	Vector4 black = { 0.0f,0.0f,0.0f,1.0f };
	Vector4 white = { 1.0f,1.0f,1.0f,1.0f };

	returnToGameSprite_->SetColor(black);
	goToTitleSprite_->SetColor(white);

	if (input_->TriggerKey(DIK_SPACE)) {
		isPause_ = false;
	}

}
