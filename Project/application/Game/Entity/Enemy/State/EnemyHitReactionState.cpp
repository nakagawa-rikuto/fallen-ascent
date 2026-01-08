#include "EnemyHitReactionState.h"
// BaseEnemy
#include "application/Game/Entity/Enemy/Base/BaseEnemy.h"
// State
#include "EnemyMoveState.h"

///-------------------------------------------/// 
/// 開始時に呼び出す
///-------------------------------------------///
void EnemyHitReactionState::Enter(BaseEnemy* enemy) {
	enemy_ = enemy;
}

///-------------------------------------------/// 
/// 更新時に呼び出す
///-------------------------------------------///
void EnemyHitReactionState::Update(BaseEnemy * enemy) {
	enemy_ = enemy;

	/// ===HitReactionComponentの更新=== ///
	// 更新コンテキストの作成
	EnemyHitReactionComponent::UpdateContext hitContext{
		.currentPosition = enemy_->GetTransform().translate,
		.currentVelocity = enemy_->GetVelocity(),
		.deltaTime = enemy_->GetDeltaTime()
	};

	// コンポーネントの更新
	auto result = enemy_->GetHitReactionComponent().Update(hitContext);

	// 結果の反映
	enemy_->SetVelocity(result.velocity);

	/// ===ヒットリアクションが終了したらMoveStateへ遷移=== ///
	// 減速も終了したら移動状態へ
	const auto& state = enemy_->GetHitReactionComponent().GetState();
	if (state.slowdownTimer <= 0.0f) {
		enemy_->ChangeState(std::make_unique<EnemyMoveState>());
	}
}

///-------------------------------------------/// 
/// 終了時に呼びだす
///-------------------------------------------///
void EnemyHitReactionState::Finalize() {
	EnemyState::Finalize();
}
