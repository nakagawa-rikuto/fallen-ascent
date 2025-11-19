#include "GameClearAnimation.h"
// Entity
#include "application/Game/Entity/Player/Player.h"
// Camera
#include "application/Game/Camera/GameCamera.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"
// Service
#include "Engine/System/Service/DeltaTimeSevice.h"

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameClearAnimation::Initialize(Player* player, GameCamera* camera) {
	player_ = player;
	camera_ = camera;

	// カメラを回転可能型に設定
	camera_->SetFollowCamera(FollowCameraType::Orbiting);

	// 初期フェーズを設定
	currentPhase_ = ClearAnimationPhase::CameraRotation;

	/// ===カメラ回転の初期設定=== ///
	cameraRotationTimer_ = 0.0f;

	// プレイヤーの基準位置を保存
	basePlayerPosition_ = player_->GetTransform().translate;

	// 開始位置のオフセット（画像の情報）
	Vector3 startOffset = { 0.0f, 0.120f, -94.0f };
	// 終了位置のオフセット（画像の情報）
	Vector3 endOffset = { 0.0f, -0.080f, -30.000f };

	// 開始角度と距離を計算
	startAngle_ = atan2f(startOffset.x, startOffset.z);
	cameraDistance_ = sqrtf(startOffset.x * startOffset.x + startOffset.z * startOffset.z);
	cameraHeight_ = startOffset.y;

	// 終了角度を計算
	targetAngle_ = atan2f(endOffset.x, endOffset.z);

	// カメラの回転情報は画像から取得不可のため、現在の回転を使用
	startCameraRotation_ = camera_->GetRotate();
	// 終了時の回転も現在の回転を維持
	targetCameraRotation_ = { 0.0f, -0.98f, 0.2f, 0.0f };

	/// ===ジャンプの初期設定=== ///
	jumpTimer_ = 0.0f;
	jumpCount_ = 0;

	/// ===最終ジャンプの初期設定=== ///
	finalJumpTimer_ = 0.0f;

	camera_->SetOrbitingOffset(startOffset);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GameClearAnimation::Update() {
	switch (currentPhase_) {
	case ClearAnimationPhase::CameraRotation:
		UpdateCameraRotation();
		break;
	case ClearAnimationPhase::FinalJump:
		UpdateFinalJump();
		break;
	case ClearAnimationPhase::Completed:
		// 完了後は何もしない
		break;
	}
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GameClearAnimation::Draw() {
	// 必要に応じてクリア演出用のエフェクトなどを描画
}

///-------------------------------------------/// 
/// 完了フラグの取得
///-------------------------------------------///
bool GameClearAnimation::IsCompleted() const {
	return currentPhase_ == ClearAnimationPhase::Completed;
}

///-------------------------------------------/// 
/// カメラ回転フェーズの更新
///-------------------------------------------///
void GameClearAnimation::UpdateCameraRotation() {
	// タイマーを進める
	cameraRotationTimer_ += DeltaTimeSevice::GetDeltaTime();

	// 進行度を計算(0.0 ~ 1.0)
	float t = cameraRotationTimer_ / cameraRotationDuration_;
	t = (std::min)(t, 1.0f);

	// Easingを適用（滑らかな回転）
	float easedT = Easing::EaseInOutCubic(t);

	// 現在の角度を補間
	float currentAngle = Math::Lerp(startAngle_, targetAngle_, easedT);

	// 距離と高さも補間（94.0 -> 30.0, 0.120 -> -0.080）
	float currentDistance = Math::Lerp(94.0f, 30.0f, easedT);
	float currentHeight = Math::Lerp(0.120f, -0.080f, easedT);

	// 円周上のカメラ位置を計算
	Vector3 newOffset;
	newOffset.x = sinf(currentAngle) * currentDistance;
	newOffset.y = currentHeight;
	newOffset.z = cosf(currentAngle) * currentDistance;

	// カメラのオフセットを更新（Orbitingカメラは自動的にプレイヤーを見る）
	camera_->SetOrbitingOffset(newOffset);

	// カメラの回転を補間（Orbitingカメラが自動調整するため、必要に応じて使用）
	Quaternion currentRotation = Math::SLerp(startCameraRotation_, targetCameraRotation_, easedT);
	camera_->SetRotate(currentRotation);

	// プレイヤーの小ジャンプ更新
	UpdatePlayerSmallJump();

	// 回転完了で最終ジャンプフェーズへ
	if (cameraRotationTimer_ >= cameraRotationDuration_) {
		currentPhase_ = ClearAnimationPhase::FinalJump;
		finalJumpTimer_ = 0.0f;
		// プレイヤーを基準位置に戻す
		player_->SetTranslate(basePlayerPosition_);
	}
}

///-------------------------------------------/// 
/// プレイヤーの小ジャンプ処理
///-------------------------------------------///
void GameClearAnimation::UpdatePlayerSmallJump() {
	// ジャンプタイマーを進める
	jumpTimer_ += DeltaTimeSevice::GetDeltaTime();

	// ジャンプ間隔ごとに新しいジャンプ開始
	if (jumpTimer_ >= jumpInterval_) {
		jumpTimer_ = 0.0f;
		jumpCount_++;
	}

	// 現在のジャンプ進行度を計算
	float localT = jumpTimer_ / jumpInterval_;
	localT = (std::min)(localT, 1.0f);

	// 放物線を描くジャンプ（0.0 -> 1.0 -> 0.0）
	float jumpProgress = -4.0f * smallJumpHeight_ * (localT - 0.5f) * (localT - 0.5f) + smallJumpHeight_;

	// プレイヤーの位置を更新
	Vector3 currentPosition = basePlayerPosition_;
	currentPosition.y += jumpProgress;
	player_->SetTranslate(currentPosition);
}

///-------------------------------------------/// 
/// 最終ジャンプフェーズの更新
///-------------------------------------------///
void GameClearAnimation::UpdateFinalJump() {
	// タイマーを進める
	finalJumpTimer_ += DeltaTimeSevice::GetDeltaTime();

	// 進行度を計算(0.0 ~ 1.0)
	float t = finalJumpTimer_ / finalJumpDuration_;

	if (t <= 1.0f) {
		// 大きな放物線を描くジャンプ
		float jumpProgress = -4.0f * finalJumpHeight_ * (t - 0.5f) * (t - 0.5f) + finalJumpHeight_;

		// プレイヤーの位置を更新
		Vector3 currentPosition = basePlayerPosition_;
		currentPosition.y += jumpProgress;
		player_->SetTranslate(currentPosition);
	} else {
		// ジャンプ完了
		currentPhase_ = ClearAnimationPhase::Completed;
	}
}