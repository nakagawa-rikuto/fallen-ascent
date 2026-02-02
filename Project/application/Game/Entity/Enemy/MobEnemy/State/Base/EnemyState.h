#pragma once
/// ===前方宣言=== ///
class MobEnemy;

///=====================================================/// 
/// エネミーのベースステート
///=====================================================///
class EnemyState {
public:
	virtual ~EnemyState() = default;

	/// <summary>
	/// ステートに入った時に呼ばれる処理、純粋仮想関数
	/// </summary>
	/// <param name="enemy">処理対象の BaseEnemy オブジェクトへのポインタ。ステートへ入る対象となる敵を指す</param>
	virtual void Enter(MobEnemy* enemy) = 0;

	/// <summary>
	/// 更新処理、純粋仮想関数
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize();

protected:
	// MobEnemyのポインター
	MobEnemy* enemy_ = nullptr;
};

