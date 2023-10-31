#include "ViewProjection.h"
#include "Engine/Base/DirectXCommon.h"

void ViewProjection::Initialize() {
	//CBVの作成
	constBuff_ = DirectXCommon::GetInstance()->CreateBufferResource(sizeof(ConstBuffDataViewProjection));
}


void ViewProjection::TransferMatrix() {
	//Resourceに書き込む
	ConstBuffDataViewProjection* viewProjectionData = nullptr;
	constBuff_->Map(0, nullptr, reinterpret_cast<void**>(&viewProjectionData));
	viewProjectionData->view = matView_;
	viewProjectionData->projection = matProjection_;
	constBuff_->Unmap(0, nullptr);
}


void ViewProjection::UpdateViewMatrix() {
	////カメラのワールド行列を作成
	//Matrix4x4 worldMatrix = MakeAffineMatrix({ 1.0f,1.0f,1.0f }, rotation_, translation_);
	//matView_ = Inverse(worldMatrix);

	//平行移動行列の計算
	Matrix4x4 translateMatrix = MakeTranslateMatrix(translation_);
	//回転行列の計算
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(rotation_.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(rotation_.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(rotation_.z);
	Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));
	//ビュー行列の計算
	matView_ = Multiply(Inverse(translateMatrix), Inverse(rotateMatrix));
}


void ViewProjection::UpdateProjectionMatrix() {
	//プロジェクション行列の計算
	matProjection_ = MakePerspectiveFovMatrix(fov_, aspectRatio_, nearClip_, farClip_);
}


void ViewProjection::UpdateMatrix() {
	//ビュー行列の計算
	ViewProjection::UpdateViewMatrix();
	//プロジェクション行列の計算
	ViewProjection::UpdateProjectionMatrix();
	//ビュープロジェクションを転送する
	ViewProjection::TransferMatrix();
}