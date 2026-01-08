#pragma once
/// ===Include=== ///
#include "Math/Vector3.h"
#include "Math/Vector4.h"

///=====================================================/// 
/// EnemyHitReactionComponent
///=====================================================///
class EnemyHitReactionComponent {
private:
	/// ===状態の構造体=== ///
	struct KnockBackState {
		float slowdownTimer = 0.0f;		 // 減速用タイマー
		float slowdownMultiplier = 1.0f; // 減速率の乗数
		bool isHit = false;				 // ヒットしたかどうか
	};

public:
	/// ===設定パラメータの構造体=== ///
	struct KnockBackConfig {
		float knockBackForce = 1.5f;	// ノックバックの強さ
		float slowdownFactor = 0.3f;	// 減速率(0.0~1.0、小さいほど遅くなる)
		float slowdownDuration = 0.2f;	// 減速効果の持続時間
	};

	/// ===更新用コンテキスト=== ///
	struct UpdateContext {
		Vector3 currentPosition;	// 現在の位置
		Vector3 currentVelocity;	// 現在の速度
		float deltaTime;
	};

	/// ===更新結果=== ///
	struct UpdateResult {
		Vector3 velocity = { 0.0f, 0.0f, 0.0f };
	};

public:

	EnemyHitReactionComponent() = default;
	~EnemyHitReactionComponent() = default;

	// コピー・ムーブは明示的に制御
	EnemyHitReactionComponent(const EnemyHitReactionComponent&) = delete;
	EnemyHitReactionComponent& operator=(const EnemyHitReactionComponent&) = delete;

	/// <summary>
	/// KnockBack の初期化を行います。
	/// </summary>
	/// <param name="config">初期化に使用する設定を const 参照で受け取ります。引数を省略した場合は KnockBackConfig の既定値が使用されます。</param>
	void Initialize(const KnockBackConfig& config = KnockBackConfig{});

	/// <summary>
	/// 与えられた更新コンテキストに基づいて更新処理を行い、その結果を返します。
	/// </summary>
	/// <param name="context">更新に必要な情報を保持する入力コンテキスト。参照で渡され、関数はこの情報を用いて更新を実行します。</param>
	/// <returns>更新処理の結果を表す UpdateResult。成功／失敗や状態に関する情報を含みます。</returns>
	UpdateResult Update(const UpdateContext& context);

	/// <summary>
	/// ImGui情報の表示
	/// </summary>
	void Information();

	/// <summary>
	/// オブジェクトがヒットされたときに呼び出されるコールバック。ヒットの方向を受け取ります。
	/// </summary>
	/// <param name="hitParentDirection">親座標系（親オブジェクト基準）で表されたヒットの方向を示すVector3へのconst参照。関数内で値は変更されません。</param>
	void OnHit(const Vector3& hitParentDirection);

public: /// ===Getter=== ///
	// KnockBackConfig
	const KnockBackConfig& GetConfig() const { return config_; }
	// KnockBackState
	const KnockBackState& GetState() const { return state_; }

#ifdef USE_IMGUI
public: /// ===Setter=== ///
	// KnockBackConfig
	void ApplyConfig(const KnockBackConfig& newConfig);
#endif // USE_IMGUI

private:
	/// ===情報=== ///
	KnockBackConfig config_;
	KnockBackState state_;

	// ヒットした相手の方向
	Vector3 hitParentDirection_ = {0.0f, 0.0f, 0.0f};

private:

	/// <summary>
	/// タイマーの更新処理
	/// </summary>
	/// <param name="deltaTime">デルタタイム</param>
	void TimerUpdate(const float deltaTime);

	/// <summary>
	/// 減速係数の更新処理
	/// </summary>
	void UpdateSlowdownMultiplier();
};

