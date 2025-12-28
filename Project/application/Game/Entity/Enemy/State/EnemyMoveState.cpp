#include "EnemyMoveState.h"
// BaseEnemy
#include "application/Game/Entity/Enemy/Base/BaseEnemy.h"
// State
#include "EnemyPrePareAttackState.h"


///-------------------------------------------/// 
/// 開始時に呼び出す
///-------------------------------------------///
void EnemyMoveState::Enter(BaseEnemy* enemy) {
	enemy_ = enemy;
	// 移動範囲の中心を設定
	enemy_->SetVelocity({ 0.0f, 0.0f, 0.0f });
	// 動くまでの時間を設定
    enemy_->GetMovementComponent().SetTimer(0.5f);
	// 移動範囲の中心を現在地に設定
    enemy_->GetMovementComponent().SetRangeCenter(enemy_->GetTransform().translate);
}

///-------------------------------------------/// 
/// 更新時に呼び出す
///-------------------------------------------///
void EnemyMoveState::Update(BaseEnemy * enemy) {
	enemy_ = enemy;

    // コンテキストの準備
    EnemyMoveComponent::UpdateContext context{
        .currentPosition = enemy_->GetTransform().translate,
        .deltaTime = enemy_->GetDeltaTime(),
        .isRotationComplete = enemy_->GetIsRotationComplete()
    };

    // 移動コンポーネントの更新
    auto result = enemy_->GetMovementComponent().Update(context);

    // 結果の適用
    enemy_->SetVelocity(result.velocity);

	// 回転の更新
    if (result.needsRotation) {
        auto& config = enemy_->GetMovementComponent().GetConfig();
        enemy_->UpdateRotationTowards(result.targetDirection, config.rotationSpeed);
    }

	// 回転完了フラグのリセット
    if (result.shouldResetRotationFlag) {
        enemy_->SetIsRotationComplete(false);
    }

	/// ===Stateの変更=== ///
    auto& attackComponent = enemy_->GetAttackComponent();
	// 攻撃可能ならAttackStateへ移行
	if (enemy_->CheckAttackable() && attackComponent.GetCooldownTimer() <= 0.0f && !attackComponent.IsAttacking()) {
		// 攻撃準備に
		enemy_->ChangeState(std::make_unique<EnemyPrePareAttackState>());
	}
}

///-------------------------------------------/// 
/// 終了時に呼びだす
///-------------------------------------------///
void EnemyMoveState::Finalize() {
	EnemyState::Finalize();
}