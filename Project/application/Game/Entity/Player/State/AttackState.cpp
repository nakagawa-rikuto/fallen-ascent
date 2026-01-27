#include "AttackState.h"
// Service
#include "Engine/System/Service/InputService.h"
// Player
#include "application/Game/Entity/Player/Player.h"
#include "application/Game/Entity/Player/Weapon/PlayerWeapon.h"
// State
#include "RootState.h"
// Math
#include "Math/sMath.h"

///-------------------------------------------/// 
/// 状態に入ったときに呼ばれる
///-------------------------------------------///
void AttackState::Enter(Player* player, GameCamera* camera) {
	// 引数の取得
	player_ = player;
	camera_ = camera;

	// 攻撃の開始
	player_->GetAttackComponent()->StartAttack(0, player_->GetWeapon(), player_->GetRightHand(), player_->GetLeftHand());
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void AttackState::Update(Player* player, GameCamera* camera) {
	// 引数の取得
	player_ = player;
	camera_ = camera;

	PlayerAttackComponent* attackComp = player_->GetAttackComponent();
	// 早期リターン
	if (!attackComp) return;

	// 減速処理
	player_->ApplyDeceleration(0.4f);

	// 攻撃コンポーネントの更新
	attackComp->Update(player_->GetDeltaTime());

	// 攻撃ボタンが押されたらコンボを試行
	if (InputService::TriggerButton(0, ControllerButtonType::X)) {
		if (attackComp->CanCombo()) {
			attackComp->TryCombo(player_->GetWeapon(), player_->GetRightHand(), player_->GetLeftHand());
		}
	}

	// 攻撃が終了したらRootStateへ遷移
	if (!attackComp->IsAttacking() && !attackComp->CanCombo()) {
		player_->ChangState(std::make_unique<RootState>());
	}
}

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void AttackState::Finalize() {
	PlayerState::Finalize();
}