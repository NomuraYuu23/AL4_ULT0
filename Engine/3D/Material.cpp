#include "Material.h"
#include <cassert>

#include <fstream>
#include <sstream>
#include <format>
#include "../base/BufferResource.h"

using namespace DirectX;
using namespace Microsoft::WRL;

// デバイス
ID3D12Device* Material::sDevice = nullptr;

/// <summary>
/// 静的初期化
/// </summary>
/// <param name="device">デバイス</param>
void Material::StaticInitialize(ID3D12Device* device) {

	assert(device);

	sDevice = device;

}

/// <summary>
/// マテリアル生成
/// </summary>
/// <returns></returns>
Material* Material::Create() {

	// 3Dオブジェクトのインスタンスを生成
	Material* material = new Material();
	assert(material);

	// 初期化
	material->Initialize();

	return material;

}

/// <summary>
/// 初期化
/// </summary>
void Material::Initialize() {

	//Sprite用のマテリアルリソースを作る
	materialBuff_ = BufferResource::CreateBufferResource(sDevice, (sizeof(MaterialData) + 0xff) & ~0xff);
	//書き込むためのアドレスを取得
	materialBuff_->Map(0, nullptr, reinterpret_cast<void**>(&materialMap_));
	//白を書き込んでみる
	materialMap_->color = { 1.0f, 1.0f, 1.0f, 1.0f };
	//SpriteはLightingしないのでfalseを設定する
	materialMap_->enableLighting = None;	//UVTransfome初期化
	materialMap_->uvTransform = Matrix4x4Calc::GetInstance()->MakeIdentity4x4();
	materialMap_->shininess = 1.0f;

	uvTransform = {
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

}

/// <summary>
/// 更新
/// </summary>
void Material::Update(const TransformStructure& transform, const Vector4& color, int enableLighting, float shininess) {

	uvTransform = transform;

	//UVTransfome用
	Matrix4x4 uvTransformMatrix = Matrix4x4Calc::GetInstance()->MakeAffineMatrix(uvTransform.scale, uvTransform.rotate, uvTransform.translate);
	materialMap_->uvTransform = uvTransformMatrix;

	materialMap_->color = color;

	materialMap_->enableLighting = enableLighting;

	materialMap_->shininess = shininess;

}
