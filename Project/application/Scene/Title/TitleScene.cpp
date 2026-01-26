#include "TitleScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"
// Service
#include "Engine/System/Service/InputService.h"
#include "Engine/System/Service/GraphicsResourceGetter.h"
#include "Engine/System/Service/OffScreenService.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
TitleScene::~TitleScene() {
	// ISceneのデストラクタ
	IScene::~IScene();
	// TitleUIのリセット
	titleUI_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void TitleScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	/// ===TitleUI=== ///
	titleUI_ = std::make_unique<TitleUI>();
	titleUI_->Initialize();

	/// ===Fadeの設定=== ///
	currentFade_ = FadeState::Selecting; // デバッグ用にフェードインをスキップ
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void TitleScene::Update() {
	/// ===デバック用ImGui=== ///
#ifdef USE_IMGUI
	ImGui::Begin("TitleScene");
	ImGui::Text("Fade State: %d", static_cast<int>(currentFade_));
	ImGui::End();
#endif // USE_IMGUI

	/// ===フェーズ別更新=== ///
	switch (currentFade_) {
	case TitleScene::FadeState::FadeIn:
		UpdateFadeIn();
		break;
	case TitleScene::FadeState::Selecting:
		titleUI_->Update();
		if (titleUI_->IsGameStart()) {
			currentFade_ = FadeState::FadeOut;
			sceneManager_->StartFadeOut(TransitionType::ShatterGlass, 1.0f);
		}
		break;
	case TitleScene::FadeState::FadeOut:
		UpdateFadeOut();
		break;
	}

	/// ===ISceneの更新=== ///
	IScene::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void TitleScene::Draw() {
	/// ===TitleUIの描画=== ///
	titleUI_->Draw();

	/// ===ISceneの描画=== ///
	IScene::Draw();
}

///-------------------------------------------/// 
/// フェードインの更新
///-------------------------------------------///
void TitleScene::UpdateFadeIn() {
	/// ===Fadeの変更=== ///
	if (sceneManager_->GetTransitionFinished()) {
		currentFade_ = FadeState::Selecting;
	}
}

///-------------------------------------------/// 
/// フェードアウトの更新
///-------------------------------------------///
void TitleScene::UpdateFadeOut() {

	/// ===シーンの切り替え=== ///
	if (sceneManager_->GetTransitionFinished()) {
		// ゲームシーンへ遷移
		sceneManager_->ChangeScene(SceneType::Game);
	}
}