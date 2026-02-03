#pragma once
/// ===Include=== ///
#include "Base/EnemyState.h"
// Math
#include "Math/Vector3.h"

///=====================================================/// 
/// MoveState
///=====================================================///
class EnemyMoveState : public EnemyState {
public:

	~EnemyMoveState() override = default;

	/// <summary>
	/// ステートに入った時に呼ばれる処理
	/// </summary>
	/// <param name="enemy">処理対象の MobEnemy オブジェクトへのポインタ。ステートへ入る対象となる敵を指す</param>
	void Enter(MobEnemy* enemy) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;
};

