#include "GameScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/D3DResourceLeakChecker.h"

#include "../../Player/PlayerState/PlayerStateAttack/PlayerStateAttack.h"
#include "../../Enemy/EnemyState/EnemyStateDashSwingDown/EnemyStateDashSwingDown.h"

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {

	IScene::Initialize();

	ModelCreate();
	MaterialCreate();
	TextureLoad();

	// デバッグ描画
	colliderDebugDraw_ = std::make_unique<ColliderDebugDraw>();
	std::vector<Model*> colliderModels = { colliderSphereModel_.get(),colliderBoxModel_.get(),colliderBoxModel_.get() };
	colliderDebugDraw_->Initialize(colliderModels, colliderMaterial_.get());
	debugWorldTransform_.Initialize();

	pause_ = std::make_unique<Pause>();
	pause_->Initialize(pauseTextureHandles_);

	// ビュープロジェクション
	TransformStructure baseCameraTransform = {
		1.0f, 1.0f, 1.0f,
		0.58f,0.0f,0.0f,
		0.0f, 23.0f, -35.0f };
	camera_.SetTransform(baseCameraTransform);

	//パーティクル
	particleManager_ = ParticleManager::GetInstance();
	std::array<Model*, ParticleModelIndex::kCountofParticleModelIndex> particleModel;
	particleModel[ParticleModelIndex::kUvChecker] = particleUvcheckerModel_.get();
	particleModel[ParticleModelIndex::kCircle] = particleCircleModel_.get();
	particleManager_->ModelCreate(particleModel);
	//TransformStructure emitter = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{-3.0f,0.0f,0.0f} };
	//particleManager_->MakeEmitter(emitter, 1, 0.5f, 300.0f, ParticleModelIndex::kUvChecker, 0, 0);
	//emitter.translate.x = 3.0f;
	//particleManager_->MakeEmitter(emitter, 1, 0.5f, 300.0f, ParticleModelIndex::kCircle, 0, 0);

	// デバッグカメラ
	isDebugCameraActive_ = true;

	// オーディオマネージャー
	audioManager_ = std::make_unique<GameAudioManager>();
	audioManager_->StaticInitialize();
	audioManager_->Initialize();

	// コリジョンマネージャー
	collisionManager_ = std::make_unique<CollisionManager>();
	collisionManager_->Initialize();

	// ライト
	directionalLightData_.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData_.direction = Vector3Calc::Normalize(directionalLightData_.direction);
	directionalLightData_.intencity = 100.0f;

	// プレイヤー
	player_ = std::make_unique<Player>();
	std::array<Model*, PlayerPartIndex::kPlayerPartIndexOfCount> playerModels;
	for (uint32_t i = 0; i < playerModels_.size(); ++i) {
		playerModels[i] = playerModels_[i].get();
	}
	player_->Initialize(playerModels);

	// 追従カメラ
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(player_->GetWorldTransformAdress());
	player_->SetCamera(static_cast<BaseCamera*>(followCamera_.get()));

	// スカイドーム
	skyDome_ = std::make_unique<Skydome>();
	skyDome_->Initialize(skyDomeModel_.get());

	// 地面
	ground_ = std::make_unique<Ground>();
	ground_->Initialize(groundModel_.get());

	// エネミー
	enemy_ = std::make_unique<Enemy>();
	std::array<Model*, EnemyPartIndex::kEnemyPartIndexOfCount> EnemyModels;
	for (uint32_t i = 0; i < enemyModels_.size(); ++i) {
		EnemyModels[i] = enemyModels_[i].get();
	}
	enemy_->Initialize(EnemyModels, enemyWeaponModel_.get());
	enemy_->SetPlayer(player_.get());

}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update() {
	ImguiDraw();
	//光源
	directionalLight_->Update(directionalLightData_);

	// 追従カメラ
	followCamera_->Update();

	camera_ = static_cast<BaseCamera>(*followCamera_.get());

	// デバッグカメラ
	DebugCameraUpdate();

	// デバッグ描画
	colliderDebugDraw_->Update();

	//パーティクル
	particleManager_->Update(camera_);

	// ポーズ機能
	pause_->Update();

	// タイトルへ行く
	GoToTheTitle();

	// プレイヤー
	player_->Update();

	// エネミー
	enemy_->Update();

	// コリジョンマネージャー
	CollisonUpdate();

	// デバッグ
	if (player_->GetCurrentStateNo() == kPlayerStateAttack) {
		debugWorldTransform_.transform_.translate = static_cast<Capsule*>(static_cast<PlayerStateAttack*>(player_->GetPlayerState())->GetCollider())->segment_.origin_;
		debugWorldTransform_.transform_.scale.x = static_cast<Capsule*>(static_cast<PlayerStateAttack*>(player_->GetPlayerState())->GetCollider())->radius_;
		debugWorldTransform_.transform_.scale.y = static_cast<Capsule*>(static_cast<PlayerStateAttack*>(player_->GetPlayerState())->GetCollider())->radius_;
		debugWorldTransform_.transform_.scale.z = static_cast<Capsule*>(static_cast<PlayerStateAttack*>(player_->GetPlayerState())->GetCollider())->radius_;
		debugWorldTransform_.UpdateMatrix();
	}

}

/// <summary>
/// 描画処理
/// </summary>
void GameScene::Draw() {

	//ゲームの処理 

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();


#pragma endregion

	Model::PreDraw(dxCommon_->GetCommadList());

	//光源
	directionalLight_->Draw(dxCommon_->GetCommadList());

	//3Dオブジェクトはここ
	player_->Draw(camera_);
	skyDome_->Draw(camera_);
	ground_->Draw(camera_);
	enemy_->Draw(camera_);

#ifdef _DEBUG

	// デバッグ描画
	colliderDebugDraw_->Draw(camera_);

	colliderSphereModel_->Draw(debugWorldTransform_, camera_);

#endif // _DEBUG

	Model::PostDraw();

#pragma region パーティクル描画
	Model::PreParticleDraw(dxCommon_->GetCommadList(), camera_.GetViewProjectionMatrix());

	//光源
	directionalLight_->Draw(dxCommon_->GetCommadList());

	// パーティクルはここ
	particleManager_->Draw();

	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(dxCommon_->GetCommadList());


	//背景
	//前景スプライト描画
	pause_->Draw();


	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::ImguiDraw() {
#ifdef _DEBUG

	ImGui::Begin("Light");
	ImGui::DragFloat3("direction", &directionalLightData_.direction.x, 0.1f);
	ImGui::DragFloat("i", &directionalLightData_.intencity, 0.01f);
	ImGui::End();

	ImGui::Begin("FPS");
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();

	debugCamera_->ImGuiDraw();

	player_->ImGuiDraw();
	enemy_->ImGuiDraw();

#endif // _DEBUG

}

void GameScene::DebugCameraUpdate()
{

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_RETURN)) {
		if (isDebugCameraActive_) {
			isDebugCameraActive_ = false;
		}
		else {
			isDebugCameraActive_ = true;
		}
	}

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		// デバッグカメラのビュー行列をコピー
		camera_ = static_cast<BaseCamera>(*debugCamera_.get());
		// ビュー行列の転送
		camera_.Update();
	}
#endif

}

void GameScene::GoToTheTitle()
{

	if (pause_->GoToTheTitle()) {
		requestSceneNo = kTitle;
	}

}

void GameScene::CollisonUpdate()
{

	collisionManager_->ListClear();
	std::array<ColliderShape, PlayerColliderIndex::kPlayerColliderIndexOfCount> playerCollider = player_->GetCollider();
	for (uint32_t i = 0; i < playerCollider.size(); ++i) {
		collisionManager_->ListRegister(playerCollider[i]);
	}
	// プレイヤーの攻撃
	if (player_->GetCurrentStateNo() == kPlayerStateAttack) {
		if (static_cast<PlayerStateAttack*>(player_->GetPlayerState())->GetIsAttackJudgment()) {
			collisionManager_->ListRegister(static_cast<PlayerStateAttack*>(player_->GetPlayerState())->GetCollider());
		}
	}

	std::array<ColliderShape, EnemyColliderIndex::kEnemyColliderIndexOfCount> enemyCollider = enemy_->GetCollider();
	for (uint32_t i = 0; i < enemyCollider.size(); ++i) {
		collisionManager_->ListRegister(enemyCollider[i]);
	}	
	// エネミーの攻撃
	if (enemy_->GetCurrentStateNo() == kEnemyStateDashSwingDown) {
		if (static_cast<EnemyStateDashSwingDown*>(enemy_->GetEnemyState())->GetIsAttackJudgment()) {
			collisionManager_->ListRegister(static_cast<EnemyStateDashSwingDown*>(enemy_->GetEnemyState())->GetCollider());
		}
	}

	collisionManager_->CheakAllCollision();

}

void GameScene::ModelCreate()
{

	colliderSphereModel_.reset(Model::Create("Resources/default/collider/sphere/", "sphere.obj", dxCommon_, textureHandleManager_.get()));
	colliderBoxModel_.reset(Model::Create("Resources/default/collider/box/", "box.obj", dxCommon_, textureHandleManager_.get()));
	particleUvcheckerModel_.reset(Model::Create("Resources/default/", "plane.obj", dxCommon_, textureHandleManager_.get()));
	particleCircleModel_.reset(Model::Create("Resources/Particle/", "plane.obj", dxCommon_, textureHandleManager_.get()));

	// プレイヤー
	playerModels_[kPlayerPartHead].reset(Model::Create("Resources/Player/", "PlayerHead.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartTorso].reset(Model::Create("Resources/Player/", "PlayerTorso.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartLowerBack].reset(Model::Create("Resources/Player/", "PlayerLowerBack.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartLeftUpperArm].reset(Model::Create("Resources/Player/", "PlayerLeftUpperArm.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartLeftForearm].reset(Model::Create("Resources/Player/", "PlayerLeftForearm.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartLeftHand].reset(Model::Create("Resources/Player/", "PlayerLeftHand.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartRightUpperArm].reset(Model::Create("Resources/Player/", "PlayerRightUpperArm.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartRightForearm].reset(Model::Create("Resources/Player/", "PlayerRightForearm.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartRightHand].reset(Model::Create("Resources/Player/", "PlayerRightHand.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartLeftThigh].reset(Model::Create("Resources/Player/", "PlayerLeftThigh.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartLeftShin].reset(Model::Create("Resources/Player/", "PlayerLeftShin.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartLeftAnkle].reset(Model::Create("Resources/Player/", "PlayerLeftAnkle.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartRightThigh].reset(Model::Create("Resources/Player/", "PlayerRightThigh.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartRightShin].reset(Model::Create("Resources/Player/", "PlayerRightShin.obj", dxCommon_, textureHandleManager_.get()));
	playerModels_[kPlayerPartRightAnkle].reset(Model::Create("Resources/Player/", "PlayerRightAnkle.obj", dxCommon_, textureHandleManager_.get()));

	// スカイドーム
	skyDomeModel_.reset(Model::Create("Resources/skydome/", "skydome.obj", dxCommon_, textureHandleManager_.get()));
	// 地面モデル
	groundModel_.reset(Model::Create("Resources/Ground/", "Ground.obj", dxCommon_, textureHandleManager_.get()));

	// エネミー
	enemyModels_[kEnemyPartHead].reset(Model::Create("Resources/Enemy/", "EnemyHead.obj", dxCommon_, textureHandleManager_.get()));
	enemyModels_[kEnemyPartTorso].reset(Model::Create("Resources/Enemy/", "EnemyTorso.obj", dxCommon_, textureHandleManager_.get()));
	enemyModels_[kEnemyPartLowerBack].reset(Model::Create("Resources/Enemy/", "EnemyLowerBack.obj", dxCommon_, textureHandleManager_.get()));
	enemyModels_[kEnemyPartLeftUpperArm].reset(Model::Create("Resources/Enemy/", "EnemyLeftUpperArm.obj", dxCommon_, textureHandleManager_.get()));
	enemyModels_[kEnemyPartLeftForearm].reset(Model::Create("Resources/Enemy/", "EnemyLeftForearm.obj", dxCommon_, textureHandleManager_.get()));
	enemyModels_[kEnemyPartLeftHand].reset(Model::Create("Resources/Enemy/", "EnemyLeftHand.obj", dxCommon_, textureHandleManager_.get()));
	enemyModels_[kEnemyPartRightUpperArm].reset(Model::Create("Resources/Enemy/", "EnemyRightUpperArm.obj", dxCommon_, textureHandleManager_.get()));
	enemyModels_[kEnemyPartRightForearm].reset(Model::Create("Resources/Enemy/", "EnemyRightForearm.obj", dxCommon_, textureHandleManager_.get()));
	enemyModels_[kEnemyPartRightHand].reset(Model::Create("Resources/Enemy/", "EnemyRightHand.obj", dxCommon_, textureHandleManager_.get()));
	enemyModels_[kEnemyPartLeftThigh].reset(Model::Create("Resources/Enemy/", "EnemyLeftThigh.obj", dxCommon_, textureHandleManager_.get()));
	enemyModels_[kEnemyPartLeftShin].reset(Model::Create("Resources/Enemy/", "EnemyLeftShin.obj", dxCommon_, textureHandleManager_.get()));
	enemyModels_[kEnemyPartLeftAnkle].reset(Model::Create("Resources/Enemy/", "EnemyLeftAnkle.obj", dxCommon_, textureHandleManager_.get()));
	enemyModels_[kEnemyPartRightThigh].reset(Model::Create("Resources/Enemy/", "EnemyRightThigh.obj", dxCommon_, textureHandleManager_.get()));
	enemyModels_[kEnemyPartRightShin].reset(Model::Create("Resources/Enemy/", "EnemyRightShin.obj", dxCommon_, textureHandleManager_.get()));
	enemyModels_[kEnemyPartRightAnkle].reset(Model::Create("Resources/Enemy/", "EnemyRightAnkle.obj", dxCommon_, textureHandleManager_.get()));
	enemyWeaponModel_.reset(Model::Create("Resources/EnemyWeapon/", "EnemyWeapon.obj", dxCommon_, textureHandleManager_.get()));

}

void GameScene::MaterialCreate()
{

	colliderMaterial_.reset(Material::Create());

}

void GameScene::TextureLoad()
{

	// ポーズ
	pauseTextureHandles_ = {
		TextureManager::Load("Resources/default/pause/pausing.png", dxCommon_, textureHandleManager_.get()),
		TextureManager::Load("Resources/default/pause/goToTitle.png", dxCommon_, textureHandleManager_.get()),
		TextureManager::Load("Resources/default/pause/returnToGame.png", dxCommon_, textureHandleManager_.get()),
	};

}
