#include "GameScene.h"
#include "Engine/Base/TextureManager.h"
#include <cassert>

GameScene::GameScene() {};

GameScene::~GameScene() {};

void GameScene::Initialize(GameManager* gameManager) {
	//Inputのインスタンスを取得
	input_ = Input::GetInstance();
	//オーディオクラスのインスタンスを取得
	audio_ = Audio::GetInstance();
	//ポストプロセスのインスタンスを取得
	postProcess_ = PostProcess::GetInstance();
};

void GameScene::Update(GameManager* gameManager) {
	Quaternion q1 = { 2.0f,3.0f,4.0f,1.0f };
	Quaternion q2 = { 1.0f,3.0f,5.0f,2.0f };
	Quaternion identity = IdentityQuaternion();
	Quaternion conj = Conjugate(q1);
	Quaternion inv = Inverse(q1);
	Quaternion normal = Normalize(q1);
	Quaternion mul1 = Multiply(q1, q2);
	Quaternion mul2 = Multiply(q2, q1);
	float norm = Norm(q1);

	ImGui::Begin(" ");
	ImGui::Text("%1.2f %1.2f %1.2f %1.2f     : Identity",
		identity.x, identity.y, identity.z, identity.w);
	ImGui::Text("%1.2f %1.2f %1.2f %1.2f  : Conjugate",
		conj.x, conj.y, conj.z, conj.w);
	ImGui::Text("%1.2f %1.2f %1.2f %1.2f  : Inverse",
		inv.x, inv.y, inv.z, inv.w);
	ImGui::Text("%1.2f %1.2f %1.2f %1.2f     : Normalize",
		normal.x, normal.y, normal.z, normal.w);
	ImGui::Text("%1.2f %1.2f %1.2f %1.2f  : Multiply(q1,q2)",
		mul1.x, mul1.y, mul1.z, mul1.w);
	ImGui::Text("%1.2f %1.2f %1.2f %1.2f : Multiply(q2,q1)",
		mul2.x, mul2.y, mul2.z, mul2.w);
	ImGui::Text("%1.2f                    : Norm",norm);
	ImGui::End();
};

void GameScene::Draw(GameManager* gameManager) {

#pragma region 背景スプライトの描画

	Sprite::PreDraw(Sprite::kBlendModeNormal);

	Sprite::PostDraw();

#pragma endregion

	//深度バッファのクリア
	DirectXCommon::GetInstance()->ClearDepthBuffer();

#pragma region モデルの描画

	Model::PreDraw();

	Model::PostDraw();

#pragma endregion

#pragma region スプライトの描画

	Sprite::PreDraw(Sprite::kBlendModeNormal);

	Sprite::PostDraw();

#pragma endregion
};