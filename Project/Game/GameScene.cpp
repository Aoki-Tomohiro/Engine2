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
	//プレイヤーの作成
	playerModel_.reset(Model::CreateFromOBJ("Resources/Player", "Player.obj"));
	weaponModel_.reset(Model::CreateFromOBJ("Resources/Weapon", "Weapon.obj"));
	std::vector<Model*> playerModels = { playerModel_.get(),weaponModel_.get() };
	player_ = std::make_unique<Player>();
	player_->Initialize(playerModels);
	player_->SetViewProjection(&viewProjection_);

	//敵の作成
	modelEnemyBody_.reset(Model::CreateFromOBJ("Resources/Enemy_Body", "Enemy_Body.obj"));
	modelEnemyL_arm_.reset(Model::CreateFromOBJ("Resources/Enemy_L_arm", "Enemy_L_arm.obj"));
	modelEnemyR_arm_.reset(Model::CreateFromOBJ("Resources/Enemy_R_arm", "Enemy_R_arm.obj"));
	std::vector<Model*> enemyModels = { modelEnemyBody_.get(),modelEnemyL_arm_.get(),modelEnemyR_arm_.get() };
	enemy_ = std::make_unique <Enemy>();
	enemy_->Initialize(enemyModels);

	//天球の作成
	skydomeModel_.reset(Model::CreateFromOBJ("Resources/Skydome", "Skydome.obj"));
	skydomeModel_->GetDirectionalLight()->SetEnableLighting(false);
	skydome_ = std::make_unique<Skydome>();
	skydome_->Initialize(skydomeModel_.get());

	//追従カメラの作成
	followCamera_ = std::make_unique<FollowCamera>();
	followCamera_->Initialize();
	followCamera_->SetTarget(&player_->GetWorldTransform());

	//床の作成
	floorModel_.reset(Model::CreateFromOBJ("Resources/Floor", "Floor.obj"));
	for (uint32_t i = 0; i < kFloorMax; ++i) {
		Floor* floor = new Floor();
		if (i % 2 == 1) {
			floor->Initialize(floorModel_.get(), { 0.0f,0.0f,i * 20.0f }, { i * 0.1f,0.0f,0.0f });
		}
		else {
			floor->Initialize(floorModel_.get(), { 0.0f,0.0f,i * 20.0f }, { 0.0f,0.0f,0.0f });
		}
		floors_.push_back(std::unique_ptr<Floor>(floor));
	}
	//敵の親を設定
	enemy_->SetParent(&floors_[2]->GetWorldTransform());

	//ゴールの作成
	goalModel_.reset(Model::CreateFromOBJ("Resources/Goal", "Goal.obj"));
	goal_ = std::make_unique<Goal>();
	goal_->Initialize(goalModel_.get(), { 0.0f,0.0f,4 * 20.0f });

	//衝突マネージャーの作成
	collisionManager_ = std::make_unique<CollisionManager>();
};

void GameScene::Update(GameManager* gameManager) {
	//プレイヤーの更新
	player_->Update();

	//敵の更新
	enemy_->Update();

	//天球の更新
	skydome_->Update();

	//床の更新
	for (std::unique_ptr<Floor>& floor : floors_) {
		floor->Update();
	}

	//ゴールの更新
	goal_->Update();

	//追従カメラの更新
	followCamera_->Update();
	viewProjection_ = followCamera_->GetViewProjection();

	//カメラの更新
	viewProjection_.UpdateMatrix();

	//衝突判定
	collisionManager_->ClearColliderList();
	collisionManager_->SetColliderList(player_.get());
	if (player_->GetWeapon()->GetIsHit()) {
		collisionManager_->SetColliderList(player_->GetWeapon());
	}
	if (enemy_->GetIsDead() == false) {
		collisionManager_->SetColliderList(enemy_.get());
	}
	for (std::unique_ptr<Floor>& floor : floors_) {
		collisionManager_->SetColliderList(floor.get());
	}
	collisionManager_->SetColliderList(goal_.get());
	collisionManager_->CheckAllCollisions();
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

	//プレイヤーの描画
	player_->Draw(viewProjection_);

	//敵の描画
	if (enemy_->GetIsDead() == false) {
		enemy_->Draw(viewProjection_);
	}

	//天球の描画
	skydome_->Draw(viewProjection_);

	//床の描画
	for (std::unique_ptr<Floor>& floor : floors_) {
		floor->Draw(viewProjection_);
	}

	//ゴールの描画
	goal_->Draw(viewProjection_);

	Model::PostDraw();

#pragma endregion

#pragma region スプライトの描画

	Sprite::PreDraw(Sprite::kBlendModeNormal);

	Sprite::PostDraw();

#pragma endregion
};