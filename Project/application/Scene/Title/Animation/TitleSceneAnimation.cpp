#include "TitleSceneAnimation.h"
// Object
#include "application/Drawing/3d/Object3d.h"
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
void TitleSceneAnimation::Initialize() {
	/// ===乱数エンジンの初期化=== ///
	std::random_device rd;
	randomEngine_.seed(rd());

	/// ===Rotation初期化=== ///
	ringRotations_.assign(2, Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f });

	/// ===Color初期化=== ///
	ringColorFrom_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	ringColorTo_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	ringColorT_ = 1.0f;

	/// ===Scale初期化=== ///
	ringScaleXZFrom_ = 10.0f;
	ringScaleXZTo_ = 10.0f;
	ringScaleT_ = 10.0f;

	// ラジアスブラーの初期設定
	radiusBlurData_ = {
		.center = { 0.5f, 0.5f }, // ブラーの中心を画面中央に設定
		.numSamples = 0,           // サンプリング数を初期化
		.blurWidth = 0.005f,       // ブラーの幅を初期化
	};
}

///-------------------------------------------/// 
/// オブジェクトの回転と色をまとめて更新
///-------------------------------------------///
void TitleSceneAnimation::UpdateObjectsAnimation() {
	const float deltaTime = Service::DeltaTime::GetDeltaTime();

	/// ===Rotation=== ///
	for (size_t i = 0; i < 2; ++i) {
		// リングごとに傾きの方向を変える
		const float sign = (i % 2 == 0) ? 1.0f : -1.0f;
		const Vector3 diagonalAxis = Normalize(Vector3{
			sign * 0.6f,
			1.0f,
			sign * 0.5f + static_cast<float>(i) * 0.3f
			});
		const Quaternion ringRotate = Math::MakeRotateAxisAngle(
			diagonalAxis, sign * (0.6f + static_cast<float>(i) * 0.2f) * deltaTime);
		ringRotations_[i] = Normalize(ringRotate * ringRotations_[i]);
	}

	/// ===Color=== ///
	ringColorT_ += ringColorSpeed_ * deltaTime;
	if (ringColorT_ >= 1.0f) {
		ringColorT_ = 0.0f;
		ringColorFrom_ = ringColorTo_;
		ringColorTo_ = GenerateRandomColor();
	}
	color_ = {
		Math::Lerp(ringColorFrom_.x, ringColorTo_.x, ringColorT_),
		Math::Lerp(ringColorFrom_.y, ringColorTo_.y, ringColorT_),
		Math::Lerp(ringColorFrom_.z, ringColorTo_.z, ringColorT_),
		1.0f
	};

	/// ===Size=== ///
	ringScaleT_ += ringScaleSpeed_ * deltaTime;
	if (ringScaleT_ >= 1.0f) {
		ringScaleT_ = 0.0f;
		ringScaleXZFrom_ = ringScaleXZTo_;
		ringScaleXZTo_ = GenerateRandomScale();
	}
	size_ = Math::Lerp(ringScaleXZFrom_, ringScaleXZTo_, ringScaleT_);
}

///-------------------------------------------/// 
/// ラジアスブラーの更新処理
///-------------------------------------------///
void TitleSceneAnimation::UpdateRadiusBlurAnimation() {
	// サンプリングを50ぐらいまで増やす
	if (radiusBlurData_.numSamples < 80) {
		radiusBlurData_.numSamples += 4; // サンプリング数を増やす
		Service::PostEffect::SetRadiusBlurData(radiusBlurData_); // データをシェーダーに送る
	}
}

///-------------------------------------------/// 
/// ランダムなRGB色（アルファ=1）を生成して返す
///-------------------------------------------///
Vector4 TitleSceneAnimation::GenerateRandomColor() { return { colorDist_(randomEngine_), colorDist_(randomEngine_), colorDist_(randomEngine_), 1.0f }; }

///-------------------------------------------/// 
/// 5〜30 の範囲でランダムなスケール値を生成して返す
///-------------------------------------------///
float TitleSceneAnimation::GenerateRandomScale() { return scaleDist_(randomEngine_); }