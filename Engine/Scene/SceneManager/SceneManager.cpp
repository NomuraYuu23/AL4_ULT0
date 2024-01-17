#include "SceneManager.h"

SceneManager::~SceneManager()
{

	if (initializing_) {
		sceneInitialize_.join();
	}

}

void SceneManager::Initialize(uint32_t earlySceneNo)
{

	// シーンファクトリー
	sceneFacyory_ = SceneFactory::GetInstance();

	// シーン(タイトル)
	scene_.reset(sceneFacyory_->CreateScene(earlySceneNo));
	// シーンの静的初期化
	scene_->StaticInitialize();
	// シーンの初期化
	sceneInitialize_ = std::thread(std::bind(&SceneManager::InitializeThread, this));

	// 初期シーン
	currentSceneNo_ = earlySceneNo;

	requestSeneNo_ = earlySceneNo; // リクエストシーン
	prevRequestSeneNo_ = earlySceneNo; // 前のリクエストシーン

	// シーン遷移ファクトリー
	sceneTransitionFactory_ = SceneTransitionFactory::GetInstance();

	// シーン遷移を保持するメンバ変数
	sceneTransition_ = nullptr;

}

void SceneManager::Update()
{

	// シーンのチェック
	if (!initializing_) {
		currentSceneNo_ = scene_->GetSceneNo();

		prevRequestSeneNo_ = requestSeneNo_; // 前のリクエストシーン
		requestSeneNo_ = scene_->GetRequestSceneNo(); // リクエストシーン
	}

	// リクエストシーンが変わったか
	if (requestSeneNo_ != prevRequestSeneNo_) {
		//シーン遷移開始（初期化）
		sceneTransition_.reset(sceneTransitionFactory_->CreateSceneTransition(currentSceneNo_, requestSeneNo_));
		sceneTransition_->Initialize();
	}

	//シーン遷移中
	if (sceneTransition_) {
		// シーン遷移更新
		sceneTransition_->Update();
		if (sceneTransition_->GetSwitchScene()) {
			// シーン切り替え
			currentSceneNo_ = requestSeneNo_;
			scene_.reset(sceneFacyory_->CreateScene(currentSceneNo_));
			// シーンの初期化
			sceneInitialize_ = std::thread(std::bind(&SceneManager::InitializeThread, this));
			initializing_ = true;
			sceneTransition_->SetSwitchScene(false);
			sceneTransition_->SetStoppingUpdates(true);
		}
		else if (!sceneTransition_->GetTransitioning()) {
			sceneTransition_.reset(nullptr);
		}
	}

	// 更新処理
	if (!initializing_) {
		scene_->Update();
	}

	// 初期化終了
	if (initializeEnd_) {
		initializeEnd_ = false;
		sceneInitialize_.detach();
		if (sceneTransition_) {
			sceneTransition_->SetStoppingUpdates(false);
		}
	}

}

void SceneManager::Draw()
{
	// 描画処理
	if (!initializing_) {
		scene_->Draw();
	}
	if (sceneTransition_) {
		sceneTransition_->Draw();
	}

}

void SceneManager::InitializeThread()
{
	initializeEnd_ = false;
	initializing_ = true;
	scene_->Initialize();
	initializeEnd_ = true;
	initializing_ = false;

}
