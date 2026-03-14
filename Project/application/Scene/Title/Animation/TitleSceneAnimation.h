#pragma once
/// ===Include=== ///
#include "Math/Vector3.h"
// RadiusBlurEffect
#include "Engine/Graphics/OffScreen/Effect/RadiusBlurEffect.h"
// C++
#include <random>
#include <vector>

/// ===前方宣言=== ///
class Object3d;

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
	void Initialize();

	/// <summary>
	/// オブジェクトの回転と色をまとめて更新します。
	/// </summary>
	void UpdateObjectsAnimation();

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

	// TorusLineのデータを取得
	std::vector<Quaternion> GetRotations() const { return ringRotations_; }
	const float GetSize() const { return size_; }
	const Vector4 GetColor() const { return color_; }

private:

	/// ===Rotation=== ///
	std::vector<Quaternion> ringRotations_;                    // Ringごとの現在回転

	/// ===Color=== ///
	// 乱数生成
	std::mt19937 randomEngine_;
	std::uniform_real_distribution<float> colorDist_{ 0.0f, 1.0f };

	Vector4 ringColorFrom_;                  // Ring 補間元の色
	Vector4 ringColorTo_;                    // Ring 補間先の色
	float   ringColorT_;                     // Ring 補間進行度
	float ringColorSpeed_ = 0.4f;    // Ring 色変化速度
	Vector4 color_ = { 0.0f, 0.0f, 0.0f, 1.0f };

	/// ===Scale=== ///
	std::uniform_real_distribution<float> scaleDist_{ 8.0f, 16.0f }; // Ringスケール範囲
	float ringScaleXZFrom_; // Ring XZスケール 補間元
	float ringScaleXZTo_;   // Ring XZスケール 補間先
	float ringScaleT_;      // Ring スケール補間進行度
	float ringScaleSpeed_ = 0.3f;		 // Ring スケール変化速度
	float size_ = 0.0f;

	/// ===ラジアスブラーのデータ=== ///
	MiiEngine::RadiusBlurData radiusBlurData_{};

private:

	/// <summary>
	/// ランダムなRGB色（アルファ=1）を生成して返す
	/// </summary>
	Vector4 GenerateRandomColor();

	/// <summary>
	/// 5〜30 の範囲でランダムなスケール値を生成して返す
	/// </summary>
	float GenerateRandomScale();
};

