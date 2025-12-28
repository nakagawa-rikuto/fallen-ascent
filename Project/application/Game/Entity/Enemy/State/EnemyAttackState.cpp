#include "EnemyAttackState.h"
// BaseEnemy
#include "application/Game/Entity/Enemy/Base/BaseEnemy.h"
#include "application/Game/Entity/Player/Player.h"
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
	// 攻撃コンポーネントを開始
	enemy_->GetAttackComponent().StartAttack();
	// 色の設定
	enemy_->SetColor({ 0.0f, 1.0f, 0.0f, 1.0f });
	// 
	targetPos_ = enemy_->GetPlayer()->GetTransform().translate;
}

///-------------------------------------------/// 
/// 更新時に呼び出す
///-------------------------------------------///
void EnemyAttackState::Update(BaseEnemy * enemy) {
	enemy_ = enemy;

	/// ===攻撃コンポーネントの更新=== ///
	Vector3 enemyPos = enemy_->GetTransform().translate;
	float yaw = enemy_->GetTransform().rotate.y;
	Vector3 direction = { std::sinf(yaw), 0.0f, std::cosf(yaw) };

	EnemyAttackComponent::UpdateContext context{
		.currentPosition = enemyPos,
		.currentDirection = direction,
		.targetPosition = targetPos_,
		.deltaTime = enemy_->GetDeltaTime()
	};

	// 更新処理
	auto result = enemy_->GetAttackComponent().Update(context);

	// 移動ベクトルの適用
	if (result.isAttacking) {
		enemy_->SetVelocity(result.velocity);
	}

	if (result.isAttackComplete) {
		enemy_->SetColor({ 1.0f, 0.0f, 1.0f, 1.0f });
		enemy_->ChangeState(std::make_unique<EnemyMoveState>());
	}
}

///-------------------------------------------/// 
/// 終了時に呼び出す
///-------------------------------------------///
void EnemyAttackState::Finalize() {
	EnemyState::Finalize();
}
