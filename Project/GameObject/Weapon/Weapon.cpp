#include "Weapon.h"
#include "Engine/2D/ImGuiManager.h"
#include <cassert>

void Weapon::Initialize(Model* model) {
	//モデルの初期化
	assert(model);
	model_ = model;
	//ワールドトランスフォームの初期化
	worldTransform_.Initialize();
	worldTransform_.translation_.y = 0.8f;
	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributeWeapon);
	SetCollisionMask(kCollisionMaskWeapon);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);
	AABB aabbSize = { {-10.0f,-10.0f,-10.0f},{10.0f,10.0f,10.0f} };
	SetAABB(aabbSize);
}

void Weapon::Update() {

	//攻撃中の時アニメーションタイマーを進める
	if (isAttack_) {
		animationTimer_++;
	}

	//振りかぶりアニメーション
	if (animationCount_ == 0) {
		if (animationTimer_ == 30) {
			animationCount_++;
			animationTimer_ = 0;
		}
		worldTransform_.rotation_.x -= 0.1f;
	}

	//攻撃アニメーション
	if (animationCount_ == 1) {
		if (animationTimer_ == 15) {
			animationCount_++;
			animationTimer_ = 0;
		}
		worldTransform_.rotation_.x += 0.2f;
	}

	//硬直アニメーション
	if (animationCount_ == 2) {
		if (animationTimer_ == 30) {
			animationCount_++;
			animationTimer_ = 0;
			isAttack_ = false;
		}
	}

	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();
}

void Weapon::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	model_->Draw(worldTransform_, viewProjection);
}

void Weapon::OnCollision(Collider* collider) {
	ImGui::Begin("Weapon");
	ImGui::Text("Hit");
	ImGui::End();
}

Vector3 Weapon::GetWorldPosition() {
	Vector3 pos{};
	pos.x = worldTransform_.matWorld_.m[3][0];
	pos.y = worldTransform_.matWorld_.m[3][1];
	pos.z = worldTransform_.matWorld_.m[3][2];
	return pos;
}

void Weapon::Attack(){
	if (isAttack_ == false) {
		Weapon::AttackInitialize();
		isAttack_ = true;
	}
}

void Weapon::AttackInitialize() {
	animationTimer_ = 0;
	animationCount_ = 0;
	worldTransform_.rotation_.x = 1.5f;
}