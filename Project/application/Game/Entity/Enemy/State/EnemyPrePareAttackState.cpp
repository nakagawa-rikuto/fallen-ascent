#include "EnemyPrePareAttackState.h"
// BaseEnemy
#include "application/Game/Entity/Enemy/Base/BaseEnemy.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// Service
#include "Engine/System/Service/ParticleService.h"
// State
#include "EnemyAttackState.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"

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
	// 向きの初期化
	attackDirection_ = { 0.0f, 0.0f, 0.0f };
	playerPos_ = { 0.0f, 0.0f, 0.0f };
	// フラグをリセット
	enemy_->SetIsRotationComplete(false);
	// パーティクルの再生
	activeParticle_ = ParticleService::Emit("EnemyPrePareAttack", enemy_->GetTransform().translate);
	prePareAttackParticle_ = ParticleService::Emit("EnemyPrePareAttackCharge", enemy_->GetTransform().translate);
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void EnemyPrePareAttackState::Update(BaseEnemy* enemy) {
	enemy_ = enemy;
	
	// タイマーが残っている場合
	if (activeTimer_ > 0.0f) {
		// タイマーの更新
		activeTimer_ -= enemy_->GetDeltaTime();

		// プレイヤーの位置への方向ベクトルを計算
		playerPos_ = enemy_->GetPlayer()->GetTransform().translate; // プレイヤーの位置を取得
		Vector3 dir = playerPos_ - enemy_->GetTransform().translate;
		attackDirection_ = Normalize(dir); // 正規化して方向ベクトルを得る

	} else { // タイマーが終了した場合

		/// ===Stateの変更=== ///
		if (enemy_->GetIsRotationComplete()) {
			// Particleの停止
			activeParticle_->Stop();
			activeParticle_ = nullptr;
			prePareAttackParticle_->Stop();
			prePareAttackParticle_ = nullptr;
			// 回転完了フラグをリセット
			enemy_->SetIsRotationComplete(false);
			// 攻撃方向を設定
			enemy_->SetAttackDirection(attackDirection_);
			// プレイヤーの位置を設定
			enemy_->SetPlayerPos(playerPos_);
			// AttackStateに移行
			enemy_->ChangeState(std::make_unique<EnemyAttackState>());
			return;
		}
	}

	//NOTE:後にのけぞりながら回転するように変更予定,Particleも追加予定

	// 回転の更新
	enemy_->UpdateRotationTowards(attackDirection_, rotationSpeed_);
	
	// 少し傾ける
	Quaternion enemyRot = Math::MakeRotateAxisAngle(Math::Cross(attackDirection_, { 0.0f, 1.0f, 0.0f }), 0.05f);
	enemy_->SetRotate(enemyRot * enemy_->GetTransform().rotate);
}

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void EnemyPrePareAttackState::Finalize() {
	EnemyState::Finalize();
}
