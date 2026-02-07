#include "GameClearAnimation.h"
// Entity
#include "application/Game/Entity/Player/Player.h"
// Camera
#include "application/Game/Camera/GameCamera.h"
// Service
#include "Service/DeltaTime.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"
#include "Math/MatrixMath.h"


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameClearAnimation::Initialize(Player* player, GameCamera* camera) {
	player_ = player;
	camera_ = camera;

	// プレイヤーの回転を保存
	Quaternion playerRotation = player_->GetTransform().rotate;

	/// ===カメラ回転の初期設定=== ///
	cameraInfo_.timer = 0.0f;
	// プレイヤーの前方向ベクトルを取得
	cameraInfo_.playerForward = Math::TransformNormal({ 0.0f, 0.0f, 1.0f }, Math::MakeRotateQuaternionMatrix(playerRotation));
	// 開始位置のオフセット
	Vector3 startOffset = { 0.0f, 0.120f, -94.0f };
	// 終了位置のオフセット
	float finalDistance = 30.0f;
	float finalHeight = 0.08f;
	Vector3 endOffset = { 0.0f, -finalHeight, -finalDistance };
	// 開始角度と距離を計算
	cameraInfo_.startAngle = atan2f(startOffset.x, startOffset.z);
	cameraInfo_.distance = sqrtf(startOffset.x * startOffset.x + startOffset.z * startOffset.z);
	cameraInfo_.height = startOffset.y;
	// 終了角度を計算
	cameraInfo_.targetAngle = atan2f(endOffset.x, endOffset.z);
	// カメラの回転情報は画像から取得不可のため、現在の回転を使用
	cameraInfo_.startRotation = camera_->GetRotate();
	// 終了時の回転も現在の回転を維持
	cameraInfo_.targetRotation = { 0.0f, -0.98f, 0.2f, 0.0f };

	/// ===ジャンプの初期設定=== ///
	smallJumpInfo_.timer = 0.0f;
	smallJumpInfo_.count = 0;
	smallJumpInfo_.basePlayerPosition = player_->GetTransform().translate;

	/// ===最終ジャンプの初期設定=== ///
	finalJumpInfo_.timer = 0.0f;

	/// ===カメラの設定=== ///
	// 回転可能型に設定
	camera_->SetFollowCamera(FollowCameraType::Orbiting); 
	// カメラのターゲットを設定
	camera_->SetTarget(&smallJumpInfo_.basePlayerPosition, &playerRotation); 
	// 初期オフセットを設定
	camera_->SetOrbitingOffset(startOffset); 

	/// ===Playerの設定=== ///
	player_->SetGravity(-9.8f); 
	// 初期速度をリセット
	player_->SetVelocity({ 0.0f, 0.0f, 0.0f }); 

	/// ===初期フェーズを設定=== ///
	currentPhase_ = ClearAnimationPhase::CameraRotation;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GameClearAnimation::Update() {
	/// ===DeltaTimeの取得=== ///
	deltaTime_ = Service::DeltaTime::GetDeltaTime();

	/// ===フェーズごとの更新=== ///
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
void GameClearAnimation::Draw() {}

///-------------------------------------------/// 
/// 完了フラグの取得
///-------------------------------------------///
bool GameClearAnimation::IsCompleted() const {return currentPhase_ == ClearAnimationPhase::Completed;}

///-------------------------------------------/// 
/// カメラ回転フェーズの更新
///-------------------------------------------///
void GameClearAnimation::UpdateCameraRotation() {
	// タイマーを進める
	cameraInfo_.timer += deltaTime_;

	// 進行度を計算(0.0 ~ 1.0)
	float t = cameraInfo_.timer / cameraInfo_.duration;
	t = (std::min)(t, 1.0f);

	// Easingを適用（滑らかな回転）
	float easedT = Easing::EaseInOutCubic(t);

	// 現在の角度を補間
	float currentAngle = Math::Lerp(cameraInfo_.startAngle, cameraInfo_.targetAngle, easedT);

	// 距離と高さも補間
	float finalDistance = 30.0f;
	float finalHeight = 0.08f;
	float currentDistance = Math::Lerp(94.0f, finalDistance, easedT);
	float currentHeight = Math::Lerp(0.120f, -finalHeight, easedT);

	// 円周上のカメラ位置を計算
	Vector3 newOffset;
	newOffset.x = sinf(currentAngle) * currentDistance;
	newOffset.y = currentHeight;
	newOffset.z = cosf(currentAngle) * currentDistance;

	// カメラのオフセットを更新
	camera_->SetOrbitingOffset(newOffset);

	// カメラの回転を補間
	Quaternion currentRotation = Math::SLerp(cameraInfo_.startRotation, cameraInfo_.targetRotation, easedT);
	camera_->SetRotate(currentRotation);

	// プレイヤーの小ジャンプ更新
	//UpdatePlayerSmallJump();

	// 回転完了で最終ジャンプフェーズへ
	if (cameraInfo_.timer >= cameraInfo_.duration) {
		currentPhase_ = ClearAnimationPhase::FinalJump;
		finalJumpInfo_.timer = 0.0f;
	}
}

///-------------------------------------------/// 
/// プレイヤーの小ジャンプ処理
///-------------------------------------------///
void GameClearAnimation::UpdatePlayerSmallJump() {
	// ジャンプタイマーを進める
	smallJumpInfo_.timer += deltaTime_;

	// ジャンプ間隔ごとに新しいジャンプ開始
	if (smallJumpInfo_.timer >= smallJumpInfo_.interval) {
		smallJumpInfo_.timer = 0.0f;
		smallJumpInfo_.count++;
	}

	// 現在のジャンプ進行度を計算
	float localT = smallJumpInfo_.timer / smallJumpInfo_.interval;
	localT = (std::min)(localT, 1.0f);

	// 放物線を描くジャンプ
	float jumpProgress = -1.0f * smallJumpInfo_.height * (localT - 0.5f) * (localT - 0.5f) + smallJumpInfo_.height;

	// プレイヤーのVelocityを更新
	Vector3 playerCurrentVelocity = player_->GetVelocity();
	playerCurrentVelocity.y += jumpProgress * deltaTime_;
	player_->SetVelocity(playerCurrentVelocity);
}

///-------------------------------------------/// 
/// 最終ジャンプフェーズの更新
///-------------------------------------------///
void GameClearAnimation::UpdateFinalJump() {
	// タイマーを進める
	finalJumpInfo_.timer += deltaTime_;

	// 進行度を計算(0.0 ~ 1.0)
	float t = finalJumpInfo_.timer / finalJumpInfo_.duration;

	if (t <= 1.0f) {
		// 大きな放物線を描くジャンプ
		float jumpProgress = -4.0f * finalJumpInfo_.height * (t - 0.5f) * (t - 0.5f) + finalJumpInfo_.height;

		// プレイヤーのVelocityを更新
		Vector3 playerCurrentVelocity = player_->GetVelocity();
		playerCurrentVelocity.y += jumpProgress * deltaTime_;
		player_->SetVelocity(playerCurrentVelocity);
	} else {
		// ジャンプ完了
		currentPhase_ = ClearAnimationPhase::Completed;
	}
}