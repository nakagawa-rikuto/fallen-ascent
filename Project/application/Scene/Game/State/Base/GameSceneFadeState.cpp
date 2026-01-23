#include "GameSceneFadeState.h"

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void GameSceneFadeState::Finalize() {
	gameScene_ = nullptr;
	sceneManager_ = nullptr;
}
