#pragma once
/// ===Include=== ///
#include "Math/Vector3.h"

///=====================================================/// 
/// PlayerAvoidanceComponent（回避）
///=====================================================///
class PlayerAvoidanceComponent {
private:
	/// ===状態の構造体=== ///
	struct AvoidanceState {
		float timer = 0.0f;	 // タイマー
		bool isActive = false;      // 回避中フラグ
		bool isPreparation = false; // 準備フラグ
	};
public:
	/// ===回避設定パラメータの構造体=== ///
	struct AvoidanceConfig {
		float speed = 0.4f;           // 回避速度
		float activeTime = 0.3f;     // 回避の有効時間
		float coolTime = 1.0f;       // 回避のクールタイム
		float invincibleTime = 0.01f; // 回避時の無敵時間
	};

	/// ===更新用コンテキスト=== ///
	struct UpdateContext {
		Vector3 inputDirection; // 入力方向
		float deltaTime;        // デルタタイム
	};

	/// ===更新結果=== ///
	struct UpdateResult {
		Vector3 velocity; // 回避ベクトル
	};

public:

	PlayerAvoidanceComponent() = default;
	~PlayerAvoidanceComponent() = default;

	// コピー・ムーブは明示的に制御
	PlayerAvoidanceComponent(const PlayerAvoidanceComponent&) = delete;
	PlayerAvoidanceComponent& operator=(const PlayerAvoidanceComponent&) = delete;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="config">初期化に使用する設定を const 参照で受け取る。省略した場合は AvoidanceConfig{} による既定設定が使用される。</param>
	void Initialize(const AvoidanceConfig& config = AvoidanceConfig{});

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="context">更新用コンテキストを示す UpdateContext の const 参照。</param>
	/// <returns>更新結果を含む UpdateResult 構造体。</returns>
	UpdateResult Update(const UpdateContext& context);

	/// <summary>
	///タイマーの更新処理
	/// </summary>
	/// <param name="deltaTime">前回の更新からの経過時間（秒単位）</param>
	void UpdateTimer(const float deltaTime);

	/// <summary>
	/// ImGui情報の表示
	/// </summary>
	void Information();

	/// <summary>
	/// 回避処理の開始
	/// </summary>
	void StartAvoidance();

	/// <summary>
	/// 状態のリセット
	/// </summary>
	void ResetState();

public: /// ===Getter=== ///

	// AvoidanceConfig
	const AvoidanceConfig& GetConfig() const { return config_; }
	// AvoidanceState
	const AvoidanceState& GetState() const { return state_; }

public: /// ===Setter=== ///
	

private:

	/// ===情報=== ///
	AvoidanceConfig config_;
	AvoidanceState state_;

	// 加速度
	float acceleration_ = 0.0f;
	// 方向
	Vector3 direction_{ 0.0f, 0.0f, 0.0f };
};

