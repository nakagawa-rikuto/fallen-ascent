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
// Particle
#include "Engine/Graphics/Particle/Derivative/ConfettiParticle.h"
#include "Engine/Graphics/Particle/Derivative/ExplosionParticle.h"
#include "Engine/Graphics/Particle/Derivative/WindParticle.h"
#include "Engine/Graphics/Particle/Derivative/HitEffectParticle.h"
#include "Engine/Graphics/Particle/Derivative/RingParticle.h"
#include "Engine/Graphics/Particle/Derivative/CylinderParticle.h"
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
	// Ground
	ground_.reset();
	// Enemy
	enemyManager_.reset();
	// Transitionのリセット
	transiton_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	// Particleの追加
	ParticleService::AddParticle("Confetti", std::make_unique<ConfettiParticle>());
	ParticleService::AddParticle("Explosion", std::make_unique<ExplosionParticle>());
	ParticleService::AddParticle("Wind", std::make_unique<WindParticle>());
	ParticleService::AddParticle("Ring", std::make_unique<RingParticle>());
	ParticleService::AddParticle("HitEffect", std::make_unique<HitEffectParticle>());
	ParticleService::AddParticle("Cylinder", std::make_unique<CylinderParticle>());

	/// ===Line=== ///
	line_ = std::make_unique<Line>();

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

	/// ===Player=== ///
	player_ = std::make_unique<Player>();
	player_->Initialize();

	/// ===Enemy=== ///
	enemyManager_ = std::make_unique<EnemyManager>();
	enemyManager_->Spawn(EnemyType::CloseRange, { -30.0f, 1.0f, 30.0f });
	enemyManager_->Spawn(EnemyType::CloseRange, { 30.0f, 1.0f, 30.0f });
	enemyManager_->Spawn(EnemyType::CloseRange, { 35.0f, 1.0f, 35.0f });
	enemyManager_->Spawn(EnemyType::CloseRange, {- 30.0f, 1.0f, -30.0f });
	enemyManager_->Spawn(EnemyType::LongRange, { -35.0f, 1.0f, 35.0f });
	// EnemyのSpaw後に呼ぶ
	enemyManager_->SetPlayer(player_.get()); // Playerを設定

	/// ===Ground=== ///
	ground_ = std::make_unique<Ground>();
	ground_->Initialize();
	ground_->Update();

	/// ===GroundOshan=== ///
	groundOshan_ = std::make_unique<GroundOshan>();
	groundOshan_->Initialize();
	groundOshan_->Update();

	/// ===StartAnimation=== ///
	startAnimation_ = std::make_unique<StartAnimation>();
	startAnimation_->Initialize(player_.get(), camera_.get());

	/// ===GameOverAnimation=== ///
	gameOverAnimation_ = std::make_unique<GameOverAnimation>();
	gameOverAnimation_->Initialize(camera_.get());

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
	ground_->Update();
	groundOshan_->Update();

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
	case GamePhase::FadeOut:
		UpdateFadeOut();
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

	line_->DrawGrid({ 0.0f, 1.0f,0.0f }, { 100.0f, 1.0f, 100.0f }, 50, { 1.0f, 1.0f, 1.0f, 1.0f });

	/// ===Ground=== ///
	//ground_->Draw();
	//groundOshan_->Draw();

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

	// ゲームが終わったらFadeOutへ

	if (InputService::TriggerKey(DIK_SPACE)) {
		currentPhase_ = GamePhase::GameOverAnimation;
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

	// アニメーション完了でシーン移動
	if (gameOverAnimation_->IsCompleted()) {
		sceneManager_->ChangeScene(SceneType::Title);
	}
}

///-------------------------------------------/// 
/// フェードアウト時の更新処理
///-------------------------------------------///
void GameScene::UpdateFadeOut() {
	// FadeOut完了で次のシーンへ
	if (fadeOutTimer_ >= fadeOutDuration_) {
		// TODO: シーン遷移処理
		// SceneManager::ChangeScene("Result");
	}
}

///-------------------------------------------/// 
/// 配置関数
///-------------------------------------------///
void GameScene::SpawnObjects(const std::string& json_name) {
	LevelData* levelData = GraphicsResourceGetter::GetLevelData(json_name);

	// オブジェクト分回す
	for (const auto& obj : levelData->objects) {

		/// ===クラス名で分岐=== ///
		switch (obj.classType) {
		case LevelData::ClassType::Player1:
			// 初期化と座標設定
			player_->Initialize();
			player_->SetTranslate(obj.translation);
			player_->SetRotate(Math::QuaternionFromVector(obj.rotation));
			break;
		case LevelData::ClassType::Enemy1:
			// Enemyの座標設定
			enemyManager_->Spawn(EnemyType::LongRange, obj.translation);
			break;
		case LevelData::ClassType::Enemy2:
			// Enemyの座標設定
			enemyManager_->Spawn(EnemyType::CloseRange, obj.translation);
			break;
		case LevelData::ClassType::Object1:
			//NOTE:Objectクラス（名所はGPTに聞く）を作成。当たり判定を付けれるようにする。
			break;
		case LevelData::ClassType::Ground1:
			//NOTE:今後地面を複数個作成する可能性あり。当たり判定を付けれるようにする。
			ground_->Initialize();
			break;
		default:
			break;
		}
	}
}