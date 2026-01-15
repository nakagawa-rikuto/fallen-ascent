#include "GameSceneUI.h"
// Player
#include "application/Game/Entity/Player/Player.h"

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void GameSceneUI::SetPlayer(Player* player) {
	// PlayerUI
	playerUI_->SetPlayer(player);
}

///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void GameSceneUI::Initialize() {
	// PlayerUIの生成と初期化
	playerUI_ = std::make_unique<PlayerUI>();
	playerUI_->Initialize();
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void GameSceneUI::Update() {
	// PlayerUIの更新
	playerUI_->Update();
}

///-------------------------------------------/// 
/// 描画処理
///-------------------------------------------///
void GameSceneUI::Draw() {
	// PlayerUIの描画
	playerUI_->Draw();
}