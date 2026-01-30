#include "EnemyAttackState.h"
// BaseEnemy
#include "application/Game/Entity/Enemy/Base/BaseEnemy.h"
#include "application/Game/Entity/Enemy/LongRange/LongRangeEnemy.h"
// Service
#include "Engine/System/Service/ParticleService.h"
// State
#include "EnemyMoveState.h"


///-------------------------------------------/// 
/// 開始時に呼び出す
///-------------------------------------------///
void EnemyAttackState::Enter(BaseEnemy* enemy) {
	enemy_ = enemy;
	// 移動量を初期化
	enemy_->SetVelocity({ 0.0f, 0.0f, 0.0f });
	// 攻撃開始処理
	enemy_->StartAttack();
}

///-------------------------------------------/// 
/// 更新時に呼び出す
///-------------------------------------------///
void EnemyAttackState::Update(BaseEnemy * enemy) {
	enemy_ = enemy;

	// Attackの処理
	enemy_->Attack();

	/// ===Stateの変更=== ///
	if (!enemy_->GetAttackInfo().isAttack) {
		// MoveStateに移行
		enemy_->ChangeState(std::make_unique<EnemyMoveState>());
		return;
	}
}

///-------------------------------------------/// 
/// 終了時に呼び出す
///-------------------------------------------///
void EnemyAttackState::Finalize() {
	EnemyState::Finalize();
}
