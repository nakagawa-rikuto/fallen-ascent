#include "OptionUI.h"
// SpriteBaseScale
#include "application/Game/UI/SpriteBaseScale.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
OptionUI::~OptionUI() {
	// スプライトのリセット
	dimSprite_.reset();
	optionMenuSprite_.reset();
}

///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void OptionUI::Initialize() {

	/// ===ウィンドウサイズの取得=== ///
	Vector2 windowSize = {
		static_cast<float>(GraphicsResourceGetter::GetWindowWidth()),
		static_cast<float>(GraphicsResourceGetter::GetWindowHeight())
	};

	/// ===基準スケール=== ///
	scale_ = GetBaseScale();

	// 薄暗いオーバーレイ
	dimSprite_ = std::make_unique<Sprite>();
	dimSprite_->Initialize("uvChecker");
	dimSprite_->SetPosition({ 0.0f, 0.0f });
	dimSprite_->SetSize({ windowSize.x, windowSize.y });
	dimSprite_->SetColor({ 0.0f, 0.0f, 0.0f, 0.7f }); // 半透明の黒
	dimSprite_->Update();

	// オプションメニュー
	optionMenuSprite_ = std::make_unique<Sprite>();
	optionMenuSprite_->Initialize("uvChecker");
	optionMenuSprite_->SetPosition({ windowSize.x / 2.0f, windowSize.y / 2.0f });
	optionMenuSprite_->SetSize({ 500.0f * scale_.x, 400.0f * scale_.y });
	optionMenuSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	optionMenuSprite_->SetColor({ 0.3f, 0.3f, 0.5f, 1.0f }); // 青みがかった色
	optionMenuSprite_->Update();
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void OptionUI::Update() {}

///-------------------------------------------/// 
/// 描画処理
///-------------------------------------------///
void OptionUI::Draw() {
	dimSprite_->Draw(GroundType::Front, BlendMode::KBlendModeNormal);
	optionMenuSprite_->Draw(GroundType::Front);
}