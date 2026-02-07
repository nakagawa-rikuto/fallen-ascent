#include "ShatterGlassTransition.h"
#include <algorithm>
// Service
#include "Service/PostEffect.h"
#include "Service/GraphicsResourceGetter.h"
// Math
#include "Math/EasingMath.h"

namespace MiiEngine {
	///-------------------------------------------/// 
	/// デストラクタ
	///-------------------------------------------///
	ShatterGlassTransition::~ShatterGlassTransition() {
		Service::PostEffect::SetOffScreenType(OffScreenType::CopyImage);
		if (sprite_) {
			sprite_.reset();
		}
	}

	///-------------------------------------------/// 
	/// フェードイン開始処理
	///-------------------------------------------///
	void ShatterGlassTransition::StartFadeIn(float duration) {
		// エフェクトタイプの設定
		Service::PostEffect::SetOffScreenType(OffScreenType::ShatterGlass);
		// Spriteの色をリセット
		if (sprite_) {
			sprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // 透明で初期化
		}

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

		// フェードアウト開始
		SceneTransitionBase::StartFadeIn(duration);
	}

	///-------------------------------------------/// 
	/// フェードアウト開始処理
	///-------------------------------------------///
	void ShatterGlassTransition::StartFadeOut(float duration) {
		// Sprite
		sprite_ = std::make_unique<Sprite>();
		sprite_->Initialize("White");
		sprite_->SetSize({ static_cast<float>(Service::GraphicsResourceGetter::GetWindowWidth()), static_cast<float>(Service::GraphicsResourceGetter::GetWindowHeight()) });
		sprite_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // 透明で初期化
		sprite_->Update();

		// フェードイン開始
		SceneTransitionBase::StartFadeOut(duration);
	}

	///-------------------------------------------/// 
	/// 描画
	///-------------------------------------------///
	void ShatterGlassTransition::Draw() {
		if (sprite_) {
			sprite_->Draw(GroundType::Front, BlendMode::KBlendModeNormal);
		}
	}

	///-------------------------------------------/// 
	/// フェードイン固有の更新処理
	///-------------------------------------------///
	void ShatterGlassTransition::OnFadeInUpdate() {
		// 進行度を計算（0.0 ~ 1.0）
		float normalizedTime = (std::min)(animation_.currentTime / animation_.duration, 1.0f);

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
			Service::PostEffect::SetOffScreenType(OffScreenType::CopyImage);
			data_.progress = 1.0f;
			// エフェクト終了判定
			animation_.isPlaying = false;
			animation_.isFinished = true;
		}

		// 設定
		Service::PostEffect::SetShatterGlassData(data_);
		if (sprite_) {
			sprite_->Update();
		}
	}

	///-------------------------------------------/// 
	/// フェードアウト固有の更新処理
	///-------------------------------------------///
	void ShatterGlassTransition::OnFadeOutUpdate() {
		// スプライトの色（アルファ値のみ更新）
		Vector4 color = sprite_->GetColor();
		color.w += 0.01f; // アルファ値を更新

		// 透明度が1.0に到達したら終了
		if (color.w >= 1.0f) {
			animation_.isPlaying = false;
			animation_.isFinished = true;
		}

		sprite_->SetColor(color);
		sprite_->Update();
	}
}