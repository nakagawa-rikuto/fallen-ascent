#pragma once
/// ===Include=== ///
#include "Base/PlayerState.h"
// Math
#include "Math/Vector3.h"

///=====================================================/// 
/// AvoidanceState
///=====================================================///
class AvoidanceState : public PlayerState {
public:
	AvoidanceState(const Vector3& direction);
	~AvoidanceState() override = default;

	/// <summary>
	/// ステートに入った時に呼ばれる処理
	/// </summary>
	/// <param name="player">処理対象のプレイヤーを指すポインタ。</param>
	/// <param name="camera">現在のゲームカメラを指すポインタ。エントリ処理でカメラの状態や視点を使用する可能性がある。</param>
	void Enter(Player* player, GameCamera* camera) override;

	/// <summary>
	/// ステート時の更新処理
	/// </summary>
	/// <param name="player">更新対象のプレイヤーを指すポインタ。</param>
	/// <param name="camera">更新処理に使用するゲームカメラを指すポインタ。</param>
	void Update(Player* player, GameCamera* camera) override;

	/// <summary>
	/// ステートの終了処理
	/// </summary>
	void Finalize() override;

private:

	/// ===回避情報=== ///
	struct AvoidanceInfo {
		float speed = 0.0f;			  // 回避時の移動速度
		float activeTime = 0.3f;      // 回避の有効時間
		float cooltime = 1.0f;		  // 回避のクールタイム
		float invincibleTime = 0.3f;  // 回避時の無敵時間
		float acceleration = 0.0f;    // 回避の加速度
		Vector3 direction = { 0.0f, 0.0f, 0.0f };
	};
	AvoidanceInfo info_;
};

