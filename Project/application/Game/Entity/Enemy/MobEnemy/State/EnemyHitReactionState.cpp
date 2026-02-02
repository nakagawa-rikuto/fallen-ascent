#include "EnemyHitReactionState.h"
// MobEnemy
#include "application/Game/Entity/Enemy/MobEnemy/Base/MobEnemy.h"
// State
#include "EnemyMoveState.h"

///-------------------------------------------/// 
/// 開始時に呼び出す
///-------------------------------------------///
void EnemyHitReactionState::Enter(MobEnemy* enemy) {
	enemy_ = enemy;
}

///-------------------------------------------/// 
/// 更新時に呼び出す
///-------------------------------------------///
void EnemyHitReactionState::Update() {

	/// ===HitReactionComponentの更新=== ///
	// 更新コンテキストの作成
	EnemyHitReactionComponent::UpdateContext hitContext{
		.currentPosition = enemy_->GetTransform().translate,
		.currentVelocity = enemy_->GetVelocity(),
		.currentColor = enemy_->GetColor(),
		.deltaTime = enemy_->GetDeltaTime()
	};

	// コンポーネントの更新
	auto result = enemy_->GetHitReactionComponent().Update(hitContext);

	// 結果の反映
	enemy_->SetVelocity(result.velocity);
	enemy_->SetColor(result.color);

	/// ===ヒットリアクションが終了したらMoveStateへ遷移=== ///
	float slowdownTimer = enemy_->GetHitReactionComponent().GetKnockBackState().slowdownTimer;
	float colorTimer = enemy_->GetHitReactionComponent().GetColorState().colorTimer;
	if (slowdownTimer <= 0.0f && colorTimer <= 0.0f) {
		enemy_->ChangeState(std::make_unique<EnemyMoveState>());
	}
}

///-------------------------------------------/// 
/// 終了時に呼びだす
///-------------------------------------------///
void EnemyHitReactionState::Finalize() {
	EnemyState::Finalize();
}
