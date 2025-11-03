#include "StartAnimation.h"
// Player & Camera
#include "application/Game/Entity/Player/Player.h"
#include "application/Game/Camera/GameCamera.h"
// Service
#include "Engine/System/Service/DeltaTimeSevice.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"
// c++
#include <algorithm>
#undef max

///-------------------------------------------/// 
/// コンストラクタ
///-------------------------------------------///
StartAnimation::StartAnimation() {}

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
StartAnimation::~StartAnimation() {
	player_ = nullptr;
	camera_ = nullptr;
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void StartAnimation::Initialize(Player* player, GameCamera* camera) {
	player_ = player;
	camera_ = camera;

	// 初期状態の設定
	timer_ = 0.0f;
	isCompleted_ = false;
	currentPhase_ = AnimationPhase::PlayerDescent;
	kDeltaTime_ = DeltaTimeSevice::GetDeltaTime();

	// プレイヤーの初期位置設定
	if (player_) {
		player_->SetTranslate(playerStartPos_);
	}

	// カメラの初期位置設定
	if (camera_) {
		camera_->SetTranslate(cameraStartPos_);
		camera_->SetRotate(cameraStartRot_);
	}
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void StartAnimation::Update() {
	if (isCompleted_) {
		return;
	}

	// デルタタイム取得
	kDeltaTime_ = DeltaTimeSevice::GetDeltaTime();

	// タイマーを進める
	timer_ += kDeltaTime_;

	// フェーズ管理
	if (timer_ < rotationStartTime_) {
		currentPhase_ = AnimationPhase::PlayerDescent;
		UpdatePlayerDescent();
	} else if (timer_ < rotationStartTime_ + rotationDuration_) {
		currentPhase_ = AnimationPhase::CameraRotation;
		UpdateCameraRotation();
	} else if (timer_ < totalDuration_) {
		currentPhase_ = AnimationPhase::FinalSetup;
	} else {
		CompleteAnimation();
	}
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void StartAnimation::Draw() {
}

///-------------------------------------------/// 
/// プレイヤー降下アニメーション
///-------------------------------------------///
void StartAnimation::UpdatePlayerDescent() {
	if (!player_) return;

	// 正規化された時間（0.0 ~ 1.0）
	float t = timer_ / descentDuration_;
	t = std::clamp(t, 0.0f, 1.0f);

	// イージング適用（EaseOutBounce で着地感を演出）
	float easedT = Easing::EaseOutBounce(t);

	// 補間してプレイヤーの位置を更新
	Vector3 currentPos = Math::Lerp(playerStartPos_, playerEndPos_, easedT);
	player_->SetTranslate(currentPos);
}

///-------------------------------------------/// 
/// カメラ回転アニメーション
///-------------------------------------------///
void StartAnimation::UpdateCameraRotation() {
	if (!camera_) return;

	// 回転フェーズでの経過時間
	float rotationTime = timer_ - rotationStartTime_;
	float t = rotationTime / rotationDuration_;
	t = std::clamp(t, 0.0f, 1.0f);

	// イージング適用（EaseInOutCubic でスムーズに）
	float easedT = Easing::EaseInOutCubic(t);

	// カメラ位置の補間（前方から後方上空へ円弧を描くように移動）
	// 中間点を経由して自然な軌道を作る
	Vector3 midPoint = {
		(cameraStartPos_.x + cameraEndPos_.x) * 0.5f,
		std::max(cameraStartPos_.y, cameraEndPos_.y) + 10.0f, // 少し高めに
		(cameraStartPos_.z + cameraEndPos_.z) * 0.5f
	};

	Vector3 currentPos;
	if (easedT < 0.5f) {
		// 前半：開始位置から中間点へ
		float t1 = easedT * 2.0f;
		currentPos = Math::Lerp(cameraStartPos_, midPoint, t1);
	} else {
		// 後半：中間点から終了位置へ
		float t2 = (easedT - 0.5f) * 2.0f;
		currentPos = Math::Lerp(midPoint, cameraEndPos_, t2);
	}

	// カメラ回転の補間（Slerp でスムーズに）
	Quaternion currentRot = Math::SLerp(cameraStartRot_, cameraEndRot_, easedT);

	// カメラ更新
	camera_->SetTranslate(currentPos);
	camera_->SetRotate(currentRot);
}

///-------------------------------------------/// 
/// アニメーション完了処理
///-------------------------------------------///
void StartAnimation::CompleteAnimation() {
	if (isCompleted_) return;

	// フラグを立てる
	isCompleted_ = true;
}