#pragma once
/// ===Include=== ///
#include "Base/EnemyState.h"
#include "Math/Vector3.h"

/// ===前方宣言=== ///
namespace MiiEngine {
	class ParticleGroup;
}

///-------------------------------------------/// 
/// PrePareAttackState
///-------------------------------------------///
class EnemyPrePareAttackState : public EnemyState {
public:

	~EnemyPrePareAttackState() override = default;

	/// <summary>
	/// ステートに入った時に呼ばれる処理
	/// </summary>
	/// <param name="enemy">処理対象の BaseEnemy オブジェクトへのポインタ。ステートへ入る対象となる敵を指す</param>
	void Enter(MobEnemy* enemy) override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

private:
	MiiEngine::ParticleGroup* activeParticle_ = nullptr;
	MiiEngine::ParticleGroup* prePareAttackParticle_ = nullptr;

	/// ===タイマー=== ///
	float activeTimer_ = 0.0f;	// アクティブタイマー
	float activeTime = 1.6f;	// アクティブ時間

	/// ===攻撃方向関連=== /
	Vector3 attackDirection_ = { 0.0f, 0.0f, 0.0f }; // 攻撃方向
	Vector3 playerPos_ = { 0.0f, 0.0f, 0.0f }; 		 // プレイヤーの位置
	
	// 回転速度
	float rotationSpeed_ = 0.2f;
};

