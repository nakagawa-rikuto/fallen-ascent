#pragma once
/// ===Include=== ///
#include "Math/Vector3.h"
// RadiusBlurEffect
#include "Engine/Graphics/OffScreen/Effect/RadiusBlurEffect.h"

/// ===前方宣言=== ///
namespace MiiEngine {
	class CameraCommon;
}

///=====================================================/// 
/// タイトルシーンのアニメーション管理クラス
///=====================================================///
class TitleSceneAnimation {
public:

	TitleSceneAnimation() = default;
	~TitleSceneAnimation() = default;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(MiiEngine::CameraCommon* camera);

	/// <summary>
	/// カメラの更新処理
	/// </summary>
	void UpdateCameraAnimation(const Vector3& translate);

	/// <summary>
	/// ラジアスブラーの更新処理
	/// </summary>
	void UpdateRadiusBlurAnimation();

public: /// ===Getter=== ///
	/// <summary>
	/// フェードアウト開始フラグを取得します。
	/// </summary>
	/// <returns></returns>
	bool GetStartFadeOut() const;

private:

	/// ===カメラ=== ///
	MiiEngine::CameraCommon* camera_ = nullptr;

	/// ===カメラアニメーション=== ///
	const float cameraAutoRotateSpeed_ = 0.3f; // カメラの自動回転速度
	float cameraAutoRotateAngle_ = 0.0f; // カメラの自動回転角度

	/// ===ラジアスブラーのデータ=== ///
	MiiEngine::RadiusBlurData radiusBlurData_{};
};

