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
};