#include "ChargeState.h"
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
void ChargeState::Enter(Player* player, GameCamera* camera) {
	// 引数の取得
	player_ = player;
	camera_ = camera;

	// チャージ状態フラグを立てる
	player_->SetStateFlag(actionType::kCharge, true);
	player_->SetpreparationFlag(actionType::kCharge, false);

	// チャージ情報の初期化
	chargeInfo_.currentPhase = ChargePhase::kCharging;
	chargeInfo_.chargeTimer = 0.0f;
	chargeInfo_.chargePower = 0.0f;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void ChargeState::Update(Player* player, GameCamera* camera) {
	// 引数の取得
	player_ = player;
	camera_ = camera;

	// 現在のフェーズに応じた処理
	switch (chargeInfo_.currentPhase) {
	case ChargePhase::kCharging:
		UpdateCharging();
		break;
	case ChargePhase::kAttacking:
		UpdateAttacking();
		break;
	}
}

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void ChargeState::Finalize() {
	PlayerState::Finalize();
}

///-------------------------------------------/// 
/// チャージフェーズの処理
///-------------------------------------------///
void ChargeState::UpdateCharging() {
	// 減速処理（チャージ中は動きを抑える）
	player_->ApplyDeceleration(0.8f);

	// RBボタンが押されている間チャージ
	if (InputService::PushButton(0, ControllerButtonType::RB)) {
		// チャージタイマーを進める
		chargeInfo_.chargeTimer += player_->GetDeltaTime();

		// チャージ力を計算（0.0～1.0の範囲）
		chargeInfo_.chargePower = (std::min)(chargeInfo_.chargeTimer / chargeInfo_.chargeMaxTime, 1.0f);

		// ここでチャージエフェクトやサウンドを再生可能
		

	} else {
		// RBボタンが離されたら攻撃フェーズへ移行
		StartChargeAttack();
		chargeInfo_.currentPhase = ChargePhase::kAttacking;
	}
}

///-------------------------------------------/// 
/// 攻撃フェーズの処理
///-------------------------------------------///
void ChargeState::UpdateAttacking() {
	// 減速処理
	player_->ApplyDeceleration(0.3f);

	// 武器が攻撃中かチェック
	PlayerWeapon* weapon = player_->GetWeapon();
	if (!weapon) {
		// 武器がない場合は通常状態へ
		player_->SetTimer(actionType::kCharge, chargeInfo_.coolTime);
		player_->SetStateFlag(actionType::kCharge, false);
		player_->ChangState(std::make_unique<RootState>());
		return;
	}

	// 攻撃が終了したら通常状態へ
	if (!weapon->GetIsAttack()) {
		player_->SetTimer(actionType::kCharge, chargeInfo_.coolTime);
		player_->SetStateFlag(actionType::kCharge, false);
		player_->ChangState(std::make_unique<RootState>());
	}
}

///-------------------------------------------/// 
/// チャージ攻撃を開始する
///-------------------------------------------///
void ChargeState::StartChargeAttack() {
	// 早期リターン
	PlayerWeapon* weapon = player_->GetWeapon();
	if (!weapon) return;

	// 基準の位置と回転の設定（ローカル座標）
	Vector3 basePos = { 0.0f, player_->GetTransform().translate.y + 0.0f, 0.0f}; // 高い位置から開始
	Quaternion baseRot = { 0.0f, 0.0f, 0.0f, 1.0f };

	// ベースの向きに基づいて方向ベクトルを計算
	Vector3 forward = Math::RotateVector({ 0.0f, 0.0f, 1.0f }, baseRot);
	Vector3 up = { 0.0f, 1.0f, 0.0f };

	// 振り下ろし攻撃の開始点と終了点
	// 開始点：頭上（前方やや上）
	Vector3 startPoint = basePos + forward * (chargeInfo_.weaponLength * 0.3f);
	startPoint.y += 1.5f; // さらに上に

	// 終了点：前方下（地面近く）
	Vector3 endPoint = basePos + forward * chargeInfo_.weaponLength;
	endPoint.y -= 1.0f; // 下に振り下ろす

	// 開始時の回転：武器を上に向ける（-90度回転）
	float startAngleRad = -90.0f * Math::Pi() / 180.0f;
	Quaternion startRot = Math::MakeRotateAxisAngle(
		Math::Cross(up, forward), // 横軸周り
		startAngleRad
	);
	startRot = Multiply(baseRot, startRot);

	// 終了時の回転：武器を下に向ける（+90度回転）
	float endAngleRad = 90.0f * Math::Pi() / 180.0f;
	Quaternion endRot = Math::MakeRotateAxisAngle(
		Math::Cross(up, forward),
		endAngleRad
	);
	endRot = Multiply(baseRot, endRot);

	// 攻撃時間をチャージ力に応じて調整（チャージが強いほど速い）
	float adjustedDuration = chargeInfo_.attackDuration * (1.0f - chargeInfo_.chargePower * 0.3f);

	// 武器の攻撃を開始（上→下）
	weapon->StartChargeAttack(startPoint, endPoint, adjustedDuration, startRot, endRot);

	// ここでチャージ力に応じたダメージを設定可能
	// float damage = chargeInfo_.minDamage + 
	//                (chargeInfo_.maxDamage - chargeInfo_.minDamage) * chargeInfo_.chargePower;
	// weapon->SetDamage(damage);
}