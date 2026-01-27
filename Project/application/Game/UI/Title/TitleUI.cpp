#include "TitleUI.h"
// SpriteBaseScale
#include "application/Game/UI/SpriteBaseScale.h"
// Service
#include "Engine/System/Service/InputService.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
TitleUI::~TitleUI() {
	// スプライトのリセット
	bgSprite_.reset();
	bgKiriSprite_.reset();
	titleSprite_.reset();
	selectSprite_.start_.reset();
	selectSprite_.option_.reset();
	selectSprite_.exit_.reset();
	selectSprite_.overlay_.reset();
	// OptionUIのリセット
	optionUI_.reset();
}

///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void TitleUI::Initialize() {
	/// ===ウィンドウサイズの取得=== ///
	Vector2 windowSize = {
		static_cast<float>(GraphicsResourceGetter::GetWindowWidth()),
		static_cast<float>(GraphicsResourceGetter::GetWindowHeight())
	};

	/// ===基準スケール=== ///
	scale_ = GetBaseScale();

	/// ===基準値の設定=== ///
	spaceY_ = windowSize.y / 12.0f; // メニュー項目間の垂直スペース
	startPos_ = { // 開始項目のY位置
		windowSize.x / 2.0f,
		windowSize.y / 2.0f + windowSize.y / 8.0f
	};

	/// ===スプライトの初期化=== ///
	// 背景スプライト
	bgSprite_ = std::make_unique<Sprite>();
	bgSprite_->Initialize("TitleBG");
	bgSprite_->SetPosition({ 0.0f, 0.0f });
	bgSprite_->SetSize({ windowSize.x, windowSize.y });
	bgSprite_->Update();

	// 背景(霧)スプライト
	bgKiriSprite_ = std::make_unique<Sprite>();
	bgKiriSprite_->Initialize("TitleBGKiri");
	bgKiriSprite_->SetPosition({ 0.0f, 0.0f });
	bgKiriSprite_->SetSize({ windowSize.x, windowSize.y });
	bgKiriSprite_->Update();

	// タイトルスプライト
	titleSprite_ = std::make_unique<Sprite>();
	titleSprite_->Initialize("Title");
	titleSprite_->SetPosition({ startPos_.x, windowSize.y / 4.0f });
	titleSprite_->SetSize({ 1000.0f * scale_.x, 1000.0f * scale_.y });
	titleSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	titleSprite_->Update();

	// 開始スプライト
	selectSprite_.start_ = std::make_unique<Sprite>();
	selectSprite_.start_->Initialize("Start");
	selectSprite_.start_->SetPosition({ startPos_.x, startPos_.y });
	selectSprite_.start_->SetSize({ 200.0f * scale_.x, 50.0f * scale_.y });
	selectSprite_.start_->SetAnchorPoint({ 0.5f, 0.5f });
	selectSprite_.start_->SetColor({ 0.8f, 0.8f, 0.8f, 1.0f }); // 未選択は灰色
	selectSprite_.start_->Update();

	// オプションスプライト
	selectSprite_.option_ = std::make_unique<Sprite>();
	selectSprite_.option_->Initialize("Option");
	selectSprite_.option_->SetPosition({ startPos_.x, startPos_.y + spaceY_ });
	selectSprite_.option_->SetSize({ 200.0f * scale_.x, 50.0f * scale_.y });
	selectSprite_.option_->SetAnchorPoint({ 0.5f, 0.5f });
	selectSprite_.option_->SetColor({ 0.8f, 0.8f, 0.8f, 1.0f }); // 未選択は灰色
	selectSprite_.option_->Update();

	// 終了スプライト
	selectSprite_.exit_ = std::make_unique<Sprite>();
	selectSprite_.exit_->Initialize("Exit");
	selectSprite_.exit_->SetPosition({ startPos_.x, startPos_.y + spaceY_ * 2.0f });
	selectSprite_.exit_->SetSize({ 200.0f * scale_.x, 50.0f * scale_.y });
	selectSprite_.exit_->SetAnchorPoint({ 0.5f, 0.5f });
	selectSprite_.exit_->SetColor({ 0.8f, 0.8f, 0.8f, 1.0f }); // 未選択は灰色
	selectSprite_.exit_->Update();

	// 選択オーバーレイスプライト
	selectSprite_.overlay_ = std::make_unique<Sprite>();
	selectSprite_.overlay_->Initialize("OverLay"); // テクスチャファイルパスは適宜変更
	selectSprite_.overlay_->SetPosition({ startPos_.x, startPos_.y }); // 初期位置は開始の位置
	selectSprite_.overlay_->SetSize({ 400.0f * scale_.x, 80.0f * scale_.y }); // メニュー項目より少し大きめ
	selectSprite_.overlay_->SetAnchorPoint({ 0.5f, 0.5f });
	selectSprite_.overlay_->SetColor({ 0.8f, 0.8f, 0.8f, 0.8f }); // 半透明の灰色
	selectSprite_.overlay_->Update();

	/// ===OptionUIの初期化=== ///
	optionUI_ = std::make_unique<OptionUI>();
	optionUI_->Initialize();

	/// ===フラグの初期化=== ///
	isOptionOpen_ = false;
	isGameStart_ = false;
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void TitleUI::Update() {
	// セレクトスプライトの更新
	selectSprite_.start_->Update();
	selectSprite_.option_->Update();
	selectSprite_.exit_->Update();
	selectSprite_.overlay_->Update();

	// オプションの更新
	if (isOptionOpen_) {
		optionUI_->TitleUpdate();
	}

	// 選択状態の更新
	UpdateSelecting();
}

///-------------------------------------------/// 
/// 描画処理
///-------------------------------------------///
void TitleUI::Draw() {
	// スプライトの描画
	bgSprite_->Draw(GroundType::Back);
	//bgKiriSprite_->Draw(GroundType::Back);
	titleSprite_->Draw(GroundType::Front);
	selectSprite_.start_->Draw(GroundType::Front);
	selectSprite_.option_->Draw(GroundType::Front);
	selectSprite_.exit_->Draw(GroundType::Front);
	selectSprite_.overlay_->Draw(GroundType::Front, BlendMode::kBlendModeAdd);

	// オプションの描画
	if (isOptionOpen_) {
		optionUI_->TitleDraw();
	}
}

///-------------------------------------------/// 
/// 選択状態の更新
///-------------------------------------------///
void TitleUI::UpdateSelecting() {

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
/// メニュー選択の更新
///-------------------------------------------///
void TitleUI::UpdateMenuSelection() {
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
void TitleUI::ConfirmSelection() {
	// Selectionに応じた処理
	switch (currentSelection_) {
	case MenuSelection::Start:
		// ゲーム開始処理へ移行
		isGameStart_ = true;
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
void TitleUI::UpdateSelectOverlayPosition() {

	// 選択中の項目の位置に応じてオーバーレイを移動
	switch (currentSelection_) {
	case MenuSelection::Start:
		selectSprite_.overlay_->SetPosition({ startPos_.x, startPos_.y });
		break;
	case MenuSelection::Option:
		selectSprite_.overlay_->SetPosition({ startPos_.x, startPos_.y + spaceY_ });
		break;
	case MenuSelection::Exit:
		selectSprite_.overlay_->SetPosition({ startPos_.x, startPos_.y + spaceY_ * 2.0f });
		break;
	}
}

///-------------------------------------------/// 
/// オプション画面の更新
///-------------------------------------------///
void TitleUI::UpdateOptionMenu() {
	// Bボタンでオプション画面を閉じる
	if (InputService::TriggerButton(0, ControllerButtonType::B) || InputService::TriggerKey(DIK_SPACE)) {
		isOptionOpen_ = false;
	}
}