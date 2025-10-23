/// ===Include=== ///
#include "Base/PlayerState.h"
// Math
#include "Math/Vector3.h"

///=====================================================/// 
/// AttackState
///=====================================================///
class AttackState : public PlayerState {
public:

	~AttackState() override = default;

	/// <summary>
	/// ステートに入った時に呼ばれる処理
	/// </summary>
	/// <param name="player">状態に入る対象のプレイヤーを指すポインタ。</param>
	/// <param name="camera">状態で使用するゲームカメラを指すポインタ。</param>
	void Enter(Player* player, GameCamera* camera) override;

	/// <summary>
	/// ステート時の更新処理
	/// </summary>
	/// <param name="player">更新対象の Player オブジェクトへのポインタ。nullptr の扱いは実装に依存する。</param>
	/// <param name="camera">更新処理で参照する GameCamera オブジェクトへのポインタ。</param>
	void Update(Player* player, GameCamera* camera) override;

	/// <summary>
	/// ステートの終了処理
	/// </summary>
	void Finalize() override;

private:

	/// ===攻撃タイプ=== ///
	enum class AttackType {
		kCombo1,	// コンボ1段目
		kCombo2,	// コンボ2段目
	};

	/// ===攻撃情報=== ///
	struct AttackInfo {
		AttackType currentAttack = AttackType::kCombo1; // 現在の攻撃タイプ

		float activTime = 0.0f;			// 攻撃のアクティブ時間
		float comboWindowTime = 0.5f;	// コンボ受付時間
		float comboTimer = 0.0f;		// コンボ受付タイマー

		bool isAttacking = false;		// 攻撃中フラグ
		bool canCombo = false;			// コンボ可能フラグ
		bool nextComboRequest = false;	// 次のコンボ入力フラグ

		// 各攻撃の接続時間
		float combo1Duration = 0.4f;	// コンボ1段目の持続時間
		float combo2Duration = 00.6f;	// コンボ2段目の持続時間

		// クールタイム
		float coolTime = 0.3f;			// 攻撃後のクールタイム
	};
	AttackInfo attackInfo_;

private:
	/// <summary>
	/// 攻撃の初期化処理
	/// </summary>
	/// <param name="type">初期化する攻撃の種類を表す AttackType 値。</param>
	void InitializeAttack(AttackType type);

	/// <summary>
	/// コンポ入力のチェック処理
	/// </summary>
	void CheckComboInput();

	/// <summary>
	/// 次の攻撃への遷移処理
	/// </summary>
	void TransitionToNextCombo();
};