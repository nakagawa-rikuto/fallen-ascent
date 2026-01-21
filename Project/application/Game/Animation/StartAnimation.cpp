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
#include <functional>

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

	/// ===値の初期化=== ///
	// タイマーとフラグ
	timer_ = 0.0f;
	isCompleted_ = false;
	currentPhase_ = AnimationPhase::Falling;
	kDeltaTime_ = DeltaTimeSevice::GetDeltaTime();
	// 落下アニメーション情報
	fallInfo_.playerStartPos = { 0.0f, 50.0f, 0.0f };
	fallInfo_.playerEndPos = { 0.0f, 1.0f, 0.0f };
	fallInfo_.FallingDuration = 1.5f;
	// カメラ回転アニメーション情報
	cameraRotInfo_.cameraStartPos = { 0.0f, 5.0f, 15.0f };
	cameraRotInfo_.cameraEndPos = { 0.0f, 70.0f, -60.0f };
	cameraRotInfo_.cameraStartRot = { 0.0f, 0.0f, 0.0f, 1.0f };
	cameraRotInfo_.cameraEndRot = { 0.5f, 0.0f, 0.0f, 1.1f };
	cameraRotInfo_.rotationDuration = 1.5f;
	cameraRotInfo_.rotationStartTime = 1.5f;

	/// ===Playerの初期位置を設定=== ///
	/*if (player_) {
		player_->SetTranslate(playerStartPos_);
	}*/

	/// ===カメラの初期位置を設定=== ///
	if (camera_) {
		camera_->SetTranslate(cameraRotInfo_.cameraStartPos);
		camera_->SetRotate(cameraRotInfo_.cameraStartRot);
	}
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void StartAnimation::Update() {
	// 早期リターン
	if (isCompleted_ && !player_ && !camera_) return;

	/// ===デルタタイム取得=== ///
	kDeltaTime_ = DeltaTimeSevice::GetDeltaTime();

	/// ===タイマーを進める=== ///
	timer_ += kDeltaTime_;

	/// ===フェーズ管理=== ///
	if (timer_ < cameraRotInfo_.rotationStartTime) {
		currentPhase_ = AnimationPhase::Falling;
		//UpdateFalling();
	} else if (timer_ < cameraRotInfo_.rotationStartTime + cameraRotInfo_.rotationDuration) {
		currentPhase_ = AnimationPhase::CameraRotation;
		UpdateCameraRotation();
	} else if (timer_ < totalDuration_) {
		currentPhase_ = AnimationPhase::FinalSetup;
	} else {
		// フラグを立てる
		isCompleted_ = true;
	}
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void StartAnimation::Draw() {}

///-------------------------------------------/// 
/// プレイヤー降下アニメーション
///-------------------------------------------///
void StartAnimation::UpdateFalling() {

	// 正規化された時間（0.0 ~ 1.0）
	float t = timer_ / fallInfo_.FallingDuration;
	t = std::clamp(t, 0.0f, 1.0f);

	// イージング適用
	float easedT = Easing::EaseOutBounce(t);

	// 補間してプレイヤーの位置を更新
	Vector3 currentPos = Math::Lerp(fallInfo_.playerStartPos, fallInfo_.playerEndPos, easedT);
	player_->SetTranslate(currentPos);
}

///-------------------------------------------/// 
/// カメラ回転アニメーション
///-------------------------------------------///
void StartAnimation::UpdateCameraRotation() {

	// 回転フェーズでの経過時間
	float rotationTime = timer_ - cameraRotInfo_.rotationStartTime;
	float t = rotationTime / cameraRotInfo_.rotationDuration;
	t = std::clamp(t, 0.0f, 1.0f);

	// イージング適用
	float easedT = Easing::EaseInOutCubic(t);

	// カメラ位置の補間
	Vector3 midPoint = {
		(cameraRotInfo_.cameraStartPos.x + cameraRotInfo_.cameraEndPos.x) * 0.5f,
		(std::max)(cameraRotInfo_.cameraStartPos.y, cameraRotInfo_.cameraEndPos.y) + 10.0f, // 少し高めに
		(cameraRotInfo_.cameraStartPos.z + cameraRotInfo_.cameraEndPos.z) * 0.5f
	};

	Vector3 currentPos;
	if (easedT < 0.5f) {
		// 開始位置から中間点へ
		float t1 = easedT * 2.0f;
		currentPos = Math::Lerp(cameraRotInfo_.cameraStartPos, midPoint, t1);
	} else {
		// 中間点から終了位置へ
		float t2 = (easedT - 0.5f) * 2.0f;
		currentPos = Math::Lerp(midPoint, cameraRotInfo_.cameraEndPos, t2);
	}

	// カメラ回転の補間
	Quaternion currentRot = Math::SLerp(cameraRotInfo_.cameraStartRot, cameraRotInfo_.cameraEndRot, easedT);

	// カメラ更新
	camera_->SetTranslate(currentPos);
	camera_->SetRotate(currentRot);
}