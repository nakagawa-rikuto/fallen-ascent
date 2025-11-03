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

	// 攻撃フラグを立てる
	player_->SetStateFlag(actionType::kAttack, true);
	player_->SetpreparationFlag(actionType::kAttack, false);

	// 最初の攻撃を初期化
	InitializeAttack(AttackType::kCombo1);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void AttackState::Update(Player* player, GameCamera* camera) {
	// 引数の取得
	player_ = player;
	camera_ = camera;

	// アクティブタイマーを進める
	attackInfo_.activTime -= player_->GetDeltaTime();

	// 減速処理
	player_->ApplyDeceleration(0.4f);

	// コンボ受付タイマーを進める
	if (attackInfo_.canCombo) {
		attackInfo_.comboTimer -= player_->GetDeltaTime();

		// コンボ受付時間を過ぎたらコンボ受付終了
		if (attackInfo_.comboTimer <= 0.0f) {
			attackInfo_.canCombo = false;
		}
	}

	// コンボ入力チェック
	CheckComboInput();

	// 攻撃中の処理
	if (attackInfo_.isAttacking) {
		// 攻撃時間が終了
		if (attackInfo_.activTime <= 0.0f) {
			attackInfo_.isAttacking = false;

			// 次のコンボ入力がされている場合
			if (attackInfo_.nextComboRequest && attackInfo_.canCombo) {
				TransitionToNextCombo();
			} else {
				// 攻撃終了、通常状態へ
				player_->SetTimer(actionType::kAttack, attackInfo_.coolTime);
				player_->SetStateFlag(actionType::kAttack, false);
				player_->ChangState(std::make_unique<RootState>());
			}
		}
	}
}

///-------------------------------------------/// 
/// 終了処理
///-------------------------------------------///
void AttackState::Finalize() {
	PlayerState::Finalize();
}

///-------------------------------------------/// 
/// 攻撃の初期化
///-------------------------------------------///
void AttackState::InitializeAttack(AttackType type) {
	// 早期リターン
	PlayerWeapon* weapon = player_->GetWeapon();
	if (!weapon)return;

	// AttackInfoの初期化	
	attackInfo_.currentAttack = type;
	attackInfo_.isAttacking = true;
	attackInfo_.canCombo = false;
	attackInfo_.nextComboRequest = false;

	// 基準の位置と回転の設定
	Vector3 BasePos = { 0.0f, 0.0f, 0.0f};
	Quaternion BaseRot = { 0.0f, 0.0f, 0.0f,1.0f };

	// ベースの向きに基づいて方向ベクトルを計算
	Vector3 forward = Math::RotateVector({ 0.0f, 0.0f, 1.0f }, BaseRot);
	Vector3 up = { 0.0f, 1.0f, 0.0f };

	/// ===攻撃タイプに応じた設定=== ///
	switch (type) {
	// コンボ1段目
	case AttackState::AttackType::kCombo1: {
		attackInfo_.activTime = attackInfo_.combo1Duration;
		attackInfo_.canCombo = true;
		attackInfo_.comboTimer = attackInfo_.comboWindowTime;

		// -90度の位置（プレイヤーの向きから見て左側）
		float startAngleRad = -90.0f * Math::Pi() / 180.0f;
		Quaternion startRotQuat = Math::MakeRotateAxisAngle(up, startAngleRad);
		Vector3 startDirection = Math::RotateVector(forward, startRotQuat);
		Vector3 startPoint = BasePos + startDirection * attackInfo_.weaponLength;

		// +90度の位置（プレイヤーの向きから見て右側）
		float endAngleRad = 90.0f * Math::Pi() / 180.0f;
		Quaternion endRotQuat = Math::MakeRotateAxisAngle(up, endAngleRad);
		Vector3 endDirection = Math::RotateVector(forward, endRotQuat);
		Vector3 endPoint = BasePos + endDirection * attackInfo_.weaponLength;

		// 開始時は左を向く
		Quaternion startRot = Math::MakeRotateAxisAngle(up, startAngleRad);
		startRot = Multiply(BaseRot, startRot);

		// 終了時は右を向く
		Quaternion endRot = Math::MakeRotateAxisAngle(up, endAngleRad);
		endRot = Multiply(BaseRot, endRot);

		// 武器の攻撃を開始（左→右）
		weapon->StartAttack(startPoint, endPoint, attackInfo_.combo1Duration, startRot, endRot);
		break;
	}
	// コンボ2段目
	case AttackState::AttackType::kCombo2: {
		attackInfo_.activTime = attackInfo_.combo2Duration;
		attackInfo_.canCombo = false;

		// +90度の位置（プレイヤーの向きから見て右側）
		float startAngleRad = 90.0f * Math::Pi() / 180.0f;
		Quaternion startRotQuat = Math::MakeRotateAxisAngle(up, startAngleRad);
		Vector3 startDirection = Math::RotateVector(forward, startRotQuat);
		Vector3 startPoint = BasePos + startDirection * attackInfo_.weaponLength;

		// -90度の位置（プレイヤーの向きから見て左側）
		float endAngleRad = -90.0f * Math::Pi() / 180.0f;
		Quaternion endRotQuat = Math::MakeRotateAxisAngle(up, endAngleRad);
		Vector3 endDirection = Math::RotateVector(forward, endRotQuat);
		Vector3 endPoint = BasePos + endDirection * attackInfo_.weaponLength;

		// 開始時は右を向く
		Quaternion startRot = Math::MakeRotateAxisAngle(up, startAngleRad);
		startRot = Multiply(BaseRot, startRot);

		// 終了時は左を向く
		Quaternion endRot = Math::MakeRotateAxisAngle(up, endAngleRad);
		endRot = Multiply(BaseRot, endRot);

		// 武器の攻撃を開始（右→左）
		weapon->StartAttack(startPoint, endPoint, attackInfo_.combo2Duration, startRot, endRot);
		break;
	}
	}
}

///-------------------------------------------/// 
/// コンボ入力のチェック
///-------------------------------------------///
void AttackState::CheckComboInput() {
	// Xボタンのトリガー入力でコンボ継続
	if (InputService::TriggerButton(0, ControllerButtonType::X)) {
		// コンボ受付中なら次のコンボをリクエスト
		if (attackInfo_.canCombo) {
			attackInfo_.nextComboRequest = true;
		}
	}
}

///-------------------------------------------/// 
/// 次のコンボへ遷移
///-------------------------------------------///
void AttackState::TransitionToNextCombo() {
	attackInfo_.nextComboRequest = false;

	// 現在の攻撃タイプに応じて次の攻撃へ
	switch (attackInfo_.currentAttack) {
	case AttackType::kCombo1:
		InitializeAttack(AttackType::kCombo2);
		break;
	case AttackType::kCombo2:
		break;
	}
}