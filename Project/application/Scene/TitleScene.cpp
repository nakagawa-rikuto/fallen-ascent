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
	// スプライトのリセット
	bgSprite_.reset();
	bgKiriSprite_.reset();
	titleSprite_.reset();
	startSprite_.reset();
	optionSprite_.reset();
	exitSprite_.reset();
	selectOverlay_.reset();
	dimSprite_.reset();
	optionMenuSprite_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void TitleScene::Initialize() {
	// ISceneの初期化(デフォルトカメラとカメラマネージャ)
	IScene::Initialize();

	// ウィンドウサイズの取得
	float windowWidth = static_cast<float>(GraphicsResourceGetter::GetWindowWidth());
	float windowHeight = static_cast<float>(GraphicsResourceGetter::GetWindowHeight());

	/// ===スプライトの初期化=== ///
	// 背景スプライト
	bgSprite_ = std::make_unique<Sprite>();
	bgSprite_->Initialize("TitleBG");
	bgSprite_->SetPosition({ 0.0f, 0.0f });
	bgSprite_->SetSize({ windowWidth, windowHeight });

	// 背景Kiriスプライト
	bgKiriSprite_ = std::make_unique<Sprite>();
	bgKiriSprite_->Initialize("TitleBGKiri");
	bgKiriSprite_->SetPosition({ 0.0f, 0.0f });
	bgKiriSprite_->SetSize({ windowWidth, windowHeight });

	// タイトルスプライト
	titleSprite_ = std::make_unique<Sprite>();
	titleSprite_->Initialize("Title"); // テクスチャファイルパスは適宜変更
	titleSprite_->SetPosition({ windowWidth / 2.0f, windowHeight / 4.0f });
	titleSprite_->SetSize({ 1000.0f, 1000.0f });
	titleSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	titleSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f });

	// 開始スプライト
	spaceY_ = windowHeight / 16.0f; // メニュー項目間の垂直スペース
	startY_ = windowHeight / 2.0f + windowHeight / 8.0f; // 開始項目のY位置

	startSprite_ = std::make_unique<Sprite>();
	startSprite_->Initialize("Start");
	startSprite_->SetPosition({ windowWidth / 2.0f, startY_ });
	startSprite_->SetSize({ 200.0f, 50.0f });
	startSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	startSprite_->SetColor({ 0.8f, 0.8f, 0.8f, 1.0f }); // 選択中は黄色

	// オプションスプライト
	optionSprite_ = std::make_unique<Sprite>();
	optionSprite_->Initialize("Option");
	optionSprite_->SetPosition({ windowWidth / 2.0f, startY_ + spaceY_ });
	optionSprite_->SetSize({ 200.0f, 50.0f });
	optionSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	optionSprite_->SetColor({ 0.8f, 0.8f, 0.8f, 1.0f }); // 未選択は灰色

	// 終了スプライト
	exitSprite_ = std::make_unique<Sprite>();
	exitSprite_->Initialize("Exit");
	exitSprite_->SetPosition({ windowWidth / 2.0f, startY_ + spaceY_ * 2.0f });
	exitSprite_->SetSize({ 200.0f, 50.0f });
	exitSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	exitSprite_->SetColor({ 0.8f, 0.8f, 0.8f, 1.0f });

	// 選択オーバーレイスプライト
	selectOverlay_ = std::make_unique<Sprite>();
	selectOverlay_->Initialize("OverLay"); // テクスチャファイルパスは適宜変更
	selectOverlay_->SetPosition({ windowWidth / 2.0f, startY_ }); // 初期位置は開始の位置
	selectOverlay_->SetSize({ 400.0f, 80.0f }); // メニュー項目より少し大きめ
	selectOverlay_->SetAnchorPoint({ 0.5f, 0.5f });
	selectOverlay_->SetColor({ 0.8f, 0.8f, 0.8f, 0.8f }); // 半透明の黄色

	// 薄暗いオーバーレイ
	dimSprite_ = std::make_unique<Sprite>();
	dimSprite_->Initialize("uvChecker");
	dimSprite_->SetPosition({ 0.0f, 0.0f });
	dimSprite_->SetSize({ windowWidth, windowHeight });
	dimSprite_->SetColor({ 0.0f, 0.0f, 0.0f, 0.7f }); // 半透明の黒

	// オプションメニュー
	optionMenuSprite_ = std::make_unique<Sprite>();
	optionMenuSprite_->Initialize("uvChecker");
	optionMenuSprite_->SetPosition({ windowWidth / 2.0f, windowHeight / 2.0f });
	optionMenuSprite_->SetSize({ 500.0f, 400.0f });
	optionMenuSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	optionMenuSprite_->SetColor({ 0.3f, 0.3f, 0.5f, 1.0f }); // 青みがかった色

	// 初期化時の位置更新
	bgSprite_->Update();
	bgKiriSprite_->Update();
	titleSprite_->Update();
	startSprite_->Update();
	optionSprite_->Update();
	exitSprite_->Update();
	selectOverlay_->Update();
	dimSprite_->Update();
	optionMenuSprite_->Update();

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
	ImGui::Text("Current Selection: %d", static_cast<int>(currentSelection_));
	ImGui::Text("Option Open: %s", isOptionOpen_ ? "true" : "false");
	ImGui::End();
#endif // USE_IMGUI

	/// ===フェーズ別更新=== ///
	switch (currentFade_) {
	case TitleScene::FadeState::FadeIn:
		UpdateFadeIn();
		break;
	case TitleScene::FadeState::Selecting:
		UpdateSelecting();
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
#pragma region 背景スプライト描画
	// 背景スプライト
	bgSprite_->Draw(GroundType::Back);
	//bgKiriSprite_->Draw(GroundType::Back);
	// タイトルスプライト
	titleSprite_->Draw(GroundType::Back);
#pragma endregion

#pragma region モデル描画
	/// ===ISceneの描画=== ///
	IScene::Draw();
#pragma endregion

#pragma region 前景スプライト描画
	// メニュースプライト
	startSprite_->Draw(GroundType::Front);
	optionSprite_->Draw(GroundType::Front);
	exitSprite_->Draw(GroundType::Front);

	// 選択オーバーレイ（オプション画面が開いていない時のみ）
	if (!isOptionOpen_) {
		selectOverlay_->Draw(GroundType::Front, BlendMode::kBlendModeAdd); // 加算合成で光らせる
	}

	// オプション画面
	if (isOptionOpen_) {
		dimSprite_->Draw(GroundType::Front, BlendMode::KBlendModeNormal);
		optionMenuSprite_->Draw(GroundType::Front);
	}
#pragma endregion
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
/// 選択状態の更新
///-------------------------------------------///
void TitleScene::UpdateSelecting() {

	/// ===スプライトの更新=== ///
	selectOverlay_->Update();

	/// ===オプション画面の処理=== ///
	if (isOptionOpen_) {
		UpdateOptionMenu();
	} else {
		/// ===メニュー選択の更新=== ///
		UpdateMenuSelection();

		/// ===決定処理=== ///
		if (InputService::TriggerButton(0, ControllerButtonType::A) || InputService::TriggerKey(DIK_SPACE)) {
			ConfirmSelection();
		}
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

///-------------------------------------------/// 
/// メニュー選択の更新
///-------------------------------------------///
void TitleScene::UpdateMenuSelection() {
	// 十字キー上
	if (InputService::TriggerButton(0, ControllerButtonType::DPadUP) || InputService::TriggerKey(DIK_UP)) {
		switch (currentSelection_) {
		case MenuSelection::Start:
			currentSelection_ = MenuSelection::Exit; // 一番上から一番下へ
			break;
		case MenuSelection::Option:
			currentSelection_ = MenuSelection::Start;
			break;
		case MenuSelection::Exit:
			currentSelection_ = MenuSelection::Option;
			break;
		}
		UpdateSelectOverlayPosition();
	}

	// 十字キー下
	if (InputService::TriggerButton(0, ControllerButtonType::DPadDOWN) || InputService::TriggerKey(DIK_DOWN)) {
		switch (currentSelection_) {
		case MenuSelection::Start:
			currentSelection_ = MenuSelection::Option;
			break;
		case MenuSelection::Option:
			currentSelection_ = MenuSelection::Exit;
			break;
		case MenuSelection::Exit:
			currentSelection_ = MenuSelection::Start; // 一番下から一番上へ
			break;
		}
		UpdateSelectOverlayPosition();
	}
}

///-------------------------------------------/// 
/// メニュー決定処理
///-------------------------------------------///
void TitleScene::ConfirmSelection() {
	// フェードイン時間
	float fadeInDuration = 1.0f;

	switch (currentSelection_) {
	case MenuSelection::Start:
		sceneManager_->StartFadeOut(TransitionType::ShatterGlass, fadeInDuration);
		currentFade_ = FadeState::FadeOut;
		break;
	case MenuSelection::Option:
		// オプション画面を開く
		isOptionOpen_ = true;
		break;
	case MenuSelection::Exit:
		// ゲーム終了
		PostQuitMessage(0);
		break;
	}
}

///-------------------------------------------/// 
/// メニュー項目の色更新
///-------------------------------------------///
void TitleScene::UpdateSelectOverlayPosition() {
	float windowWidth = static_cast<float>(GraphicsResourceGetter::GetWindowWidth());

	// 選択中の項目の位置に応じてオーバーレイを移動
	switch (currentSelection_) {
	case MenuSelection::Start:
		selectOverlay_->SetPosition({ windowWidth / 2.0f, startY_ });
		break;
	case MenuSelection::Option:
		selectOverlay_->SetPosition({ windowWidth / 2.0f, startY_ + spaceY_ });
		break;
	case MenuSelection::Exit:
		selectOverlay_->SetPosition({ windowWidth / 2.0f, startY_ + spaceY_ * 2.0f });
		break;
	}
}

///-------------------------------------------/// 
/// オプション画面の更新
///-------------------------------------------///
void TitleScene::UpdateOptionMenu() {
	// Bボタンでオプション画面を閉じる
	if (InputService::TriggerButton(0, ControllerButtonType::B) || InputService::TriggerKey(DIK_SPACE)) {
		isOptionOpen_ = false;
	}

	/// ===ここにオプション画面の処理を追加=== ///
}