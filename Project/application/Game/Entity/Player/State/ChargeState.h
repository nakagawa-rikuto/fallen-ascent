/// ===Include=== ///
#include "Base/PlayerState.h"
// Math
#include "Math/Vector3.h"

///=====================================================/// 
/// ChargeState
///=====================================================///
class ChargeState : public PlayerState {
public:
	ChargeState(const Vector3& direction);
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

	/// ===突進情報=== ///
	struct ChargeInfo {
		float speed = 0.0f;			  // 突進時の移動速度
		float activeTime = 0.15f;      // 突進の有効時間
		float cooltime = 0.8f;		  // 突進のクールタイム
		float invincibleTime = 0.1f;  // 突進時の無敵時間
		float acceleration = 0.0f;    // 突進の加速度
		Vector3 direction = { 0.0f, 0.0f, 0.0f };
	};
	ChargeInfo info_;
};