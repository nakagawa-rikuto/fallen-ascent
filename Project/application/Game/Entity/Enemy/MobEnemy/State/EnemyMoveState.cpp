#include "EnemyMoveState.h"
// MobEnemy
#include "application/Game/Entity/Enemy/MobEnemy/Base/MobEnemy.h"
// State
#include "EnemyPrePareAttackState.h"


///-------------------------------------------/// 
/// 開始時に呼び出す
///-------------------------------------------///
void EnemyMoveState::Enter(MobEnemy* enemy) {
	enemy_ = enemy;
	// 移動範囲の中心を設定
	enemy_->SetVelocity({ 0.0f, 0.0f, 0.0f });
    // 移動開始処理
    enemy_->GetMovementComponent().StartMove(enemy_->GetTransform().translate);
}

///-------------------------------------------/// 
/// 更新時に呼び出す
///-------------------------------------------///
void EnemyMoveState::Update() {
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
        float rotationSpeed = enemy_->GetMovementComponent().GetConfig().rotationSpeed;
        enemy_->UpdateRotationTowards(result.targetDirection, rotationSpeed);
    }

	// 回転完了フラグのリセット
    if (result.shouldResetRotationFlag) {
        enemy_->SetIsRotationComplete(false);
    }

	/// ===Stateの変更=== ///
	if (enemy_->CheckAttackable() && enemy_->GetAttackInfo().timer <= 0.0f && !enemy_->GetAttackInfo().isAttack) {
		// Attackに
		enemy_->ChangeState(std::make_unique<EnemyPrePareAttackState>());
	}
}

///-------------------------------------------/// 
/// 終了時に呼びだす
///-------------------------------------------///
void EnemyMoveState::Finalize() {
	EnemyState::Finalize();
}