#include "BlackOutTransition.h"
// Service
#include "Service/GraphicsResourceGetter.h"
#include "Service/Sprite.h"
// Math
#include "Math/EasingMath.h"
#include <algorithm>

namespace MiiEngine {

	///-------------------------------------------/// 
	/// デストラクタ
	///-------------------------------------------///
	BlackOutTransition::~BlackOutTransition() {
		if (sprite_) {
			sprite_.reset();
		}
	}

	///-------------------------------------------/// 
	/// フェードイン開始処理（シーンに入るとき：明転）
	///-------------------------------------------///
	void BlackOutTransition::StartFadeIn(float duration) {
		// スプライトがない場合は作成
		if (!sprite_) {
			sprite_ = std::make_unique<Object2d>();
			sprite_->Initialize("White");
			sprite_->SetAnchorPoint({ 0.5f, 0.5f });
			sprite_->SetPosition({ static_cast<float>(Service::GraphicsResourceGetter::GetWindowWidth()) / 2.0f, static_cast<float>(Service::GraphicsResourceGetter::GetWindowHeight()) / 2.0f });
			sprite_->SetSize({ static_cast<float>(Service::GraphicsResourceGetter::GetWindowWidth()), static_cast<float>(Service::GraphicsResourceGetter::GetWindowHeight()) });
		}
		
		// 完全に真っ黒な状態からスタート
		sprite_->SetColor({ 0.0f, 0.0f, 0.0f, 1.0f });

		// フェードイン開始
		SceneTransitionBase::StartFadeIn(duration);
	}

	///-------------------------------------------/// 
	/// フェードアウト開始処理（シーンから出るとき：暗転）
	///-------------------------------------------///
	void BlackOutTransition::StartFadeOut(float duration) {
		// スプライトの設定
		sprite_ = std::make_unique<Object2d>();
		sprite_->Initialize("White");
		sprite_->SetAnchorPoint({ 0.5f, 0.5f });
		// 画面中央に配置し、画面全体を覆うサイズにする
		sprite_->SetPosition({ static_cast<float>(Service::GraphicsResourceGetter::GetWindowWidth()) / 2.0f, static_cast<float>(Service::GraphicsResourceGetter::GetWindowHeight()) / 2.0f });
		sprite_->SetSize({ static_cast<float>(Service::GraphicsResourceGetter::GetWindowWidth()), static_cast<float>(Service::GraphicsResourceGetter::GetWindowHeight()) });
		
		// 最初は完全に透明な黒
		sprite_->SetColor({ 0.0f, 0.0f, 0.0f, 0.0f });

		// フェードアウト開始
		SceneTransitionBase::StartFadeOut(duration);
	}

	///-------------------------------------------/// 
	/// 描画
	///-------------------------------------------///
	void BlackOutTransition::Draw() {}

	///-------------------------------------------/// 
	/// フェードイン固有の更新処理（徐々に明るく）
	///-------------------------------------------///
	void BlackOutTransition::OnFadeInUpdate() {
		// 進行度を計算（0.0 ~ 1.0）
		float normalizedTime = (std::min)(animation_.currentTime / animation_.duration, 1.0f);

		// イージング適用
		float easedTime = Easing::EaseOutCubic(normalizedTime);

		// アルファ値を 1.0(黒) から 0.0(透明) へ向かわせる
		float alpha = 1.0f - easedTime;
		sprite_->SetColor({ 0.0f, 0.0f, 0.0f, alpha });
		sprite_->Update();

		// アニメーション終了判定
		if (normalizedTime >= 1.0f) {
			animation_.isPlaying = false;
			animation_.isFinished = true;
		}
	}

	///-------------------------------------------/// 
	/// フェードアウト固有の更新処理（徐々に暗く）
	///-------------------------------------------///
	void BlackOutTransition::OnFadeOutUpdate() {
		// 進行度を計算（0.0 ~ 1.0）
		float normalizedTime = (std::min)(animation_.currentTime / animation_.duration, 1.0f);

		// イージング適用
		float easedTime = Easing::EaseOutCubic(normalizedTime);

		// アルファ値を 0.0(透明) から 1.0(黒) へ向かわせる
		float alpha = easedTime;
		sprite_->SetColor({ 0.0f, 0.0f, 0.0f, alpha });
		sprite_->Update();

		// アニメーション終了判定
		if (normalizedTime >= 1.0f) {
			animation_.isPlaying = false;
			animation_.isFinished = true;
		}
	}
}

