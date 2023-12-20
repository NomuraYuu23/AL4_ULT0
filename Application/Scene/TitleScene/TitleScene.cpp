#include "TitleScene.h"

void TitleScene::Initialize()
{

	IScene::Initialize();

	ModelCreate();
	MaterialCreate();
	TextureLoad();

}

void TitleScene::Update()
{

	if (input_->TriggerKey(DIK_SPACE)) {
		// 行きたいシーンへ
		requestSceneNo = kGame;
	}

}

void TitleScene::Draw()
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

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void TitleScene::ModelCreate()
{
}

void TitleScene::MaterialCreate()
{
}

void TitleScene::TextureLoad()
{
}
