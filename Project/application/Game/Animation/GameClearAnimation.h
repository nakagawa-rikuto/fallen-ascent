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

	// 現在のフェーズ
	ClearAnimationPhase currentPhase_ = ClearAnimationPhase::CameraRotation;

	/// ===カメラ回転用=== ///
	struct CameraRotationInfo {
		float timer = 0.0f;			  // カメラ回転タイマー
		float duration = 3.0f;		  // カメラ回転時間
		float startAngle = 0.0f;      // 初期角度（ラジアン）
		float targetAngle = 0.0f;     // 目標角度（ラジアン）
		float height = 0.120f;		  // カメラの高さ（補間用）
		float distance = 94.0f;		  // カメラとプレイヤーの距離（補間用）
		Quaternion startRotation;     // 初期カメラ回転
		Quaternion targetRotation;    // 目標カメラ回転
		Vector3 playerForward;		  // プレイヤーの前方向ベクトル
	};
	CameraRotationInfo cameraInfo_;

	/// ===プレイヤージャンプ用=== ///
	struct PlayerSmallJumpInfo {
		float timer = 0.0f;           // ジャンプタイマー
		float interval = 0.5f;        // ジャンプの間隔
		Vector3 basePlayerPosition;   // プレイヤーの基準位置
		float height = 6.0f;          // 小ジャンプの高さ
		int count = 0;                // ジャンプ回数
	};
	PlayerSmallJumpInfo smallJumpInfo_;

	/// ===最終ジャンプ用=== ///
	struct FinalJumpInfo {
		float timer = 0.0f;      // 最終ジャンプタイマー
		float duration = 1.0f;   // 最終ジャンプ時間
		float height = 7.0f;     // 最終ジャンプの高さ
	};
	FinalJumpInfo finalJumpInfo_;

	float deltaTime_ = 0.0f; // deltaTime

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