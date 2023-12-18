#pragma once
#include "../3D/WorldTransform.h"
#include "../Camera/BaseCamera.h"
#include "../3D/Model.h"
#include "../3D/Material.h"
#include <memory>
#include <map>
#include "BoneData.h"
#include "AnimationFile.h"

/// <summary>
/// ボーン
/// </summary>
class IBone
{

private:

	// アニメーションファイル
	static AnimationFile* animationFile_;

public: // メンバ関数

	/// <summary>
	///  デストラクタ
	/// </summary>
	~IBone() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	virtual void Initialize(Model* model, const std::string& objectName, const std::string& partName, const std::vector<std::string>& motionNames);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(uint32_t frameCount);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update(uint32_t frameCount, const std::vector<std::string>& motionNames);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(BaseCamera& camera);

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(BaseCamera& camera, Material* material);

public:

	void SetWorldTransform(WorldTransform worldTransform) { worldTransform_ = worldTransform; }

	WorldTransform GetWorldTransform() { return worldTransform_; }

protected: // 

	// モデル
	Model* model_;

	// ワールドトランスフォーム
	WorldTransform worldTransform_;

protected: //アニメーション関数

	/// <summary>
	/// アニメーション
	/// </summary>
	void Animation(uint32_t frameCount);

	// AnimationFile登録
	virtual void PreRegistrationAnimationFile();

	// AnimationFile登録
	virtual void RegistrationAnimationFile();

	// AnimationFile適用
	virtual void ApplyAnimationFile(const std::vector<std::string>& motionNames);

public: //アニメーション関数

	/// <summary>
	/// animationTransforms_の変更
	/// </summary>
	void animationTransformChange(const std::string& key);

protected: // アニメーション変数

	// ボーンのアニメーションデータ
	std::vector<BoneData> animationTransforms_;

	// マップ
	std::map<std::string, std::vector<BoneData>> animationTransformDatas_;

	// オブジェクト名
	std::string objectName_;

	// パーツ名
	std::string partName_;;

};

