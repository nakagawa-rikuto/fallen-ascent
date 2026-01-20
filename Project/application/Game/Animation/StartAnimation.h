#pragma once
/// ===Include=== ///
#include "Engine/DataInfo/CData.h"

/// ===前方宣言=== ///
class Player;
class GameCamera;

///=====================================================/// 
/// StartAnimation
///=====================================================///
class StartAnimation {
public:
	StartAnimation() = default;
	~StartAnimation();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="player">プレイヤーのポインタ</param>
	/// <param name="camera">カメラのポインタ</param>
	void Initialize(Player* player, GameCamera* camera);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

public: /// ===Getter=== ///
	/// <summary>
	/// アニメーション完了フラグを取得
	/// </summary>
	bool IsCompleted() const { return isCompleted_; }

private:
	/// ===ポインタ=== ///
	Player* player_ = nullptr;
	GameCamera* camera_ = nullptr;

	/// ===アニメーション制御=== ///
	float timer_ = 0.0f;              // アニメーション用タイマー
	float totalDuration_ = 3.5f;      // 全体の長さ
	bool isCompleted_ = false;        // 完了フラグ

	/// ===フェーズ管理=== ///
	enum class AnimationPhase {
		Falling,		 // プレイヤー降下フェーズ
		CameraRotation,  // カメラ回転フェーズ
		FinalSetup       // 最終調整フェーズ
	};
	AnimationPhase currentPhase_ = AnimationPhase::Falling;

	/// ===落下アニメーション情報=== ///
	struct FallingAnimationInfo {
		Vector3 playerStartPos;  // 開始位置
		Vector3 playerEndPos;    // 終了位置
		float FallingDuration;   // 降下時間
	};
	FallingAnimationInfo fallInfo_;

	/// ===カメラ回転アニメーション情報=== ///
	struct CameraRotationAnimationInfo {
		Vector3 cameraStartPos;      // 開始位置
		Vector3 cameraEndPos;        // 終了位置
		Quaternion cameraStartRot;   // 開始回転
		Quaternion cameraEndRot;     // 終了回転
		float rotationDuration;      // 回転時間
		float rotationStartTime;     // 回転開始時刻
	};
	CameraRotationAnimationInfo cameraRotInfo_;

	/// ===デルタタイム=== ///
	float kDeltaTime_ = 0.0f;

private:

	/// <summary>
	/// プレイヤーの降下アニメーション更新処理
	/// </summary>
	void UpdateFalling();

	/// <summary>
	/// カメラの回転アニメーション更新処理
	/// </summary>
	void UpdateCameraRotation();
};