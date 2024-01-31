#pragma once
#include "../../../Engine/Scene/IScene/IScene.h"
#include <memory>

class TitleScene : public IScene
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() override;

private: // メンバ関数

	/// <summary>
	/// モデルクリエイト
	/// </summary>
	void ModelCreate() override;

	/// <summary>
	/// マテリアルクリエイト
	/// </summary>
	void MaterialCreate() override;

	/// <summary>
	/// テクスチャロード
	/// </summary>
	void TextureLoad() override;

private:

	// タイトル
	std::unique_ptr<Sprite> titleSprite_;
	uint32_t titleTextureHandle_;

	// 点滅用媒介変数
	float titleAlphaT_;
	// 点滅用媒介変数速度
	float titleAlphaTSpeed_;
	// 点滅用媒介変数は増えるか
	bool titleItIncreaseAlphaT_;
	// 色
	Vector4 titleColor_;

	// ボタン
	std::unique_ptr<Sprite> buttonSprite_;
	uint32_t buttonTextureHandle_;
	// 点滅用媒介変数
	float buttonAlphaT_;
	// 点滅用媒介変数速度
	float buttonAlphaTSpeed_;
	// 点滅用媒介変数は増えるか
	bool buttonItIncreaseAlphaT_;
	// 色
	Vector4 buttonColor_;

};

