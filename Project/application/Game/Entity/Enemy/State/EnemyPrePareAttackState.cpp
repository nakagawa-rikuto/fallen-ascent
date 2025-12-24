#include "EnemyPrePareAttackState.h"
// BaseEnemy
#include "application/Game/Entity/Enemy/Base/BaseEnemy.h"

///-------------------------------------------/// 
/// ステートに入った時に呼ばれる処理
///-------------------------------------------///
void EnemyPrePareAttackState::Enter(BaseEnemy* enemy) {
	enemy_ = enemy;
	// 移動量を初期化
	enemy_->SetVelocity({ 0.0f, 0.0f, 0.0f });

}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void EnemyPrePareAttackState::Update(BaseEnemy* enemy) {
	enemy;
}

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void EnemyPrePareAttackState::Finalize() {}
