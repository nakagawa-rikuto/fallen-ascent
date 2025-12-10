#include "SceneTransitionBase.h"
#include "Engine/System/Service/DeltaTimeSevice.h"

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
bool SceneTransitionBase::IsFading() const { return animation_.isPlaying; }
bool SceneTransitionBase::IsFinished() const { return animation_.isFinished; }
FadeState SceneTransitionBase::GetState() const { return currentState_; }

///-------------------------------------------/// 
/// フェードイン開始処理
///-------------------------------------------///
void SceneTransitionBase::StartFadeIn(float duration) {
	// 設定
	animation_.isPlaying = true;
	animation_.isFinished = false;
	animation_.currentTime = 0.0f;
	animation_.duration = duration;
	currentState_ = FadeState::FadeIn;
}

///-------------------------------------------/// 
/// フェードアウト開始処理
///-------------------------------------------///
void SceneTransitionBase::StartFadeOut(float duration) {

	// 設定
	animation_.isPlaying = true;
	animation_.isFinished = false;
	animation_.currentTime = 0.0f;
	animation_.duration = duration;
	currentState_ = FadeState::FadeOut;
}

///-------------------------------------------/// 
/// フェードインの更新処理
///-------------------------------------------///
void SceneTransitionBase::FadeInUpdate() {
	// 早期リターン
	if (!animation_.isPlaying || animation_.isFinished || currentState_ != FadeState::FadeIn) {
		return;
	}

	// 経過時間を加算
	animation_.currentTime += DeltaTimeSevice::GetDeltaTime();

	/// ===フェードインの更新処理=== ///
	OnFadeInUpdate();
}

///-------------------------------------------/// 
/// フェードアウトの更新処理
///-------------------------------------------///
void SceneTransitionBase::FadeOutUpdate() {
	// 早期リターン
	if (!animation_.isPlaying || animation_.isFinished || currentState_ != FadeState::FadeOut) {
		return;
	}

	// 経過時間を加算
	animation_.currentTime += DeltaTimeSevice::GetDeltaTime();

	/// ===フェードアウトの更新処理=== ///
	OnFadeOutUpdate();
}

///-------------------------------------------/// 
/// 描画処理
///-------------------------------------------///
void SceneTransitionBase::Draw() {
	// 基底クラスでは何もしない
}

///-------------------------------------------/// 
/// リセット
///-------------------------------------------///
void SceneTransitionBase::Reset() {
	animation_.isPlaying = false;
	animation_.isFinished = false;
	animation_.currentTime = 0.0f;
	animation_.duration = 1.2f;
	currentState_ = FadeState::None;
}
