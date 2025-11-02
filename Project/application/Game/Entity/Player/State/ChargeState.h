/// ===Include=== ///
#include "Base/PlayerState.h"
// Math
#include "Math/Vector3.h"

///=====================================================/// 
/// ChargeState
///=====================================================///
class ChargeState : public PlayerState {
public:

	~ChargeState() override = default;

	/// <summary>
	/// ステートに入った時に呼ばれる処理
	/// </summary>
	/// <param name="player">操作対象または影響を受けるプレイヤーを指すポインタ。入力やプレイヤー固有の初期化に使用される。</param>
	/// <param name="camera">現在のゲームカメラを指すポインタ。表示や視点の設定・初期化に使用される。</param>
	void Enter(Player* player, GameCamera* camera) override;

	/// <summary>
	/// ステート時の更新処理
	/// </summary>
	/// <param name="player">更新処理で使用するプレイヤーを指すポインタ。</param>
	/// <param name="camera">更新処理で使用するゲームカメラを指すポインタ。</param>
	void Update(Player* player, GameCamera* camera) override;

	/// <summary>
	/// ステートの終了処理
	/// </summary>
	void Finalize() override;

private:

	/// ===チャージフェーズ=== ///
	enum class ChargePhase {
		kCharging,  // チャージ中
		kAttacking  // 攻撃中
	};

	/// ===チャージ情報=== ///
	struct ChargeInfo {
		ChargePhase currentPhase = ChargePhase::kCharging; // 現在のフェーズ

		float chargeTimer = 0.0f;      // チャージタイマー
		float chargeMaxTime = 2.0f;    // 最大チャージ時間
		float chargePower = 0.0f;      // チャージ力（0.0～1.0）

		float attackDuration = 0.6f;   // 攻撃の持続時間
		float coolTime = 0.5f;         // 攻撃後のクールタイム

		// 攻撃の設定パラメータ
		const float weaponLength = 8.0f; // 武器までの距離
		const float minDamage = 1.0f;    // 最小ダメージ
		const float maxDamage = 5.0f;    // 最大ダメージ（フルチャージ時）
	};
	ChargeInfo chargeInfo_;

private:
	/// <summary>
	/// チャージフェーズの処理
	/// </summary>
	void UpdateCharging();

	/// <summary>
	/// 攻撃フェーズの処理
	/// </summary>
	void UpdateAttacking();

	/// <summary>
	/// チャージ攻撃を開始する
	/// </summary>
	void StartChargeAttack();
};