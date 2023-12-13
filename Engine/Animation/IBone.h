#pragma once
#include "../3D/WorldTransform.h"
#include "../3D/ViewProjection.h"
#include "../3D/Model.h"
#include "../3D/Material.h"
#include "../Collider/Collider.h"
#include "../Collider/ColliderShape.h"
#include <memory>
#include <map>
#include "BoneData.h"

/// <summary>
/// ボーン
/// </summary>
class IBone
{
public: // メンバ関数

	/// <summary>
	///  デストラクタ
	/// </summary>
	~IBone() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	virtual void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(uint32_t frameCount);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const ViewProjection& viewProjection, Material* material);

public:

	void SetWorldTransform(WorldTransform worldTransform) { worldTransform_ = worldTransform; }

	WorldTransform GetWorldTransform() { return worldTransform_; }

	ColliderShape* GetCollider() { return collider_.get(); };

protected: // 

	// モデル
	Model* model_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

	// コライダー
	std::unique_ptr<ColliderShape> collider_;

protected: //アニメーション関数

	/// <summary>
	/// アニメーション
	/// </summary>
	void Animation(uint32_t frameCount);

	/// <summary>
	/// アニメーションの登録
	/// </summary>
	//void RegistrationOfAnimation(const std::string& fileName);

public: //アニメーション関数

	/// <summary>
	/// animationTransforms_の変更
	/// </summary>
	void animationTransformChange(const std::string& key);

protected: // アニメーション変数

	// アニメーションのフレーム
	// アニメーションのトランスフォーム
	std::vector<BoneData> animationTransforms_;

	// マップ
	std::map<std::string, std::vector<BoneData>> animationTransformDatas_;

};

