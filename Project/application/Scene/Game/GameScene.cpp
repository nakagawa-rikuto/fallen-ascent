#include "GameScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"
// Service
#include "Engine/System/Service/CameraService.h"
#include "Engine/System/Service/ParticleService.h"
#include "Engine/System/Service/ColliderService.h"
#include "Engine/System/Service/GraphicsResourceGetter.h"
// State
#include "State/GameSceneInitializeState.h"
// Math
#include "Math/SMath.h"

///-------------------------------------------/// 
/// コンストラクタ
///-------------------------------------------///
GameScene::GameScene() {
	/// ===パーティクルの読み込み=== ///
	LoadParticle();
}

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() {
	// ISceneのデストラクタ
	IScene::~IScene();
	// Colliderのリセット
	ColliderService::Reset();
	// State
	currentState_.reset();
	// Camera
	camera_.reset();
	// Player
	player_.reset();
	// Enemy
	enemyManager_.reset();
	// Ground
	stage_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameScene::Initialize() {
	/// ===ISceneの初期化=== ///
	IScene::Initialize();

	/// ===Camera=== ///
	camera_ = std::make_shared<GameCamera>();
	camera_->Init(CameraType::Follow);
	SetUpCamera();
	// Managerに追加,アクティブに
	CameraService::AddCamera("Game", camera_);
	CameraService::SetActiveCamera("Game");

	/// ===Playerの生成=== ///
	player_ = std::make_unique<Player>();

	/// ===EnemyManagerの生成=== ///
	enemyManager_ = std::make_unique<EnemyManager>();

	/// ===SponEntity=== ///
	SpawnEntity("Level/EntityData.json");
	// EnemyにPlayerを設定
	enemyManager_->SetPlayer(player_.get());

	/// ===GameStage=== ///
	stage_ = std::make_unique<GameStage>();
	stage_->Initialize("Level/StageData.json");

	/// ===State=== ///
	// 初期状態をInitializeStateに設定
	ChangState(std::make_unique<GameSceneInitializeState>());
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GameScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("GameScene");
	ImGui::End();

	// Camera
	camera_->ImGuiUpdate();
	camera_->DebugUpdate();

	// Player
	player_->Information();
	// Enemy
	enemyManager_->UpdateImGui();

#endif // USE_IMGUI

	/// ===Groundの更新=== ///
	stage_->Update();

	/// ===Stateの管理=== ///
	if (currentState_) {
		// 各Stateの更新
		currentState_->Update();
	}

	/// ===ISceneの更新=== ///
	IScene::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
	/// ===GameStage=== ///
	stage_->Draw();

	/// ===Enemy=== ///
	enemyManager_->Draw();

	/// ===Player=== ///
	player_->Draw();

	/// ===Stateの管理=== ///
	if (currentState_) {
		// 各Stateの更新
		currentState_->Draw();
	}

	/// ===ISceneの描画=== ///
	IScene::Draw();
}

///-------------------------------------------/// 
/// Stateの変更処理
///-------------------------------------------///
void GameScene::ChangState(std::unique_ptr<GameSceneFadeState> newState) {
	if (currentState_) {
		// 古い状態を解放  
		currentState_->Finalize();
		currentState_.reset();
	}

	// 新しい状態をセット  
	currentState_ = std::move(newState);
	// SceneManagerの設定
	currentState_->SetSceneManager(sceneManager_);
	// 新しい状態の初期化  
	currentState_->Enter(this);
}

///-------------------------------------------/// 
/// 配置関数
///-------------------------------------------///
void GameScene::SpawnEntity(const std::string& json_name) {
	LevelData* levelData = GraphicsResourceGetter::GetLevelData(json_name);

	// オブジェクト分回す
	for (const auto& obj : levelData->objects) {
		// OBBの半分の大きさを計算
		Vector3 obbHalfSize = obj.colliderInfo2 / 2.0f;

		/// ===クラス名で分岐=== ///
		switch (obj.classType) {
		case LevelData::ClassTypeLevel::Player1:
			// 初期化と座標設定
			player_->Initialize();
			player_->SetTranslate(obj.translation);
			player_->SetRotate(Math::QuaternionFromVector(obj.rotation));
			player_->SetHalfSize(obbHalfSize);
			break;
		case LevelData::ClassTypeLevel::Enemy1:
			// Enemyの座標設定
			enemyManager_->Spawn(EnemyType::LongRange, obj.translation, Math::QuaternionFromVector(obj.rotation), obbHalfSize);
			break;
		case LevelData::ClassTypeLevel::Enemy2:
			// Enemyの座標設定
			enemyManager_->Spawn(EnemyType::CloseRange, obj.translation, Math::QuaternionFromVector(obj.rotation), obbHalfSize);
			break;
		}
	}
}

///-------------------------------------------/// 
/// カメラの初期設定
///-------------------------------------------///
void GameScene::SetUpCamera() {
	/// ===カメラの初期設定=== ///
	// カメラの座標設定
	camera_->SetTranslate({ 0.0f, 5.0f, 15.0f });
	// カメラの回転設定
	camera_->SetRotate({ 0.0f, 0.0f, 0.0f, 1.0f });
	// カメラの追従設定
	camera_->SetFollowCamera(FollowCameraType::TopDown);
	// 追従オフセット設定
	camera_->SetOffset({ 0.0f, 70.0f, -60.0f });
	// 追従速度設定
	camera_->SetFollowSpeed(0.1f);
}

///-------------------------------------------/// 
/// パーティクルの読み込み
///-------------------------------------------///
void GameScene::LoadParticle() {
	ParticleService::LoadParticleDefinition("Game.json");
	ParticleService::LoadParticleDefinition("WeaponAttack.json");
	ParticleService::LoadParticleDefinition("nakagawa.json");
	ParticleService::LoadParticleDefinition("PlayerWarke.json");
	ParticleService::LoadParticleDefinition("EnemyAttack.json");
	ParticleService::LoadParticleDefinition("EnemyPrePareAttack.json");
	ParticleService::LoadParticleDefinition("EnemyPrePareAttackCharge.json");
	ParticleService::LoadParticleDefinition("CloseEnemyAttack.json");
	ParticleService::LoadParticleDefinition("LongEnemyAttack.json");
}
