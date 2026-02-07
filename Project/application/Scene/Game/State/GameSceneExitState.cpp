#include "GameSceneExitState.h"
// GameScene
#include "application/Scene/Game/GameScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"

///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void GameSceneExitState::Enter(GameScene* gameScene) {
	// ゲームシーンのポインタを保存
	gameScene_ = gameScene;

	// フェードインの継続時間を設定
	float fadeInDuration = 1.0f;

	// シーンマネージャーでフェードインを開始
	sceneManager_->StartFadeOut(TransitionType::None, fadeInDuration);
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void GameSceneExitState::Update() {
	/// ===シーンの切り替え=== ///
	if (sceneManager_->GetTransitionFinished()) {
		// タイトルへ遷移
		sceneManager_->ChangeScene(MiiEngine::SceneType::Title);
	}

	// タイトルへ遷移
	sceneManager_->ChangeScene(MiiEngine::SceneType::Title);
}

///-------------------------------------------/// 
/// 描画処理
///-------------------------------------------///
void GameSceneExitState::Draw() {}

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void GameSceneExitState::Finalize() {
	GameSceneFadeState::Finalize();
}
