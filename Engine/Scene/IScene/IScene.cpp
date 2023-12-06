#include "IScene.h"

//タイトルシーンで初期化
int IScene::sceneNo = kTitle;
int IScene::requestSeneNo = kTitle;

DirectXCommon* IScene::dxCommon_ = nullptr;
Input* IScene::input_ = nullptr;
Audio* IScene::audio_ = nullptr;

//ビュープロジェクション
ViewProjection IScene::viewProjection_;

//デバッグカメラ
std::unique_ptr<DebugCamera> IScene::debugCamera_;
bool IScene::isDebugCameraActive_;

//光源
std::unique_ptr<DirectionalLight> IScene::directionalLight_;


void IScene::StaticInitialize()
{
	//機能
	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	//ビュープロジェクション
	viewProjection_.Initialize();

	//デバッグカメラ
	debugCamera_ = std::make_unique<DebugCamera>();
	debugCamera_->Initialize();
	isDebugCameraActive_ = false;

	//光源
	directionalLight_.reset(DirectionalLight::Create());


}

IScene::~IScene(){}

int IScene::GetSceneNo(){ return sceneNo; }

int IScene::GetRequestSceneNo(){ return requestSeneNo; }

void IScene::ModelCreate(){}

void IScene::MaterialCreate(){}

void IScene::TextureLoad(){}
