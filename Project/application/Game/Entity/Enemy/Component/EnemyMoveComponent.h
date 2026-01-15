#pragma once
/// ===Include=== ///
#include "Math/Vector3.h"
#include <random>

///=====================================================/// 
/// EnemyMoveComponent
///=====================================================///
class EnemyMoveComponent {
private:
	/// ===状態の構造体=== ///
	struct MoveState {
		float timer = 0.0f;			// タイマー
		Vector3 rangeCenter{};		// 移動範囲の中心
		Vector3 direction{};		// 移動方向
		bool isWaiting = false;		// 待機フラグ	
	};
public:
	/// ===設定パラメータの構造体=== ///
	struct MoveConfig {
		float speed = 0.05f;		// 移動速度
		float range = 20.0f;	    // 移動範囲
		float interval = 5.0f;	    // 移動間隔
		float waitTime = 1.5f;	    // 待機時間
		float rotationSpeed = 0.1f; // 回転速度
	};

	/// ===更新用コンテキスト=== ///
	struct UpdateContext {
		Vector3 currentPosition{};		 // 現在の位置
		float deltaTime = 0.0f;			 // デルタタイム
		bool isRotationComplete = false; // 回転完了フラグ
	};

	/// ===更新結果=== ///
	struct UpdateResult {
		Vector3 velocity{};					   // 移動ベクトル
		Vector3 targetDirection{};			   // 目標方向
		bool needsRotation = false;			   // 回転が必要かどうか
		bool shouldResetRotationFlag = false;  // 回転完了フラグをリセットする必要があるか
	};

public:

	EnemyMoveComponent() = default;
	~EnemyMoveComponent() = default;

	// コピー・ムーブは明示的に制御
	EnemyMoveComponent(const EnemyMoveComponent&) = delete;
	EnemyMoveComponent& operator=(const EnemyMoveComponent&) = delete;

	/// <summary>
	/// 指定した敵オブジェクトを初期化し、中心位置を設定します。
	/// </summary>
	/// <param name="center">初期化に使用する中心位置を示す Vector3 の const 参照。</param>
	void Initialize(const Vector3& center, const MoveConfig& config = MoveConfig{});

	/// <summary>
	/// 更新処理
	/// </summary>
	UpdateResult Update(const UpdateContext& context);

	/// <summary>
	/// ImGui情報の表示
	/// </summary>
	void Information();

	/// <summary>
	/// 指定した中心点を基準に移動を開始します。
	/// </summary>
	/// <param name="center">移動の基準となる中心位置を表す Vector3 型の座標（const 参照）。</param>
	void StartMove(const Vector3& center);

public: /// ===Getter=== ///
	// MoveConfig
	const MoveConfig& GetConfig() const { return config_; }
	// MoveState
	const MoveState& GetState() const { return state_; }

#ifdef USE_IMGUI
public: /// ===Setter=== ///
	// MoveConfig
	void ApplyConfig(const MoveConfig& newconfig);
#endif // USE_IMGUI

private:
	/// ===情報=== ///
	MoveConfig config_;
	MoveState state_;

	// ランダムシード
	std::mt19937 randomEngine_;

private:

	/// <summary>
	/// 方向の設定と待機時間の設定
	/// </summary>
	/// <param name="vector">設定する方向のベクトル</param>
	void PreparNextMove(const Vector3& vector);
};

