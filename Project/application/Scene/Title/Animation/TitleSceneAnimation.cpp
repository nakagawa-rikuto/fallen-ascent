#include "TitleSceneAnimation.h"
// GameCamera
#include "application/Game/Camera/GameCamera.h"
// Service
#include "Service/DeltaTime.h"
#include "Service/PostEffect.h"
// Math
#include "Math/sMath.h"

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
bool TitleSceneAnimation::GetStartFadeOut() const {
	if (radiusBlurData_.numSamples > 25) {
		return true;
	} else {
		return false;
	}
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void TitleSceneAnimation::Initialize(GameCamera* camera) {
	// カメラの設定
	camera_ = camera;

	// ラジアスブラーの初期設定
	radiusBlurData_ = {
		.center = { 0.5f, 0.7f }, // ブラーの中心を画面中央に設定
		.numSamples = 0,           // サンプリング数を初期化
		.blurWidth = 0.005f,       // ブラーの幅を初期化
	};
}

///-------------------------------------------/// 
/// カメラの更新処理
///-------------------------------------------///
void TitleSceneAnimation::UpdateCameraAnimation(const Vector3& translate) {
	// 回転角度の更新
	float yawAngle = cameraAutoRotateSpeed_ * Service::DeltaTime::GetDeltaTime(); // Y軸回転の角度を計算

	// ターゲット位置からカメラの位置を計算
	Vector3 toCamera = camera_->GetTranslate() - translate; // ターゲットからカメラへのベクトル

	// Yaw回転（ワールドのY軸周り）
	Quaternion yawRotation = Math::MakeRotateAxisAngle(Vector3(0, 1, 0), yawAngle);

	// Pitch回転（カメラの右方向軸周り）
	Vector3 rightAxis = Math::RotateVector(Vector3(1, 0, 0), camera_->GetRotate());

	// 回転を適用
	Quaternion totalRotation = yawRotation;
	camera_->SetRotate(Normalize(totalRotation * camera_->GetRotate()));

	// ターゲットを中心にカメラ位置を回転
	Vector3 newOffset = Math::RotateVector(toCamera, totalRotation);
	camera_->SetTranslate(translate + newOffset);
}

///-------------------------------------------/// 
/// ラジアスブラーの更新処理
///-------------------------------------------///
void TitleSceneAnimation::UpdateRadiusBlurAnimation() {
	// サンプリングを50ぐらいまで増やす
	if (radiusBlurData_.numSamples < 50) {
		radiusBlurData_.numSamples += 1; // サンプリング数を増やす
		Service::PostEffect::SetRadiusBlurData(radiusBlurData_); // データをシェーダーに送る
	}
}
