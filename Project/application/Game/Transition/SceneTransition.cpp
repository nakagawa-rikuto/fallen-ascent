#include "SceneTransition.h"
#include <algorithm>
// Service
#include "Engine/System/Service/OffScreenService.h"
#include "Engine/System/Service/GraphicsResourceGetter.h"
// Math
#include "Math/EasingMath.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
SceneTransition::~SceneTransition() {
	if (sprite_) {
		sprite_.reset();
	}
}

///-------------------------------------------/// 
/// 
///-------------------------------------------///
void SceneTransition::StartFadeIn(float duration) {
	
	// Sprite
	sprite_ = std::make_unique<Sprite>();
	sprite_->Initialize("White");
	sprite_->SetSize({ static_cast<float>(GraphicsResourceGetter::GetWindowWidth()), static_cast<float>(GraphicsResourceGetter::GetWindowHeight()) });
	sprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // 黒で初期化
	sprite_->Update();

	isPlaying_ = true;
	isFinished_ = false;
	currentTime_ = 0.0f;
	duration_ = duration;
	currentState_ = FadeState::FadeIn;
}

///-------------------------------------------/// 
/// 
///-------------------------------------------///
void SceneTransition::StartFadeOut(float duration) {
	// エフェクトタイプの設定
	//OffScreenService::SetOffScreenType(OffScreenType::ShatterGlass);

	// デフォルトパラメータの設定
	data_.progress = 0.0f;
	data_.impactX = 0.5f;
	data_.impactY = 0.5f;
	data_.crackDensity = 15.0f;
	data_.dispersion = 1.2f;
	data_.rotation = 1.5f;
	data_.fadeOut = 0.5f;  // フェードアウトを有効に

	// 新しいランダムパターンを生成
	data_.randomSeed = static_cast<float>(rand() % 10000) / 10.0f;

	isPlaying_ = true;
	isFinished_ = false;
	currentTime_ = 0.0f;
	duration_ = duration;
	currentState_ = FadeState::FadeOut;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void SceneTransition::FadeOutUpdate() {
	if (!isPlaying_ || isFinished_) {
		return;
	}

	currentTime_ += 1.0f / 60.0f;

	// 進行度を計算（0.0 ~ 1.0）
	float normalizedTime = (std::min)(currentTime_ / duration_, 1.0f);

	// イージング適用（最初は速く、後でゆっくり）
	float easedTime = Easing::EaseOutCubic(normalizedTime);

	if (data_.progress < 0.5f) {
		// 各フェーズでの動きを調整
		if (normalizedTime < 0.5f) {
			// Phase 1: ひび割れが広がる（0.0 ~ 0.15秒）
			data_.crackDensity = 15.0f;
			data_.progress = easedTime * 0.3f;
		} else {
			// Phase 3: 破片が飛散して消える（0.5 ~ 1.0秒）
			data_.crackDensity = 15.0f;
			data_.dispersion = 4.0f;
			float phaseTime = (normalizedTime - 0.5f) / 0.5f;
			data_.progress = 0.25f + Easing::EaseOutCubic(phaseTime) * 0.3f;
		}
	} else {
		// エフェクト終了判定
		isPlaying_ = false;
		isFinished_ = true;
		currentState_ = FadeState::Finished;
		data_.progress = 1.0f;
	}

	// 設定
	OffScreenService::SetShatterGlassData(data_);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void SceneTransition::FadeInUpdate() {
	if (!isPlaying_ || isFinished_) {
		return;
	}

	// スプライトの色（アルファ値のみ更新）
	Vector4 color = sprite_->GetColor();
	color.w += 0.01f; // アルファ値を更新
	

	// 透明度が1.0に到達したら終了
	if (color.w >= 1.0f) {
		isPlaying_ = false;
		isFinished_ = true;
		currentState_ = FadeState::Finished;
	}

	sprite_->SetColor(color);
	sprite_->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void SceneTransition::Draw() {
	if (sprite_) {
		sprite_->Draw(GroundType::Front, BlendMode::KBlendModeNormal);
	}
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
bool SceneTransition::IsFading() const { return isPlaying_; }
bool SceneTransition::IsFinished() const { return isFinished_; }
FadeState SceneTransition::GetState() const { return currentState_; }
