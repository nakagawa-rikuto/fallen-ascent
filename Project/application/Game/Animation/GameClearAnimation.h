#pragma once
/// ===Include=== ///
#include "Engine/DataInfo/CData.h"

/// ===前方宣言=== ///
class Player;
class GameCamera;

/// ===アニメーションフェーズ=== ///
enum class ClearAnimationPhase {
	CameraRotation,  // カメラ回転フェーズ
	FinalJump,       // 最終ジャンプフェーズ
	Completed        // 完了
};

///=====================================================/// 
/// ゲームクリアアニメーションクラス
///=====================================================///
class GameClearAnimation {
public:
	GameClearAnimation() = default;
	~GameClearAnimation() = default;

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

	/// <summary>
	/// アニメーション完了フラグを取得
	/// </summary>
	/// <returns>完了していればtrue</returns>
	bool IsCompleted() const;

private:
	/// ===メンバ変数=== ///
	Player* player_ = nullptr;      // プレイヤーへの参照
	GameCamera* camera_ = nullptr;  // カメラへの参照

	ClearAnimationPhase currentPhase_ = ClearAnimationPhase::CameraRotation;

	/// ===カメラ回転用=== ///
	float cameraRotationTimer_ = 0.0f;      // カメラ回転タイマー
	float cameraRotationDuration_ = 3.0f;   // カメラ回転時間
	float startAngle_ = 0.0f;                // 初期角度（ラジアン）
	float targetAngle_ = 0.0f;               // 目標角度（ラジアン）
	float cameraHeight_ = 0.120f;            // カメラの高さ（補間用）
	float cameraDistance_ = 94.0f;           // カメラとプレイヤーの距離（補間用）
	Quaternion startCameraRotation_;         // 初期カメラ回転
	Quaternion targetCameraRotation_;        // 目標カメラ回転

	/// ===プレイヤージャンプ用（カメラ回転中）=== ///
	float jumpTimer_ = 0.0f;                 // ジャンプタイマー
	float jumpInterval_ = 0.5f;              // ジャンプの間隔
	Vector3 basePlayerPosition_;             // プレイヤーの基準位置
	float smallJumpHeight_ = 1.5f;           // 小ジャンプの高さ
	int jumpCount_ = 0;                      // ジャンプ回数

	/// ===最終ジャンプ用=== ///
	float finalJumpTimer_ = 0.0f;            // 最終ジャンプタイマー
	float finalJumpDuration_ = 1.0f;         // 最終ジャンプ時間
	float finalJumpHeight_ = 5.0f;           // 最終ジャンプの高さ

private:
	/// <summary>
	/// カメラ回転フェーズの更新
	/// </summary>
	void UpdateCameraRotation();

	/// <summary>
	/// 最終ジャンプフェーズの更新
	/// </summary>
	void UpdateFinalJump();

	/// <summary>
	/// プレイヤーの小ジャンプ処理
	/// </summary>
	void UpdatePlayerSmallJump();
};