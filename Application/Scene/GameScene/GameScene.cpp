#include "GameScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/D3DResourceLeakChecker.h"

#include "../../Player/PlayerState/PlayerStateAttack/PlayerStateAttack.h"
#include "../../Enemy/EnemyState/EnemyStateMowingDown/EnemyStateMowingDown.h"

/// <summary>
/// 初期化
/// </summary>
void GameScene::Initialize() {

	IScene::Initialize();

	ModelCreate();
	TextureLoad();

	// デバッグ描画
	colliderMaterial_.reset(Material::Create());
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
	isDebugCameraActive_ = false;

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
	player_->Initialize(playerModels, playerWeaponModel_.get());

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

	// UI
	UIManager_ = std::make_unique<UIManager>();
	UIManager_->Initialize(UITextureHandles_);

	// 平行光源
	directionalLight_ = std::make_unique<DirectionalLight>();
	directionalLight_->Initialize();
	directionalLightData_.color = { 0.1f,0.1f,0.1f,1.0f };
	directionalLightData_.direction = { 0.0f, -1.0f, 0.0f };
	directionalLightData_.intencity = 1.0f;
	directionalLight_->Update(directionalLightData_);

	// 点光源
	pointLightManager_ = std::make_unique<PointLightManager>();
	pointLightManager_->Initialize();
	for (size_t i = 0; i < pointLightDatas_.size(); ++i) {
		pointLightDatas_[i].color = { 1.0f,1.0f,1.0f,1.0f };
		pointLightDatas_[i].position = { 100.0f * i, 5.0f, 0.0f };
		pointLightDatas_[i].intencity = 1.0f;
		pointLightDatas_[i].radius = 100.0f;
		pointLightDatas_[i].decay = 2.0f;
		pointLightDatas_[i].used = false;
	}

	pointLightDatas_[0].used = true;
	pointLightDatas_[1].color = { 0.8f,0.0f,0.1f,1.0f };
	pointLightDatas_[1].used = true;

	// スポットライト
	spotLightManager_ = std::make_unique<SpotLightManager>();
	spotLightManager_->Initialize();
	for (size_t i = 0; i < spotLightDatas_.size(); ++i) {
	//	spotLightDatas_[i].color = { 1.0f,1.0f,1.0f,1.0f };
	//	spotLightDatas_[i].position = { 100.0f * i, 5.0f, 0.0f };
		spotLightDatas_[i].intencity = 10.0f;
	//	spotLightDatas_[i].direction = { -1.0f, 0.0f, 0.0f };
		spotLightDatas_[i].distance = 100.0f;
		spotLightDatas_[i].decay = 2.0f;
		spotLightDatas_[i].cosAngle = 0.5f;
		spotLightDatas_[i].cosFalloffStart = 1.0f;
	//	spotLightDatas_[i].used = false;
	}

	spotLightDatas_[0].color = { 0.8f,0.0f,0.0f,1.0f };
	spotLightDatas_[0].position = { 40.0f, 5.0f, 0.0f };
	spotLightDatas_[0].direction = { -1.0f, 0.0f, 0.0f };
	spotLightDatas_[0].used = true;

	spotLightDatas_[1].color = { 0.0f,0.8f,0.0f,1.0f };
	spotLightDatas_[1].position = { 0.0f, 5.0f, 40.0f };
	spotLightDatas_[1].direction = { 0.0f, 0.0f, -1.0f };
	spotLightDatas_[1].used = true;

	spotLightDatas_[2].color = { 0.0f,0.0f,0.8f,1.0f };
	spotLightDatas_[2].position = { -40.0f , 5.0f, 0.0f };
	spotLightDatas_[2].direction = { 1.0f, 0.0f, 0.0f };
	spotLightDatas_[2].used = true;

	spotLightDatas_[3].color = { 1.0f,1.0f,1.0f,1.0f };
	spotLightDatas_[3].position = { 100.0f, 5.0f, 0.0f };
	spotLightDatas_[3].direction = { -1.0f, 0.0f, 0.0f };
	spotLightDatas_[3].used = false;

}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update() {

	// タイトルへ
	if (enemy_->GetIsDead()) {
		requestSceneNo = kClear;
	}
	if (player_->GetIsDead()) {
		requestSceneNo = kGameOver;
	}


	ImguiDraw();
	//光源
	directionalLight_->Update(directionalLightData_);
	pointLightDatas_[0].position = player_->GetWorldTransformAdress()->GetWorldPosition();
	pointLightDatas_[1].position = enemy_->GetWorldTransformAdress()->GetWorldPosition();
	pointLightManager_->Update(pointLightDatas_);
	spotLightManager_->Update(spotLightDatas_);

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

	// UI
	UIManager_->Update(player_->RatioHP(), enemy_->RatioHP());

	// デバッグ
	if (enemy_->GetCurrentStateNo() == kEnemyStateMowingDown) {
		debugWorldTransform_.transform_.translate = static_cast<Capsule*>(static_cast<EnemyStateMowingDown*>(enemy_->GetEnemyState())->GetCollider())->segment_.origin_;
		debugWorldTransform_.transform_.scale.x = static_cast<Capsule*>(static_cast<EnemyStateMowingDown*>(enemy_->GetEnemyState())->GetCollider())->radius_;
		debugWorldTransform_.transform_.scale.y = static_cast<Capsule*>(static_cast<EnemyStateMowingDown*>(enemy_->GetEnemyState())->GetCollider())->radius_;
		debugWorldTransform_.transform_.scale.z = static_cast<Capsule*>(static_cast<EnemyStateMowingDown*>(enemy_->GetEnemyState())->GetCollider())->radius_;
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

	Model::PreDraw(dxCommon_->GetCommadList(), pointLightManager_.get(), spotLightManager_.get());

	//光源
	directionalLight_->Draw(dxCommon_->GetCommadList(), 3);

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
	directionalLight_->Draw(dxCommon_->GetCommadList(), 3);

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
	UIManager_->Draw();

	// 前景スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion

}

void GameScene::ImguiDraw() {
#ifdef _DEBUG

	ImGui::Begin("Light");
	ImGui::DragFloat3("SpotPosition0", &spotLightDatas_[0].position.x, 0.1f);
	ImGui::DragFloat("SpotIntencity0", &spotLightDatas_[0].intencity, 0.01f);
	ImGui::DragFloat3("SpotDirection0", &spotLightDatas_[0].direction.x, 0.1f);
	ImGui::DragFloat("SpotDistance0", &spotLightDatas_[0].distance, 0.01f);
	ImGui::DragFloat("SpotDecay0", &spotLightDatas_[0].decay, 0.01f);
	ImGui::DragFloat("SpotCosAngle0", &spotLightDatas_[0].cosAngle, 0.01f);
	ImGui::DragFloat("SpotCosFalloffStart0", &spotLightDatas_[0].cosFalloffStart, 0.01f);

	ImGui::DragFloat3("SpotPosition1", &spotLightDatas_[1].position.x, 0.1f);
	ImGui::DragFloat("SpotIntencity1", &spotLightDatas_[1].intencity, 0.01f);
	ImGui::DragFloat3("SpotDirection1", &spotLightDatas_[1].direction.x, 0.1f);
	ImGui::DragFloat("SpotDistance1", &spotLightDatas_[1].distance, 0.01f);
	ImGui::DragFloat("SpotDecay1", &spotLightDatas_[1].decay, 0.01f);
	ImGui::DragFloat("SpotCosAngle1", &spotLightDatas_[1].cosAngle, 0.01f);
	ImGui::DragFloat("SpotCosFalloffStart1", &spotLightDatas_[1].cosFalloffStart, 0.01f);

	ImGui::DragFloat3("SpotPosition2", &spotLightDatas_[2].position.x, 0.1f);
	ImGui::DragFloat("SpotIntencity2", &spotLightDatas_[2].intencity, 0.01f);
	ImGui::DragFloat3("SpotDirection2", &spotLightDatas_[2].direction.x, 0.1f);
	ImGui::DragFloat("SpotDistance2", &spotLightDatas_[2].distance, 0.01f);
	ImGui::DragFloat("SpotDecay2", &spotLightDatas_[2].decay, 0.01f);
	ImGui::DragFloat("SpotCosAngle2", &spotLightDatas_[2].cosAngle, 0.01f);
	ImGui::DragFloat("SpotCosFalloffStart2", &spotLightDatas_[2].cosFalloffStart, 0.01f);

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

	// リストクリア
	collisionManager_->ListClear();
	
	// プレイヤー
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

	// エネミー
	std::array<ColliderShape, EnemyColliderIndex::kEnemyColliderIndexOfCount> enemyCollider = enemy_->GetCollider();
	for (uint32_t i = 0; i < enemyCollider.size(); ++i) {
		collisionManager_->ListRegister(enemyCollider[i]);
	}	

	// エネミーの攻撃
	//if (enemy_->GetCurrentStateNo() == kEnemyStateDashSwingDown) {
	//	if (static_cast<EnemyStateDashSwingDown*>(enemy_->GetEnemyState())->GetIsAttackJudgment()) {
	//		collisionManager_->ListRegister(static_cast<EnemyStateDashSwingDown*>(enemy_->GetEnemyState())->GetCollider());
	//	}
	//}
	if (enemy_->GetCurrentStateNo() == kEnemyStateMowingDown) {
		if (static_cast<EnemyStateMowingDown*>(enemy_->GetEnemyState())->GetIsAttackJudgment()) {
			collisionManager_->ListRegister(static_cast<EnemyStateMowingDown*>(enemy_->GetEnemyState())->GetCollider());
		}
	}

	// 確認
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
	playerWeaponModel_.reset(Model::Create("Resources/PlayerWeapon/", "PlayerWeapon.obj", dxCommon_, textureHandleManager_.get()));

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

void GameScene::TextureLoad()
{

	// ポーズ
	pauseTextureHandles_ = {
		TextureManager::Load("Resources/default/pause/pausing.png", dxCommon_, textureHandleManager_.get()),
		TextureManager::Load("Resources/default/pause/goToTitle.png", dxCommon_, textureHandleManager_.get()),
		TextureManager::Load("Resources/default/pause/returnToGame.png", dxCommon_, textureHandleManager_.get()),
	};

	// UI
	UITextureHandles_ = {
		TextureManager::Load("Resources/UI/attack.png", dxCommon_, textureHandleManager_.get()),
		TextureManager::Load("Resources/UI/rolling.png", dxCommon_, textureHandleManager_.get()),
		TextureManager::Load("Resources/default/white2x2.png", dxCommon_, textureHandleManager_.get()),
	};

}
