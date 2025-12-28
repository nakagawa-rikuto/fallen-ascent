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
	// 色
	enemy_->SetColor({ 0.0f, 0.0f, 1.0f, 1.0f });


	// 準備タイマーをリセット
	preparationTImer_ = preparationDuration_;
	// ターゲットの位置を保存
	if (enemy_->GetPlayer()) {
		previousTargetPos_ = enemy_->GetPlayer()->GetTransform().translate;
	}
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void EnemyPrePareAttackState::Update(BaseEnemy* enemy) {
	enemy_ = enemy;

	// タイマーを減らす
	preparationTImer_ -= enemy_->GetDeltaTime();

	// プレイヤーの位置を取得
	Player* player = enemy_->GetPlayer();
	if (player) {
		// プレイヤーの現在位置
		Vector3 currentTargetPos = player->GetTransform().translate;
		// 前フレームの位置都の差分を計算
		Vector3 targetDelta = currentTargetPos - previousTargetPos_;

		// 座標が変化していれば回転
		if (Length(targetDelta) > 0.001f) {
			// ターゲットの方向を計算
			Vector3 directionToTarget = currentTargetPos - enemy_->GetTransform().translate;
			directionToTarget.y = 0.0f; // 水平方向のみ考慮

			if (Length(directionToTarget) > 0.001f) {
				Vector3 direction = Normalize(directionToTarget);
				// 回転
				enemy_->UpdateRotationTowards(direction, 0.3f);
			}
		}
	}

	// 準備時間が経過したら攻撃状態へ移行
	if (enemy_->GetIsRotationComplete() && preparationTImer_ <= 0.0f) {
		// 回転完了フラグのリセット
		enemy_->SetIsRotationComplete(false);
		// 攻撃に
		enemy_->ChangeState(std::make_unique<EnemyAttackState>());
	}
}

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void EnemyPrePareAttackState::Finalize() {
	EnemyState::Finalize();
}
