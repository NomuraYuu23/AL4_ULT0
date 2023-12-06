#include "SceneTransitionBlackOut.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/base/DirectXCommon.h"
#include "../../../Engine/base/WinApp.h"

void SceneTransitionBlackOut::Initialize()
{

	ISceneTransition::Initialize();

	// テクスチャ読み込み
	textureHandle_ = TextureManager::Load("Resources/default/white2x2.png", DirectXCommon::GetInstance());
	color_ = { 0.0f,0.0f,0.0f,0.0f };
	Vector2 position = { static_cast<float>(WinApp::kWindowWidth) / 2.0f, static_cast<float>(WinApp::kWindowHeight) / 2.0f };
	sprite_.reset(Sprite::Create(textureHandle_, position, color_));
	sprite_->SetSize(Vector2{ static_cast<float>(WinApp::kWindowWidth), static_cast<float>(WinApp::kWindowHeight) });

}

void SceneTransitionBlackOut::Update()
{

	ISceneTransition::Update();

	// α値変更
	if (isFadeIn_) {
		color_.w = fadeTimer_ / fadeInTime_;
	}
	else {
		color_.w = 1.0f - fadeTimer_ / fadeOutTime_;
	}
	sprite_->SetColor(color_);

}

void SceneTransitionBlackOut::Draw()
{

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(DirectXCommon::GetInstance()->GetCommadList());

	sprite_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}
