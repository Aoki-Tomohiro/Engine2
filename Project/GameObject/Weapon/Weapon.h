#pragma once
#include "Engine/Components/Input.h"
#include "Engine/3D/Model/Model.h"
#include "Engine/3D/Matrix/WorldTransform.h"
#include "Engine/Utility/CollisionManager/Collider.h"
#include <array>

/// <summary>
/// 武器クラス
/// </summary>
class Weapon : public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// 親を設定
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* parent) { worldTransform_.parent_ = parent; };

	/// <summary>
	/// 攻撃フラグ開始
	/// </summary>
	/// <param name="flag"></param>
	void SetIsAttack();

	/// <summary>
	/// 攻撃フラグを取得
	/// </summary>
	/// <returns></returns>
	bool GetIsAttack() { return isAttack_; };

	/// <summary>
	/// 攻撃フラグを取得
	/// </summary>
	bool GetIsHit() { return isHit_; };

	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	/// <returns></returns>
	WorldTransform& GetWorldTransform() override { return worldTransform_; };

	/// <summary>
	/// 衝突判定
	/// </summary>
	/// <param name="collider"></param>
	void OnCollision(Collider* collider) override;

	/// <summary>
	/// ワールドポジションを取得
	/// </summary>
	/// <returns></returns>
	Vector3 GetWorldPosition() override;

private:
	/// <summary>
	/// 攻撃アニメーションの初期化
	/// </summary>
	void AttackInitialize();

private:
	Input* input_ = nullptr;
	//モデル
	Model* model_ = nullptr;
	//ワールドトランスフォーム
	WorldTransform worldTransform_{};
	WorldTransform worldTransformCollision_{};
	//攻撃フラグ
	bool isAttack_ = false;
	//当たり判定のフラグ
	bool isHit_ = false;
	//アニメーションタイマー
	uint16_t animationTimer_ = 0;
	uint16_t animationCount_ = 0;
};

