#include "EnemyPrePareAttackState.h"
// BaseEnemy
#include "application/Game/Entity/Enemy/Base/BaseEnemy.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// State
#include "EnemyAttackState.h"

///-------------------------------------------/// 
/// ステートに入った時に呼ばれる処理
///-------------------------------------------///
void EnemyPrePareAttackState::Enter(BaseEnemy* enemy) {
	enemy_ = enemy;
	// 移動量を初期化
	enemy_->SetVelocity({ 0.0f, 0.0f, 0.0f });
	// 色の設定
	enemy_->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });
	// 実行時間の設定
	activeTimer_ = acticeTime;
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void EnemyPrePareAttackState::Update(BaseEnemy* enemy) {
	enemy_ = enemy;

	// タイマーの更新
	activeTimer_ -= enemy_->GetDeltaTime();

	// プレイヤーの位置への方向ベクトルを計算
	Vector3 dir = enemy_->GetTransform().translate - enemy_->GetPlayer()->GetTransform().translate;
	Vector3 NormalDir = Normalize(dir); // 正規化
	NormalDir = NormalDir * -1.0f; // 反転

	// 回転の更新
	enemy_->UpdateRotationTowards(NormalDir, 0.2f);

	// Stateの変更
	if (enemy_->GetIsRotationComplete() && activeTimer_ <= 0.0f) {
		// 回転完了フラグをリセット
		enemy_->SetIsRotationComplete(false);
		// 攻撃方向を設定
		enemy_->SetAttackDirection(NormalDir);
		// AttackStateに移行
		enemy_->ChangeState(std::make_unique<EnemyAttackState>());
	}
}

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void EnemyPrePareAttackState::Finalize() {
	EnemyState::Finalize();
}
