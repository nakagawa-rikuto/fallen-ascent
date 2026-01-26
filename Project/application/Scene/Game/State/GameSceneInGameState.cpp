#include "GameSceneInGameState.h"
// GameScene
#include "application/Scene/Game/GameScene.h"
// State
#include "GameSceneGameOverAnimationState.h"
#include "GameSceneGameClearState.h"

///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void GameSceneInGameState::Enter(GameScene* gameScene) {
	// ゲームシーンのポインタを保存
	gameScene_ = gameScene;

	/// ===GameSceneUI=== ///
	ui_ = std::make_unique<GameSceneUI>();
	ui_->Initialize();
	// GameSceneUIにPlayerを設定
	ui_->SetPlayer(gameScene_->GetPlayer());
	ui_->Update();
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void GameSceneInGameState::Update() {

	// EnemyManagerの更新
	gameScene_->GetEnemyManager()->Update();
	// Playerの更新
	gameScene_->GetPlayer()->Update();
	// GameSceneUIの更新
	ui_->Update();

	// Playerが死亡しているか確認
	if (gameScene_->GetPlayer()->GetIsDead()) {
		// ゲームオーバーアニメーション状態に変更
		gameScene_->ChangState(std::make_unique<GameSceneGameOverAnimationState>());
	} else if (gameScene_->GetEnemyManager()->GetTotalEnemyCount() <= 0) {
		// ゲームクリアアニメーション状態に変更
		gameScene_->ChangState(std::make_unique<GameSceneGameOverAnimationState>());
	}
}

///-------------------------------------------/// 
/// 描画処理
///-------------------------------------------///
void GameSceneInGameState::Draw() {
	ui_->Draw();
}

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void GameSceneInGameState::Finalize() {
	GameSceneFadeState::Finalize();
}
