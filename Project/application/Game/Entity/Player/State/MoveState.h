#pragma once
/// ===Include=== ///
#include "Base/PlayerState.h"
// Math
#include "Math/Vector3.h"

///=====================================================/// 
/// MoveState
///=====================================================///
class MoveState : public PlayerState{
public:

	~MoveState() override = default;

	/// <summary>
	/// ステートに入った時に呼ばれる処理
	/// </summary>
	/// <param name="player">処理対象のプレイヤーを指すポインタ。状態遷移時にプレイヤーに対する初期化や設定を行うために使用される。</param>
	/// <param name="camera">状態で使用するゲームカメラを指すポインタ。表示や視点の初期設定に使用される。</param>
	void Enter(Player* player, GameCamera* camera) override;

	/// <summary>
	/// ステート時の更新処理
	/// </summary>
	/// <param name="player">更新対象の Player インスタンスへのポインタ。</param>
	/// <param name="camera">更新処理に使用する GameCamera インスタンスへのポインタ。</param>
	void Update(Player* player, GameCamera* camera) override;

	/// <summary>
	/// ステートの終了処理
	/// </summary>
	void Finalize() override;

private:

	/// ===移動情報=== ///
	struct MoveInfo {
		float speed = 0.5f;
		Vector3 direction = { 0.0f, 0.0f, 0.0f };
	};
	MoveInfo info_;
};

