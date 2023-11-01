#pragma once
#include "Engine/3D/Model/Model.h"
#include "Engine/3D/Matrix/WorldTransform.h"
#include <vector>

class BaseCharacter {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="models"></param>
	virtual void Initialize(const std::vector<Model*>& models);

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="viewProjection"></param>
	virtual void Draw(const ViewProjection& viewProjection);

	/// <summary>
	/// ワールド変換データを取得
	/// </summary>
	/// <returns></returns>
	const WorldTransform& GetWorldTransform() { return worldTransform_; };

protected:
	//モデルデータ配列
	std::vector<Model*> models_;
	//ワールド変換データ
	WorldTransform worldTransform_;
};