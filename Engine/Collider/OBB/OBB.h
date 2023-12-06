#pragma once
#include "../Collider.h"
class OBB : public Collider
{

public: // メンバ関数

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& center,
		const Vector3& otientatuons1, const Vector3& otientatuons2, const Vector3& otientatuons3,
		const Vector3& size, void* parentObject);

	void SetOtientatuons(const Vector3& otientatuons1, const Vector3& otientatuons2, const Vector3& otientatuons3);

	/// <summary>
	/// ワールドトランスフォーム更新
	/// </summary>
	void worldTransformUpdate() override;

public: // メンバ変数

	Vector3 center_;//中心
	Vector3 otientatuons_[3];//座標軸
	Vector3 size_;//座標軸方向の長さの半分

};

