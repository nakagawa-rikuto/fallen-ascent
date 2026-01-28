#include "GameSceneInGameState.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"
// GameScene
#include "application/Scene/Game/GameScene.h"
// State
#include "GameSceneGameOverAnimationState.h"
#include "GameSceneGameClearState.h"
#include "GameSceneExitState.h"
// Service
#include "Engine/System/Service/InputService.h"
#include "Engine/System/Service/DeltaTimeSevice.h"

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

	/// ===OptionUI=== ///
	optionUI_ = std::make_unique<OptionUI>();
	optionUI_->Initialize();
	optionUI_->GameUpdate();
	isOptionActive_ = false;
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void GameSceneInGameState::Update() {

	// オプションアクティブ
	if (InputService::TriggerButton(0, ControllerButtonType::Start)) {
		if (isOptionActive_) {
			DeltaTimeSevice::SetDeltaTime(1.0f / 60.0f);
			isOptionActive_ = false;
		} else {
			DeltaTimeSevice::SetDeltaTime(0.0f);
			isOptionActive_ = true;
		}
	}

	// UIの更新
	if (isOptionActive_) {
		// OptionUIの更新
		optionUI_->GameUpdate();
	} else {
		// EnemyManagerの更新
		gameScene_->GetEnemyManager()->Update();
		// Playerの更新
		gameScene_->GetPlayer()->Update();
		// GameSceneUIの更新
		ui_->Update();
	}

	// Playerが死亡しているか確認
	if (gameScene_->GetPlayer()->GetIsDead()) {
		// ゲームオーバーアニメーション状態に変更
		gameScene_->ChangState(std::make_unique<GameSceneGameOverAnimationState>());
	} else if (gameScene_->GetEnemyManager()->GetTotalEnemyCount() <= 0) {
		// ゲームクリアアニメーション状態に変更
		gameScene_->ChangState(std::make_unique<GameSceneGameOverAnimationState>());
	} else if (optionUI_->GetReturnToTitle()) {
		DeltaTimeSevice::SetDeltaTime(1.0f / 60.0f);
		// Exit状態に変更
		gameScene_->ChangState(std::make_unique<GameSceneGameOverAnimationState>());
	}
}

///-------------------------------------------/// 
/// 描画処理
///-------------------------------------------///
void GameSceneInGameState::Draw() {
	
	if (isOptionActive_) {
		optionUI_->GameDraw();
	} else {
		ui_->Draw();
	}
}

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void GameSceneInGameState::Finalize() {
	ui_.reset();
	optionUI_.reset();
	GameSceneFadeState::Finalize();
}
