#pragma once
#include "IScene.h"
#include "Engine/Components/Input.h"
#include "Engine/Components/Audio.h"
#include "Engine/Components/PostProcess.h"
#include "Engine/3D/Model/Model.h"
#include "Engine/2D/Sprite.h"
#include "Engine/3D/Matrix/ViewProjection.h"
#include "Engine/Utility/CollisionManager/CollisionManager.h"
#include "../GameObject/Character/Player.h"
#include "../GameObject/Character/Enemy.h"
#include "../GameObject/Skydome/Skydome.h"
#include "../GameObject/FollowCamera/FollowCamera.h"
#include "../GameObject/Floor/Floor.h"
#include "../GameObject/Goal/Goal.h"

class GameScene : public IScene {
public:
	//床の数
	static const int kFloorMax = 5;

	/// <summary>
	/// コンストラクタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(GameManager* gameManager) override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update(GameManager* gameManager) override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(GameManager* gameManager) override;

private:
	//Input
	Input* input_ = nullptr;
	//オーディオクラス
	Audio* audio_ = nullptr;
	//ポストプロセス
	PostProcess* postProcess_ = nullptr;
	//カメラ
	ViewProjection viewProjection_{};
	//プレイヤー
	std::unique_ptr<Model> playerModel_ = nullptr;
	std::unique_ptr<Player> player_ = nullptr;
	//敵
	std::unique_ptr<Model> modelEnemyBody_ = nullptr;
	std::unique_ptr<Model> modelEnemyL_arm_ = nullptr;
	std::unique_ptr<Model> modelEnemyR_arm_ = nullptr;
	std::unique_ptr<Model> weaponModel_ = nullptr;
	//敵キャラ
	std::unique_ptr<Enemy> enemy_ = nullptr;
	//天球
	std::unique_ptr<Model> skydomeModel_ = nullptr;
	std::unique_ptr<Skydome> skydome_ = nullptr;
	//追従カメラ
	std::unique_ptr<FollowCamera> followCamera_ = nullptr;
	//床
	std::unique_ptr<Model> floorModel_ = nullptr;
	std::vector<std::unique_ptr<Floor>> floors_{};
	//ゴール
	std::unique_ptr<Model> goalModel_ = nullptr;
	std::unique_ptr<Goal> goal_ = nullptr;
	//衝突マネージャー
	std::unique_ptr<CollisionManager> collisionManager_ = nullptr;

};