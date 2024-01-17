#pragma once

#include "../IScene/IScene.h"
#include "../../../Application/Scene/SceneFactory/SceneFactory.h"
#include "../../SceneTransition/ISceneTransition/ISceneTransition.h"
#include "../../../Application/SceneTransition/SceneTransitionFactory/SceneTransitionFactory.h"
#include <thread>

/// <summary>
/// シーンマネージャー
/// </summary>
class SceneManager
{

public: // メンバ関数


	/// <summary>
	/// デストラクタ
	/// </summary>
	~SceneManager();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t earlySceneNo);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 初期化を回す関数
	/// </summary>
	void InitializeThread();

private: // メンバ変数

	// シーンファクトリー	
	SceneFactory* sceneFacyory_ = nullptr;

	// シーンを保持するメンバ変数
	std::unique_ptr<IScene> scene_;

	// どのステージを呼び出すかを管理する変数
	int currentSceneNo_; // 現在のシーン

	int requestSeneNo_; // リクエストシーン
	int prevRequestSeneNo_; // 前のリクエストシーン

	// シーン遷移ファクトリー
	SceneTransitionFactory* sceneTransitionFactory_ = nullptr;

	// シーン遷移を保持するメンバ変数
	std::unique_ptr<ISceneTransition> sceneTransition_;

	// 初期化用スレッド
	std::thread sceneInitialize_;

	// 初期化中か
	bool initializing_;

	// 初期化が終わったか
	bool initializeEnd_;

public:

	bool GetInitializing() { return initializing_; };
};

