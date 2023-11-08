#include "Player.h"
#include "Engine/Utility/GlobalVariables.h"
#include <cassert>

void Player::Initialize(const std::vector<Model*>& models) {
	//基底クラスの初期化
	BaseCharacter::Initialize(models);
	//入力クラスのインスタンスを取得
	input_ = Input::GetInstance();
	//武器の作成
	weapon_ = std::make_unique<Weapon>();
	weapon_->Initialize(models_[1]);
	weapon_->SetParent(&worldTransform_);
	//衝突属性を設定
	SetCollisionAttribute(kCollisionAttributePlayer);
	SetCollisionMask(kCollisionMaskPlayer);
	SetCollisionPrimitive(kCollisionPrimitiveAABB);

	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	//グループを追加
	globalVariables->CreateGroup(groupName);
	globalVariables->AddItem(groupName, "BehaviorDashTime", behaviorDashTime_);
}

void Player::Update() {
	//当たった瞬間に親子付けする
	if (preOnCollision_ == false && onCollision_ == true) {
		worldTransform_.SetParent(parent_);
	}

	//離れた瞬間に親子付けを外す
	if (preOnCollision_ == true && onCollision_ == false) {
		worldTransform_.UnsetParent();
	}


	//Behaviorの遷移処理
	if (behaviorRequest_) {
		//振る舞いを変更する
		behavior_ = behaviorRequest_.value();
		//各振る舞いごとの初期化を実行
		switch (behavior_) {
		case Behavior::kRoot:
		default:
			BehaviorRootInitialize();
			break;
		case Behavior::kDash:
			BehaviorDashInitialize();
			break;
		case Behavior::kAttack:
			BehaviorAttackInitialize();
			break;
		}
		behaviorRequest_ = std::nullopt;
	}


	//Behaviorの実行
	switch (behavior_) {
	case Behavior::kRoot:
	default:
		BehaviorRootUpdate();
		break;
	case Behavior::kDash:
		BehaviorDashUpdate();
		break;
	case Behavior::kAttack:
		BehaviorAttackUpdate();
		break;
	}


	//当たっていないときは落ちる
	if (onCollision_ == false) {
		worldTransform_.translation_.y -= 0.1f;
	}
	else {
		worldTransform_.translation_.y = 0.0f;
	}

	//落ちたらリスタート
	if (worldTransform_.translation_.y <= -5.0f) {
		Restart();
	}


	//ワールドトランスフォームの更新
	worldTransform_.UpdateMatrix();

	//武器の更新
	weapon_->Update();

	//次のフレーム用のフラグを保存
	preOnCollision_ = onCollision_;
	onCollision_ = false;

	//グローバル変数の適応
	Player::ApplyGlobalVariables();
	Matrix4x4 rotateXMatrix = MakeRotateXMatrix(worldTransform_.rotation_.x);
	Matrix4x4 rotateYMatrix = MakeRotateYMatrix(worldTransform_.rotation_.y);
	Matrix4x4 rotateZMatrix = MakeRotateZMatrix(worldTransform_.rotation_.z);
	Matrix4x4 rotateMatrix = Multiply(rotateXMatrix, Multiply(rotateYMatrix, rotateZMatrix));

	ImGui::Begin("Player");
	ImGui::Text("WorldTransform matRot_\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f",
		rotateMatrix.m[0][0], rotateMatrix.m[0][1], rotateMatrix.m[0][2],rotateMatrix.m[0][3],
		rotateMatrix.m[1][0], rotateMatrix.m[1][1], rotateMatrix.m[1][2],rotateMatrix.m[1][3],
		rotateMatrix.m[2][0], rotateMatrix.m[2][1], rotateMatrix.m[2][2],rotateMatrix.m[2][3],
		rotateMatrix.m[3][0], rotateMatrix.m[3][1], rotateMatrix.m[3][2],rotateMatrix.m[3][3]);
	ImGui::Text("DirectionToDirection matRot_\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f\n%f %f %f %f",
		matRot_.m[0][0], matRot_.m[0][1], matRot_.m[0][2], matRot_.m[0][3],
		matRot_.m[1][0], matRot_.m[1][1], matRot_.m[1][2], matRot_.m[1][3],
		matRot_.m[2][0], matRot_.m[2][1], matRot_.m[2][2], matRot_.m[2][3],
		matRot_.m[3][0], matRot_.m[3][1], matRot_.m[3][2], matRot_.m[3][3]);
	ImGui::End();
}

void Player::Draw(const ViewProjection& viewProjection) {
	//モデルの描画
	models_[0]->Draw(worldTransform_, viewProjection);
	if (behavior_ == Behavior::kAttack) {
		weapon_->Draw(viewProjection);
	}
}

void Player::Restart() {
	//初期座標に戻す
	worldTransform_.translation_ = { 0.0f,0.0f,0.0f };
	worldTransform_.parent_ = nullptr;
}

void Player::OnCollision(Collider* collider) {
	//床と当たっていた場合
	if (collider->GetCollisionAttribute() & kCollisionAttributeFloor) {
		//衝突フラグを立てる
		onCollision_ = true;
		//親を設定する
		parent_ = &collider->GetWorldTransform();
		//現在の親と別の親なら親子付けする
		if (worldTransform_.parent_ != parent_) {
			worldTransform_.UnsetParent();
			worldTransform_.SetParent(parent_);
		}
	}

	//敵と当たったらリスタート
	if (collider->GetCollisionAttribute() & kCollisionAttributeEnemy) {
		Restart();
	}

	//ゴールに触れたらリスタート
	if (collider->GetCollisionAttribute() & kCollisionAttributeGoal) {
		Restart();
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 pos{};
	pos.x = worldTransform_.matWorld_.m[3][0];
	pos.y = worldTransform_.matWorld_.m[3][1];
	pos.z = worldTransform_.matWorld_.m[3][2];
	return pos;
}

void Player::BehaviorRootInitialize() {

}

void Player::BehaviorRootUpdate() {
	//ダッシュのクールタイム
	const uint32_t behaviorDashCoolTime = 60;

	XINPUT_STATE joyState{};

	if (input_->GetJoystickState(joyState)) {

		//しきい値
		const float threshold = 0.7f;

		//移動フラグ
		bool isMoving = false;

		//移動量
		Vector3 move = {
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX,
			0.0f,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX,
		};

		//スティックの押し込みが遊び範囲を超えていたら移動フラグをtrueにする
		if (Length(move) > threshold) {
			isMoving = true;
		}

		if (isMoving) {

			//速さ
			const float speed = 0.3f;

			//移動量に速さを反映
			move = Multiply(Normalize(move), speed);

			//移動ベクトルをカメラの角度だけ回転する
			Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
			move = TransformNormal(move, rotateMatrix);

			//目標角度の算出
			destinationAngleY_ = std::atan2(move.x, move.z);
			matRot_ = DirectionToDirection(Normalize(worldTransform_.translation_), Normalize(move));

			//移動
			worldTransform_.translation_ = Add(worldTransform_.translation_, move);
		}
	}

	//移動方向に見た目を合わせる
	worldTransform_.rotation_.y = LerpShortAngle(worldTransform_.rotation_.y, destinationAngleY_, 0.2f);

	//攻撃行動に変更
	if (input_->GetJoystickState(joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) {
			if (workDash_.coolTime == behaviorDashCoolTime) {
				behaviorRequest_ = Behavior::kAttack;
			}
		}
	}

	//ダッシュ行動に変更
	if (workDash_.coolTime != behaviorDashCoolTime) {
		workDash_.coolTime++;
	}

	if (input_->GetJoystickState(joyState)) {
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_A) {
			if (workDash_.coolTime == behaviorDashCoolTime) {
				behaviorRequest_ = Behavior::kDash;
			}
		}
	}
}

void Player::BehaviorDashInitialize() {
	workDash_.dashParameter_ = 0;
	workDash_.coolTime = 0;
	worldTransform_.rotation_.y = destinationAngleY_;
}

void Player::BehaviorDashUpdate() {
	
	XINPUT_STATE joyState{};

	if (input_->GetJoystickState(joyState)) {
		//速さ
		float kSpeed = 1.0f;
		//移動量
		Vector3 move = {
			(float)joyState.Gamepad.sThumbLX / SHRT_MAX,
			0.0f,
			(float)joyState.Gamepad.sThumbLY / SHRT_MAX,
		};

		//移動量に速さを反映
		move = Multiply(Normalize(move), kSpeed);

		//移動ベクトルをカメラの角度だけ回転する
		Matrix4x4 rotateMatrix = MakeRotateYMatrix(viewProjection_->rotation_.y);
		move = TransformNormal(move, rotateMatrix);

		//移動
		worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	}

	//規定の時間経過で通常行動に戻る
	if (++workDash_.dashParameter_ >= behaviorDashTime_) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::BehaviorAttackInitialize() {
	//攻撃開始
	weapon_->Attack();
}

void Player::BehaviorAttackUpdate() {
	//攻撃が終わったら通常状態に戻す
	if (weapon_->GetIsAttack() == false) {
		behaviorRequest_ = Behavior::kRoot;
	}
}

void Player::ApplyGlobalVariables() {
	GlobalVariables* globalVariables = GlobalVariables::GetInstance();
	const char* groupName = "Player";
	behaviorDashTime_ = globalVariables->GetIntValue(groupName, "BehaviorDashTime");
}