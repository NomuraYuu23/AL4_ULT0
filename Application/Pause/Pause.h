#pragma once
#include "../../Engine/Input/Input.h"
#include "../../Engine/2D/Sprite.h"
#include <array>

/// <summary>
/// ポーズ
/// </summary>
class Pause
{

public: // サブクラス

	// ポーズメニュー
	enum PauseMenu{
		kGoToTitle,       // タイトルへ
		kReturnToGame,    // ゲームに戻る
		kCountOfPauseMenu // 使用不可
	};

	// ポーズで使うテクスチャ番号
	enum PauseTextureNo{
		kPausingTextureNo,              // ポーズ中
		kGoToTitleTextureNo,			   // タイトルへ
		kReturnToGameTextureNo,         // ゲームに戻る
		kCountOfPauseTextureNo // 使用不可
	};

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::array<uint32_t, PauseTextureNo::kCountOfPauseTextureNo>& textureHandles);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: // メンバ関数

	/// <summary>
	/// ポーズ画面の切り替え
	/// </summary>
	void PoseSwitching();

	/// <summary>
	/// ポーズメニュー操作
	/// </summary>
	void PauseMenuOperation();

	/// <summary>
	/// タイトルへ行く
	/// </summary>
	void PauseMenuGoToTitle();

	/// <summary>
	/// ゲームに戻る
	/// </summary>
	void PauseMenuReturnToGame();

public: // アクセッサ

	bool IsPause() { return isPause_; }

	bool GoToTheTitle() { return goToTheTitle_; }

private: // メンバ変数

	// 入力
	Input* input_ = nullptr;

	// ポーズしているか
	bool isPause_;
	// ポーズメニューでどこを選択しているか
	int pauseMenuSelect_;

	// タイトルへ戻るか
	bool goToTheTitle_;

	// テクスチャハンドル
	std::array<uint32_t, PauseTextureNo::kCountOfPauseTextureNo> textureHandles_;

private: // メンバ変数(スプライト)

	// ポーズ中
	std::unique_ptr<Sprite> pausingSprite_;
	// タイトルへ
	std::unique_ptr<Sprite> goToTitleSprite_;
	// ゲームに戻る
	std::unique_ptr<Sprite> returnToGameSprite_;

};

