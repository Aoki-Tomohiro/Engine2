#pragma once
#include "BaseCharacter.h"
#include "Engine/Components/Input.h"
#include "Engine/Utility/MathFunction.h"
#include "Engine/Utility/CollisionManager/Collider.h"

/// <summary>
/// プレイヤー
/// </summary>
class Player : public BaseCharacter,public Collider {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model"></param>
	void Initialize(const std::vector<Model*>& models) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	void Draw(const ViewProjection& viewProjection) override;

	/// <summary>
	/// リスタート処理
	/// </summary>
	void Restart();

	/// <summary>
	/// ビュープロジェクションを設定
	/// </summary>
	/// <param name="viewProjection"></param>
	void SetViewProjection(const ViewProjection* viewProjection) { viewProjection_ = viewProjection; };

	/// <summary>
	/// 親を設定
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* parent) { parent_ = parent; };

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
	//ビュープロジェクション
	const ViewProjection* viewProjection_ = nullptr;
	//入力クラス
	Input* input_ = nullptr;
	//親
	const WorldTransform* parent_ = nullptr;
	//当たり判定のフラグ
	bool onCollision_ = false;
	bool preOnCollision_ = false;
};
