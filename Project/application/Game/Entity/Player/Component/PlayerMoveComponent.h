#pragma once
/// ===Include=== ///
#include "Math/Vector3.h"
#include "Math/Quaternion.h"

///=====================================================/// 
/// PlayerMoveComponent（移動）
///=====================================================///
class PlayerMoveComponent {
public:
	/// ===移動設定パラメータの構造体=== ///
	struct MoveConfig {
		float speed = 0.5f;			 // 移動速度
		float rotationSpeed = 0.15f; // 回転速度
		float deceleration = 0.95f;   // 減速率
	};

	/// ===更新用コンテキスト=== ///
	struct UpdateContext {
		Vector3 inputDirection;		// 入力方向
		Vector3 currentPosition;	// 現在の位置
		Quaternion currentRotation; // 現在の回転
		float deltaTime;			// デルタタイム
	};

	/// ===更新結果=== ///
	struct UpdateResult {
		Vector3 velocity;		    // 移動ベクトル
		Quaternion targetRotation; // 目標回転
	};

public:

	PlayerMoveComponent() = default;
	~PlayerMoveComponent() = default;

	// コピー・ムーブは明示的に制御
	PlayerMoveComponent(const PlayerMoveComponent&) = delete;
	PlayerMoveComponent& operator=(const PlayerMoveComponent&) = delete;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="config">初期化に使用する設定を const 参照で受け取る。省略した場合は MoveConfig{} による既定設定が使用される。</param>
	void Initialize(const MoveConfig& config = MoveConfig{});

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="context">更新用コンテキストを示す UpdateContext の const 参照。</param>
	/// <returns>更新結果を含む UpdateResult 構造体。</returns>
	UpdateResult Update(const UpdateContext& contex);

	/// <summary>
	/// ImGui情報の表示
	/// </summary>
	void Information();

public: /// ===Getter=== ///
	// MoveConfig
	const MoveConfig& GetConfig() const { return config_; }
	// CurrentDirection
	const Vector3& GetCurrentDirection() const { return currentDirection_; }

private:

	/// ===情報=== ///
	MoveConfig config_;
	// 現在の移動方向
	Vector3 currentDirection_ = { 0.0f, 0.0f, 0.0f };
};

