#include "MoveState.h"
// Service
#include "Engine/System/Service/InputService.h"
#include "Engine/System/Service/ParticleService.h"
// Player, Camera
#include "application/Game/Entity/Player/Player.h"
#include "application/Game/Camera/GameCamera.h"
// State
#include "RootState.h"
#include "AvoidanceState.h"
#include "AttackState.h"
// Math
#include "Math/sMath.h"


///-------------------------------------------/// 
/// 開始時に呼び出す
///-------------------------------------------///
void MoveState::Enter(Player* player, GameCamera* camera) {
	// 引数の取得
	player_ = player;
	camera_ = camera;

	// 移動パーティクルの強制停止
	if (moveParticle_) {
		moveParticle_->Stop();
		moveParticle_ = nullptr;
	}
	// 移動パーティクルの再生
	moveParticle_ = ParticleService::Emit("PlayerWarke", player_->GetTransform().translate);
	moveParticle_->SetEmitterPosition(player_->GetTransform().translate);
}

///-------------------------------------------/// 
/// 更新時に呼び出す
///-------------------------------------------///
void MoveState::Update(Player * player, GameCamera* camera) {
	// 引数の取得
	player_ = player;
	camera_ = camera;

	/// ===左スティックの取得=== ///
	StickState leftStick = InputService::GetLeftStickState(0);

	/// ===移動処理=== ///
	PlayerMoveComponent::UpdateContext context{
		.inputDirection = { leftStick.x, 0.0f, leftStick.y },
		.currentPosition = player_->GetTransform().translate,
		.currentRotation = player_->GetTransform().rotate,
		.deltaTime = player_->GetDeltaTime()
	};
	// 更新
	auto result = player_->GetMoveComponent()->Update(context);

	// 結果を反映
	player_->SetVelocity(result.velocity);
	player_->SetRotate(result.targetRotation);

	// パーティクルの位置更新
	if (moveParticle_) {
		moveParticle_->SetEmitterPosition(player_->GetTransform().translate);
	}

	/// ===Stateの変更=== ///
	// 攻撃ボタンが押されたら攻撃状態へ
	if (InputService::TriggerButton(0, ControllerButtonType::X)) {
		// 攻撃の準備ができていれば
		if (!player_->GetAttackComponent()->GetState().isActive) {
			// パーティクルの停止
			//StopMoveParticle();
			// AttackStateへ移行
			player_->ChangState(std::make_unique<AttackState>());
		}
	// RBボタンが押されたら進んでいる突進状態へ
	} else if (InputService::TriggerButton(0, ControllerButtonType::RB)) {

	// Aボタンが押されたら回避状態へ
	} else if (InputService::TriggerButton(0, ControllerButtonType::A)) {
		// 回避の準備ができていれば
		if (player_->GetAvoidanceComponent()->GetState().isPreparation) {
			// パーティクルの停止
			//StopMoveParticle();
			// AvoidanceStateへ移行
			player_->ChangState(std::make_unique<AvoidanceState>(Normalize(player_->GetMoveComponent()->GetCurrentDirection())));
		}
	// 移動が無ければ通常状態へ
	} else if (std::abs(leftStick.x) < 0.1f && std::abs(leftStick.y) < 0.1f) {
		// パーティクルの停止
		//StopMoveParticle();
		// RootStateへ移行
		player_->ChangState(std::make_unique<RootState>());
	}
}

///-------------------------------------------/// 
/// 終了時に呼び出す
///-------------------------------------------///
void MoveState::Finalize() {
	PlayerState::Finalize();
}

///-------------------------------------------/// 
/// パーティクルの停止
///-------------------------------------------///
void MoveState::StopMoveParticle() {
	if (moveParticle_) {
		moveParticle_->Stop();
		moveParticle_ = nullptr;
	}
}
