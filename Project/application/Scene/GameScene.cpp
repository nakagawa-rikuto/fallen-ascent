#include "GameScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"
// Service
#include "Engine/System/Service/CameraService.h"
#include "Engine/System/Service/ParticleService.h"
#include "Engine/System/Service/ColliderService.h"
#include "Engine/System/Service/InputService.h"
#include "Engine/System/Service/GraphicsResourceGetter.h"
#include "Engine/System/Service/OffScreenService.h"
#include "Engine/System/Service/Loader.h"
//// Particle
//#include "Engine/Graphics/Particle/Derivative/ConfettiParticle.h"
//#include "Engine/Graphics/Particle/Derivative/ExplosionParticle.h"
//#include "Engine/Graphics/Particle/Derivative/WindParticle.h"
//#include "Engine/Graphics/Particle/Derivative/HitEffectParticle.h"
//#include "Engine/Graphics/Particle/Derivative/RingParticle.h"
//#include "Engine/Graphics/Particle/Derivative/CylinderParticle.h"
//#include "application/Game/Particle/DustCloudParticle.h"
//// GameParticle
//#include "application/Game/Particle/AttackTrajectoryParticle.h"
// Math
#include "Math/SMath.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameScene::~GameScene() {
	// ISceneのデストラクタ
	IScene::~IScene();
	// Colliderのリセット
	ColliderService::Reset();
	// StartAnimation
	startAnimation_.reset();
	// Camera
	camera_.reset();
	// Player
	player_.reset();
	// Enemy
	enemyManager_.reset();
	// Ground
	stage_.reset();
	// Transitionのリセット
	transiton_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	// Jsonの読み込み
	Loader::LoadLevelJson("StageData.json");
	Loader::LoadLevelJson("EntityData.json");

	/// ===Transition=== ///
	transiton_ = std::make_unique<SceneTransition>();
	fadeInDuration_ = 2.0f;
	transiton_->StartFadeOut(fadeInDuration_); // フェードイン開始

	/// ===Camera=== ///
	camera_ = std::make_shared<GameCamera>();
	camera_->Init(CameraType::Follow);
	camera_->SetRotate(cameraInfo_.rotate);
	camera_->SetFollowCamera(FollowCameraType::TopDown);
	camera_->SetOffset({ 0.0f, 70.0f, -60.0f });
	camera_->SetFollowSpeed(0.1f);
	// Managerに追加,アクティブに
	CameraService::AddCamera("Game", camera_);
	CameraService::SetActiveCamera("Game");

	/// ===Playerの生成=== ///
	player_ = std::make_unique<Player>();

	/// ===EnemyManagerの生成=== ///
	enemyManager_ = std::make_unique<EnemyManager>();

	/// ===SponEntity=== ///
	SpawnEntity("EntityData.json");
	// EnemyにPlayerを設定
	enemyManager_->SetPlayer(player_.get());

	/// ===GameStage=== ///
	stage_ = std::make_unique<GameStage>();
	stage_->Initialize("StageData.json");

	/// ===StartAnimation=== ///
	startAnimation_ = std::make_unique<StartAnimation>();
	startAnimation_->Initialize(player_.get(), camera_.get());

	/// ===GameOverAnimation=== ///
	gameOverAnimation_ = std::make_unique<GameOverAnimation>();

	/// ===GameClearAnimation=== ///
	gameClearAnimation_ = std::make_unique<GameClearAnimation>();

	// 初期フェーズをFadeInに設定
	currentPhase_ = GamePhase::FadeIn;
	transiton_->StartFadeOut(fadeInDuration_); // フェードイン開始
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
	// Oshan
	//groundOshan_->ShowImGui();

	// カメラ切り替え（デバッグ）
	if (InputService::TriggerKey(DIK_O)) {
		CameraService::SetActiveCamera("Game");
	} else if (InputService::TriggerKey(DIK_P)) {
		CameraService::SetActiveCamera("Default");
	}

#endif // USE_IMGUI

	/// ===Groundの更新=== ///
	stage_->Update();

	/// ===フェーズ別更新=== ///
	switch (currentPhase_) {
	// フェードイン
	case GamePhase::FadeIn:
		UpdateFadeIn();
		break;
	// 開始アニメーション
	case GamePhase::StartAnimation:
		UpdateStartAnimation();
		break;
	// ゲームプレイ
	case GamePhase::Game:
		UpdateGame();
		break;
	case GamePhase::GameOverAnimation:
		UpdateGameOverAnimation();
		break;
	// フェードアウト
	case GamePhase::GameClearAnimation:
		UpdateGameClearAnimtaion();
		break;
	}

	/// ===ISceneの更新=== ///
	IScene::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameScene::Draw() {
#pragma region 背景スプライト描画
#pragma endregion

#pragma region モデル描画

	/// ===GameStage=== ///
	stage_->Draw();

	/// ===Enemy=== ///
	enemyManager_->Draw();

	/// ===Player=== ///
	player_->Draw();

	/// ===StartAnimation=== ///
	if (currentPhase_ == GamePhase::StartAnimation) {
		startAnimation_->Draw();
	/// ===GameOverAnimation=== ///
	} else if (currentPhase_ == GamePhase::GameOverAnimation) {
		gameOverAnimation_->Draw();
	}

	/// ===ISceneの描画=== ///
	IScene::Draw();
#pragma endregion

#pragma region 前景スプライト描画
#pragma endregion
}

///-------------------------------------------/// 
/// フェードイン時の更新処理
///-------------------------------------------///
void GameScene::UpdateFadeIn() {

	// FadeIn更新
	transiton_->FadeOutUpdate();

	// アニメーション時のEnemy更新
	enemyManager_->UpdateAnimation();

	// アニメーション時のPlayer更新
	player_->UpdateAnimation();

	// FadeIn完了でStartAnimationフェーズへ
	if (transiton_->GetState() == FadeState::Finished) {
		OffScreenService::SetOffScreenType(OffScreenType::CopyImage);
		currentPhase_ = GamePhase::StartAnimation;
	}
}

///-------------------------------------------/// 
/// 開始アニメーション時の更新処理
///-------------------------------------------///
void GameScene::UpdateStartAnimation() {
	// アニメーション更新
	startAnimation_->Update();

	// アニメーション時のEnemy更新
	enemyManager_->UpdateAnimation();

	// アニメーション時のPlayer更新
	player_->UpdateAnimation();

	// アニメーション完了でGameフェーズへ
	if (startAnimation_->IsCompleted()) {
		// カメラターゲットをPlayerに設定
		player_->SetCameraTargetPlayer(); // ぎこちなさが残る
		currentPhase_ = GamePhase::Game;
	}
}

///-------------------------------------------/// 
/// ゲーム時の更新処理
///-------------------------------------------///
void GameScene::UpdateGame() {
	/// ===Playerの更新=== ///
	player_->Update();

	/// ===Enemy=== ///
	enemyManager_->Update();
	enemyManager_->SetPlayer(player_.get()); // Playerを設定

	// Playerが死んだら
	if (player_->GetIsDead() || InputService::TriggerKey(DIK_Q)) {
		gameOverAnimation_->Initialize(camera_.get());
		currentPhase_ = GamePhase::GameOverAnimation; // GameOverAnimationへ

	// ゲームが終わったらFadeOutへ
	} else if (enemyManager_->GetTotalEnemyCount() <= 0 || InputService::TriggerKey(DIK_SPACE)) {
		gameClearAnimation_->Initialize(player_.get(), camera_.get());
		currentPhase_ = GamePhase::GameClearAnimation; // GameClearへ
	}
}

///-------------------------------------------/// 
/// GameOverアニメーション時の更新処理
///-------------------------------------------///
void GameScene::UpdateGameOverAnimation() {
	// アニメーション更新
	gameOverAnimation_->Update();

	// アニメーション時のPlayer更新
	player_->UpdateAnimation();
	// アニメーション時のEnemy更新
	enemyManager_->UpdateAnimation();

	// アニメーション完了でシーン移動
	if (gameOverAnimation_->IsCompleted()) {
		sceneManager_->ChangeScene(SceneType::Title);
	}
}

///-------------------------------------------/// 
/// フェードアウト時の更新処理
///-------------------------------------------///
void GameScene::UpdateGameClearAnimtaion() {

	// アニメーション更新
	gameClearAnimation_->Update();
	// アニメーション時のPlayer更新
	player_->UpdateAnimation();
	// アニメーション時のEnemy更新
	enemyManager_->UpdateAnimation();
	
	// アニメーション完了でシーン移動
	if (gameClearAnimation_->IsCompleted()) {
		sceneManager_->ChangeScene(SceneType::Title);
	}
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
			player_->SethalfSize(obbHalfSize);
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