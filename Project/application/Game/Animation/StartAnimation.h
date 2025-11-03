#pragma once
/// ===Include=== ///
#include "Engine/DataInfo/CData.h"
#include <functional>

/// ===前方宣言=== ///
class Player;
class GameCamera;

///=====================================================/// 
/// StartAnimation
///=====================================================///
class StartAnimation {
public:
	StartAnimation();
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
	float totalDuration_ = 3.5f;      // 全体の長さ（秒）
	bool isCompleted_ = false;        // 完了フラグ

	/// ===フェーズ管理=== ///
	enum class AnimationPhase {
		PlayerDescent,   // プレイヤー降下フェーズ（0.0 ~ 1.5秒）
		CameraRotation,  // カメラ回転フェーズ（1.5 ~ 3.0秒）
		FinalSetup       // 最終調整フェーズ（3.0 ~ 3.5秒）
	};
	AnimationPhase currentPhase_ = AnimationPhase::PlayerDescent;

	/// ===プレイヤー関連=== ///
	Vector3 playerStartPos_ = { 0.0f, 50.0f, 0.0f };  // 開始位置（上空）
	Vector3 playerEndPos_ = { 0.0f, 1.0f, 0.0f };     // 終了位置（地面）
	float descentDuration_ = 1.5f;                     // 降下時間

	/// ===カメラ関連=== ///
	Vector3 cameraStartPos_ = { 0.0f, 5.0f, 15.0f };  // 開始位置（前方）
	Vector3 cameraEndPos_ = { 0.0f, 70.0f, -60.0f };  // 終了位置（後方上空）
	Quaternion cameraStartRot_ = { 0.0f, 0.0f, 0.0f, 1.0f };  // 開始回転
	Quaternion cameraEndRot_ = { 0.5f, 0.0f, 0.0f, 1.1f }; // 終了回転
	float rotationDuration_ = 1.5f;                    // 回転時間
	float rotationStartTime_ = 1.5f;                   // 回転開始時刻

	/// ===デルタタイム=== ///
	float kDeltaTime_;

private:

	/// <summary>
	/// プレイヤーの降下アニメーション更新処理
	/// </summary>
	void UpdatePlayerDescent();

	/// <summary>
	/// カメラの回転アニメーション更新処理
	/// </summary>
	void UpdateCameraRotation();

	/// <summary>
	/// アニメーション完了処理
	/// </summary>
	void CompleteAnimation();
};