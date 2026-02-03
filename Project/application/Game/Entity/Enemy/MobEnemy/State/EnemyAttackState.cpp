#include "EnemyAttackState.h"
// MobEnemy
#include "application/Game/Entity/Enemy/MobEnemy/Base/MobEnemy.h"
#include "application/Game/Entity/Enemy/MobEnemy/LongRange/LongRangeEnemy.h"
// Service
#include "Engine/System/Service/ParticleService.h"
// State
#include "EnemyMoveState.h"


///-------------------------------------------/// 
/// 開始時に呼び出す
///-------------------------------------------///
void EnemyAttackState::Enter(MobEnemy* enemy) {
	enemy_ = enemy;
	// 移動量を初期化
	enemy_->SetVelocity({ 0.0f, 0.0f, 0.0f });
	// 攻撃開始処理
	enemy_->StartAttack();
}

///-------------------------------------------/// 
/// 更新時に呼び出す
///-------------------------------------------///
void EnemyAttackState::Update() {

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
