#include "OptionUI.h"
// SpriteBaseScale
#include "application/Game/UI/SpriteBaseScale.h"
// Service
#include "Engine/System/Service/InputService.h"
#include "Engine/System/Service/AudioService.h"
#ifdef USE_IMGUI
// ImGui
#include <imgui.h>
#endif // USE_IMGUI

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
OptionUI::~OptionUI() {
	// スプライトのリセット
	dimSprite_.reset();
	optionMenuSprite_.reset();
	titleButtonSprite_.reset();
	audioButtonSprite_.reset();
	audioTextSprite_.reset();
	audioSliderSprite_.reset();
	audioHandleSprite_.reset();
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
	dimSprite_->Initialize("White");
	dimSprite_->SetPosition({ 0.0f, 0.0f });
	dimSprite_->SetSize({ windowSize.x, windowSize.y });
	dimSprite_->SetColor({ 0.0f, 0.0f, 0.0f, 0.7f }); // 半透明の黒
	dimSprite_->Update();

	// オプションメニュー
	optionMenuSprite_ = std::make_unique<Sprite>();
	optionMenuSprite_->Initialize("White");
	optionMenuSprite_->SetPosition({ windowSize.x / 2.0f, windowSize.y / 2.0f });
	optionMenuSprite_->SetSize({ 800.0f * scale_.x, 700.0f * scale_.y });
	optionMenuSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	optionMenuSprite_->SetColor({ 0.3f, 0.3f, 0.5f, 1.0f }); // 青みがかった色
	optionMenuSprite_->Update();

	/// ===AudioSprite=== ///
	audioButtonSprite_ = std::make_unique<Sprite>();
	audioButtonSprite_->Initialize("OptionVolume");
	audioButtonSprite_->SetPosition({ audioButtonPos_.x * scale_.x, audioButtonPos_.y * scale_.y });
	audioButtonSprite_->SetSize({ 200.0f * scale_.x, 50.0f * scale_.y });
	audioButtonSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	audioButtonSprite_->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f }); // 黄色
	audioButtonSprite_->Update();

	// オーディオスプライト
	audioHandleSprite_ = std::make_unique<Sprite>();
	audioHandleSprite_->Initialize("White");
	audioHandleSprite_->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f }); // 黄色

	audioTextSprite_ = std::make_unique<Sprite>();
	audioTextSprite_->Initialize("White");
	audioTextSprite_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f }); // 黒色

	audioSliderSprite_ = std::make_unique<Sprite>();
	audioSliderSprite_->Initialize("White");
	audioSliderSprite_->SetColor({ 0.8f, 0.8f, 0.8f, 1.0f }); // 灰色
	UpdateAudioSprite();

	// タイトルボタンスプライト
	titleButtonSprite_ = std::make_unique<Sprite>();
	titleButtonSprite_->Initialize("OptionTitle");
	titleButtonSprite_->SetPosition({ windowSize.x / 2.0f, windowSize.y / 2.0f + 150.0f * scale_.y });
	titleButtonSprite_->SetSize({ 400.0f * scale_.x, 150.0f * scale_.y });
	titleButtonSprite_->SetAnchorPoint({ 0.5f, 0.5f });
	titleButtonSprite_->Update();

	// 初期値設定
	returnToTitle_ = false;
	optionState_ = OptionState::Audio;
}

///-------------------------------------------/// 
/// 更新処理（タイトル）
///-------------------------------------------///
void OptionUI::TitleUpdate() {
#ifdef USE_IMGUI

	ImGui::Begin("OptionUI");
	ImGui::Text("OptionUI");
	ImGui::DragFloat2("audioSpritePos", &audioButtonPos_.x, 1.0f);
	ImGui::DragFloat2("spritePos_", &spritePos_.x, 1.0f);
	ImGui::DragFloat2("spritePos_2_", &spritePos_2_.x, 1.0f);
	ImGui::DragFloat2("spriteSize_", &spriteSize_.x, 1.0f);
	ImGui::DragFloat2("spriteSize_2_", &spriteSize_2_.x, 1.0f);
	ImGui::End();

#endif // USE_IMGUI


	// 音量調整（左右）
	if (InputService::PushButton(stickNo_, ControllerButtonType::DPadRIGHT)) {
		volumeBGM_ = (std::min)(volumeBGM_ + 1, 100);
		AudioService::AllVolumeSound(volumeBGM_ / 100.0f);
	}
	if (InputService::PushButton(stickNo_, ControllerButtonType::DPadLEFT)) {
		volumeBGM_ = (std::max)(volumeBGM_ - 1, 0);
		AudioService::AllVolumeSound(volumeBGM_ / 100.0f);
	}

	// スプライトの更新
	UpdateAudioSprite();

}

///-------------------------------------------/// 
/// 更新処理（ゲーム）
///-------------------------------------------///
void OptionUI::GameUpdate() {

	// 項目選択（上下）
	if (InputService::TriggerButton(stickNo_, ControllerButtonType::DPadUP) ||
		InputService::TriggerButton(stickNo_, ControllerButtonType::DPadDOWN)) {
		// Audio <-> Title をトグル
		if (optionState_ == OptionState::Audio) {
			optionState_ = OptionState::Title;
			titleButtonSprite_->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f }); // 選択中は黄色
			audioHandleSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }); // 白色
			audioButtonSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }); // 白色
		} else {
			optionState_ = OptionState::Audio;
			titleButtonSprite_->SetColor({ 1.0f, 1.0f, 1.0f, 1.0f }); // 非選択中は白色
			audioHandleSprite_->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f }); // 黄色
			audioButtonSprite_->SetColor({ 1.0f, 1.0f, 0.0f, 1.0f }); // 黄色
		}
	}

    // 音量調整（左右）
    if (optionState_ == OptionState::Audio) { // BGM
        if (InputService::PushButton(stickNo_, ControllerButtonType::DPadRIGHT)) {
            volumeBGM_ = (std::min)(volumeBGM_ + 1, 100);
            AudioService::AllVolumeSound(volumeBGM_ / 100.0f);
        }
        if (InputService::PushButton(stickNo_, ControllerButtonType::DPadLEFT)) {
            volumeBGM_ = (std::max)(volumeBGM_ - 1, 0);
            AudioService::AllVolumeSound(volumeBGM_ / 100.0f);
        }
    }

	// スプライトの更新
	titleButtonSprite_->Update();
	// スプライトの更新
	UpdateAudioSprite();

    // タイトルに戻る
    if (optionState_ == OptionState::Title && InputService::TriggerButton(stickNo_, ControllerButtonType::A)) {
        returnToTitle_ = true;
    }
}

///-------------------------------------------/// 
/// 描画処理
///-------------------------------------------///
void OptionUI::TitleDraw() {
	dimSprite_->Draw(GroundType::Front, BlendMode::KBlendModeNormal);
	//optionMenuSprite_->Draw(GroundType::Front);
	audioButtonSprite_->Draw(GroundType::Front);
	audioHandleSprite_->Draw(GroundType::Front);
	audioTextSprite_->Draw(GroundType::Front);
	audioSliderSprite_->Draw(GroundType::Front);
}

///-------------------------------------------/// 
/// 描画処理
///-------------------------------------------///
void OptionUI::GameDraw() {
	dimSprite_->Draw(GroundType::Front, BlendMode::KBlendModeNormal);
	//optionMenuSprite_->Draw(GroundType::Front);
	audioButtonSprite_->Draw(GroundType::Front);
	audioHandleSprite_->Draw(GroundType::Front);
	audioTextSprite_->Draw(GroundType::Front);
	audioSliderSprite_->Draw(GroundType::Front);
	titleButtonSprite_->Draw(GroundType::Front);
}

///-------------------------------------------/// 
/// オーディオスプライトの更新
///-------------------------------------------///
void OptionUI::UpdateAudioSprite() {

	audioButtonSprite_->SetPosition({ audioButtonPos_.x * scale_.x, audioButtonPos_.y * scale_.y });
	audioButtonSprite_->Update();

	// オーディオスプライト
	audioHandleSprite_->SetPosition({ spritePos_.x * scale_.x, spritePos_.y * scale_.y });
	audioHandleSprite_->SetAnchorPoint({ 0.0f, 0.0f });
	audioHandleSprite_->SetSize({ spriteSize_.x * scale_.x, spriteSize_.y * scale_.y });
	audioHandleSprite_->Update();

	audioTextSprite_->SetPosition({ spritePos_2_.x * scale_.x, spritePos_2_.y * scale_.y });
	audioTextSprite_->SetAnchorPoint({ 0.0f, 0.0f });
	audioTextSprite_->SetSize({ (spriteSize_2_.x * 100.0f) * scale_.x, spriteSize_2_.y * scale_.y });
	audioTextSprite_->Update();

	audioSliderSprite_->SetPosition({ spritePos_2_.x * scale_.x, spritePos_2_.y * scale_.y });
	audioSliderSprite_->SetAnchorPoint({ 0.0f, 0.0f });
	audioSliderSprite_->SetSize({ (spriteSize_2_.x * static_cast<float>(volumeBGM_)) * scale_.x, spriteSize_2_.y * scale_.y });
	audioSliderSprite_->Update();

}
