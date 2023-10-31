#pragma once
#include "Engine/Utility/MathFunction.h"
#include <d3d12.h>
#include <wrl.h>

struct ConstBuffDataWorldTransform {
	Matrix4x4 world;
};

/// <summary>
/// ワールドトランスフォーム
/// </summary>
struct WorldTransform {

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// ワールド行列を転送
	/// </summary>
	void TransferMatrix();

	/// <summary>
	/// 行列の計算・転送
	/// </summary>
	void UpdateMatrix();

	/// <summary>
	/// 親を設定
	/// </summary>
	/// <param name="parent"></param>
	void SetParent(const WorldTransform* parent);

	/// <summary>
	/// 親子付けを解除する
	/// </summary>
	void UnsetParent();

	//CBV
	Microsoft::WRL::ComPtr<ID3D12Resource> constBuff_ = nullptr;
	//スケール
	Vector3 scale_ = { 1.0f,1.0f,1.0f };
	//角度
	Vector3 rotation_ = { 0.0f,0.0f,0.0f };
	//座標
	Vector3 translation_ = { 0.0f,0.0f,0.0f };
	//ワールド行列
	Matrix4x4 matWorld_{};
	//親となるワールド変換へのポインタ
	const WorldTransform* parent_ = nullptr;

};