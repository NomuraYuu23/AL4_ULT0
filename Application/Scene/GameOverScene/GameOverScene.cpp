#include "GameOverScene.h"
#include "../../../Engine/Math/Ease.h"
#include "../../../Engine/base/TextureManager.h"

void GameOverScene::Initialize()
{

	IScene::Initialize();

	ModelCreate();
	MaterialCreate();
	TextureLoad();

	gameOverSprite_.reset(Sprite::Create(gameOverTextureHandle_, { 640.0f, 360.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));
	buttonSprite_.reset(Sprite::Create(buttonTextureHandle_, { 640.0f, 500.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }));
	buttonAlphaT_ = 0.0f;
	buttonAlphaTSpeed_ = 0.01f;
	buttonItIncreaseAlphaT_ = true;
	buttonColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };

}

void GameOverScene::Update()
{

	if (input_->TriggerJoystick(JoystickButton::kJoystickButtonA)) {
		// 行きたいシーンへ
		requestSceneNo = kTitle;
	}

	// ボタンスプライト
	if (buttonItIncreaseAlphaT_) {
		buttonAlphaT_ += buttonAlphaTSpeed_;
		if (buttonAlphaT_ >= 1.0f) {
			buttonAlphaT_ = 1.0f;
			buttonItIncreaseAlphaT_ = false;
		}
	}
	else {
		buttonAlphaT_ -= buttonAlphaTSpeed_;
		if (buttonAlphaT_ <= 0.0f) {
			buttonAlphaT_ = 0.0f;
			buttonItIncreaseAlphaT_ = true;
		}
	}
	buttonColor_.w = Ease::Easing(Ease::EaseName::Lerp, 0.0f, 1.0f, buttonAlphaT_);
	buttonSprite_->SetColor(buttonColor_);

}

void GameOverScene::Draw()
{

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();


#pragma endregion

	Model::PreDraw(dxCommon_->GetCommadList());

	//3Dオブジェクトはここ

	Model::PostDraw();

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());


	//背景
	//前景スプライト描画
	gameOverSprite_->Draw();
	buttonSprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameOverScene::ModelCreate()
{
}

void GameOverScene::MaterialCreate()
{
}

void GameOverScene::TextureLoad()
{

	gameOverTextureHandle_ = TextureManager::Load("Resources/OutGame/over.png", dxCommon_, textureHandleManager_.get());
	buttonTextureHandle_ = TextureManager::Load("Resources/OutGame/button.png", dxCommon_, textureHandleManager_.get());

}
