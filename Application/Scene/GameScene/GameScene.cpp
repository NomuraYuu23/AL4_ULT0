#include "GameScene.h"
#include "../../../Engine/base/WinApp.h"
#include "../../../Engine/base/TextureManager.h"
#include "../../../Engine/2D/ImguiManager.h"
#include "../../../Engine/base/D3DResourceLeakChecker.h"

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
	TransformStructure emitter = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{-3.0f,0.0f,0.0f} };
	particleManager_->MakeEmitter(emitter, 1, 0.5f, 300.0f, ParticleModelIndex::kUvChecker, 0, 0);
	emitter.translate.x = 3.0f;
	particleManager_->MakeEmitter(emitter, 1, 0.5f, 300.0f, ParticleModelIndex::kCircle, 0, 0);

	isDebugCameraActive_ = true;

	model_.reset(Model::Create("Resources/default/", "Ball.obj", dxCommon_, textureHandleManager_.get()));
	material_.reset(Material::Create());
	material_->Initialize();
	TransformStructure uvTransform = {
	{1.0f,1.0f,1.0f},
	{0.0f,0.0f,0.0f},
	{0.0f,0.0f,0.0f},
	};
	Vector4 color = { 1.0f,1.0f,1.0f,1.0f };
	material_->Update(uvTransform, color, BlinnPhongReflection, 100.0f);

	worldTransform_.Initialize();

	sampleBone_ = std::make_unique<SampleBone>();
	sampleBone_->Initialize(model_.get());

	audioManager_ = std::make_unique<GameAudioManager>();
	audioManager_->StaticInitialize();
	audioManager_->Initialize();
	audioManager_->PlayWave(GameAudioNameIndex::kSample);

}

/// <summary>
/// 更新処理
/// </summary>
void GameScene::Update(){
	ImguiDraw();
	//光源
	DirectionalLightData directionalLightData;
	directionalLightData.color = { 1.0f,1.0f,1.0f,1.0f };
	directionalLightData.direction = Vector3Calc::Normalize(direction);
	directionalLightData.intencity = intencity;
	directionalLight_->Update(directionalLightData);

	camera_.Update();

	worldTransform_.UpdateMatrix();

	// デバッグ
	sampleBone_->Update();

	// デバッグカメラ
	DebugCameraUpdate();
	
	// デバッグ描画
	colliderDebugDraw_->Update();
	
	//パーティクル
	particleManager_->Update(debugCamera_->GetTransformMatrix());

	// ポーズ機能
	pause_->Update();

	// タイトルへ行く
	GoToTheTitle();

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

	model_->Draw(worldTransform_, camera_, material_.get());
	sampleBone_->Draw(camera_);

#ifdef _DEBUG

	// デバッグ描画
	colliderDebugDraw_->Draw(camera_);

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

void GameScene::ImguiDraw(){
#ifdef _DEBUG

	ImGui::Begin("Light");
	ImGui::DragFloat3("direction", &direction.x, 0.1f);
	ImGui::DragFloat3("worldtransform", &worldTransform_.transform_.scale.x, 0.1f);
	ImGui::DragFloat("i", &intencity, 0.01f);
	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);
	ImGui::End();

	debugCamera_->ImGuiDraw();

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

void GameScene::ModelCreate()
{

	colliderSphereModel_.reset(Model::Create("Resources/TD2_November/collider/sphere/", "sphere.obj", dxCommon_, textureHandleManager_.get()));
	colliderBoxModel_.reset(Model::Create("Resources/TD2_November/collider/box/", "box.obj", dxCommon_, textureHandleManager_.get()));
	particleUvcheckerModel_.reset(Model::Create("Resources/default/", "plane.obj", dxCommon_, textureHandleManager_.get()));
	particleCircleModel_.reset(Model::Create("Resources/Particle/", "plane.obj", dxCommon_, textureHandleManager_.get()));

}

void GameScene::MaterialCreate()
{

	colliderMaterial_.reset(Material::Create());

}

void GameScene::TextureLoad()
{

	// ポーズ
	pauseTextureHandles_ = {
		TextureManager::Load("Resources/TD2_November/pause/pausing.png", dxCommon_, textureHandleManager_.get()),
		TextureManager::Load("Resources/TD2_November/pause/goToTitle.png", dxCommon_, textureHandleManager_.get()),
		TextureManager::Load("Resources/TD2_November/pause/returnToGame.png", dxCommon_, textureHandleManager_.get()),
	};

}
