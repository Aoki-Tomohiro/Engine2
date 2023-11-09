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
	Quaternion rotation0 = MakeRotateAxisAngleQuaternion({ 0.71f,0.71f,0.0f }, 0.3f);
	Quaternion rotation1 = MakeRotateAxisAngleQuaternion({ 0.71f,0.0f,0.71f }, 3.141592f);

	Quaternion interpolate0 = Slerp(rotation0, rotation1, 0.0f);
	Quaternion interpolate1 = Slerp(rotation0, rotation1, 0.3f);
	Quaternion interpolate2 = Slerp(rotation0, rotation1, 0.5f);
	Quaternion interpolate3 = Slerp(rotation0, rotation1, 0.7f);
	Quaternion interpolate4 = Slerp(rotation0, rotation1, 1.0f);

	ImGui::Begin(" ");
	ImGui::Text("%1.2f %1.2f %1.2f %1.2f : interpolate0, Slerp(q0,q1,0.0f)",
		interpolate0.x, interpolate0.y, interpolate0.z, interpolate0.w);
	ImGui::Text("%1.2f %1.2f %1.2f %1.2f : interpolate1, Slerp(q0,q1,0.3f)",
		interpolate1.x, interpolate1.y, interpolate1.z, interpolate1.w);
	ImGui::Text("%1.2f %1.2f %1.2f %1.2f : interpolate2, Slerp(q0,q1,0.5f)",
		interpolate2.x, interpolate2.y, interpolate2.z, interpolate2.w);
	ImGui::Text("%1.2f %1.2f %1.2f %1.2f : interpolate3, Slerp(q0,q1,0.7f)",
		interpolate3.x, interpolate3.y, interpolate3.z, interpolate3.w);
	ImGui::Text("%1.2f %1.2f %1.2f %1.2f : interpolate4, Slerp(q0,q1,1.0f)",
		interpolate4.x, interpolate4.y, interpolate4.z, interpolate4.w);
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