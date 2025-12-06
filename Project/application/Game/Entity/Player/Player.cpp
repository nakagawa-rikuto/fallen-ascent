#include "Player.h"
// Camera
#include "application/Game/Camera/GameCamera.h"
// State
#include "State/RootState.h"
// Service
#include "Engine/System/Service/InputService.h"
#include "Engine/System/Service/CameraService.h"
#include "Engine/System/Service/ColliderService.h"
// Math
#include "Math/sMath.h"

// ImGui
#ifdef USE_IMGUI
#include "imgui.h"
#include "application/Game/Entity/Enemy/Base/BaseEnemy.h"
#endif // USE_IMGUI

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Player::~Player() {
	weapon_.reset();
	object3d_.reset();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// Weapon
PlayerWeapon* Player::GetWeapon() const { return weapon_.get(); }
// フラグ
bool Player::GetStateFlag(actionType type) const {
	// タイプで分岐
	if (type == actionType::kAvoidance) {
		return avoidanceInfo_.isFlag;
	} else if (type == actionType::kCharge) {
		return chargeInfo_.isFlag;
	} else {
		return attackInfo_.isFlag;
	}
}
bool Player::GetpreparationFlag(actionType type) const {
	// タイプで分岐
	if (type == actionType::kAvoidance) {
		return avoidanceInfo_.isPreparation;
	} else if (type == actionType::kCharge) {
		return chargeInfo_.isPreparation;
	} else {
		return attackInfo_.isPreparation;
	}
}
// タイマー
float Player::GetTimer(actionType type) {
	// タイプで分岐
	if (type == actionType::kAvoidance) {
		return avoidanceInfo_.timer;
	} else if (type == actionType::kCharge) {
		return chargeInfo_.timer;
	} else {
		return attackInfo_.timer;
	}
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// Cameraの追従対象としてPlaeyrを設定
void Player::SetCameraTargetPlayer() { camera_->SetTarget(&transform_.translate, &transform_.rotate); }
// 開始フラグ
void Player::SetStateFlag(actionType type, bool falg) {
	// タイプで分岐
	switch (type) {
	case actionType::kAvoidance:
		avoidanceInfo_.isFlag = falg;
		break;
	case actionType::kCharge:
		chargeInfo_.isFlag = falg;
		break;
	case actionType::kAttack:
		attackInfo_.isFlag = falg;
		break;
	}
}
// 準備フラグ
void Player::SetpreparationFlag(actionType type, bool falg) {
	// タイプで分岐
	switch (type) {
	case actionType::kAvoidance:
		avoidanceInfo_.isPreparation = falg;
		break;
	case actionType::kCharge:
		chargeInfo_.isPreparation = falg;
		break;
	case actionType::kAttack:
		attackInfo_.isPreparation = falg;
		break;
	}
}
// タイマーの設定
void Player::SetTimer(actionType type, const float& timer) {
	// タイプで分岐
	switch (type) {
	case actionType::kAvoidance:
		avoidanceInfo_.timer = timer;
		break;
	case actionType::kCharge:
		chargeInfo_.timer = timer;
		break;
	case actionType::kAttack:
		attackInfo_.timer = timer;
		break;
	}
}
// 無敵時間の設定
void Player::SetInvicibleTime(const float& time) { 
	invicibleInfo_.time = time;
	invicibleInfo_.timer = invicibleInfo_.time;
}


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Player::Initialize() {
	// Cameraの設定
	camera_ = CameraService::GetActiveCamera().get();

	// Object3dの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "player");

	// GameCharacterの設定
	GameCharacter::Initialize();
	name_ = ColliderName::Player;
	// コライダーに追加
	ColliderService::AddCollider(this);

	// Weaponの初期化
	weapon_ = std::make_unique<PlayerWeapon>();
	weapon_->Initialize();
	weapon_->SetUpParent(this);
	
	// 無敵情報の設定
	SetInvicibleTime(1.0f);
	invicibleInfo_.isFlag = true;

	// HPの設定
	baseInfo_.HP = 5;

	// 初期設定
	ChangState(std::make_unique<RootState>());

	// Updateを一回行う
	UpdateAnimation();
}


///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Player::Update() {
	/// ===スティックの取得=== ///
	StickState rightStick = InputService::GetRightStickState(0);
	camera_->SetStick({ rightStick.x, rightStick.y });

	/// ===タイマーを進める=== ///
	advanceTimer();

	/// ===Stateの管理=== ///
	if (currentState_) {
		// 各Stateの更新
		currentState_->Update(this, camera_);
	}

	/// ===更新処理=== ///
	UpdateAnimation();
}

///-------------------------------------------/// 
/// 開始アニメーション時の更新
///-------------------------------------------///
void Player::UpdateAnimation() {

	/// ===Weapon=== ///
	weapon_->Update();

	/// ===SphereColliderの更新=== ///
	GameCharacter::Update();
}

///-------------------------------------------///  
/// 描画
///-------------------------------------------///
void Player::Draw(BlendMode mode) {

	/// ===Weapon=== ///
	weapon_->Draw(mode);

	/// ===SphereColliderの描画=== ///
	GameCharacter::Draw(mode);
}


///-------------------------------------------/// 
/// ImGui
///-------------------------------------------///
void Player::Information() {
#ifdef USE_IMGUI
	ImGui::Begin("Player");
	GameCharacter::Information();
	ImGui::DragFloat("無敵時間", &invicibleInfo_.timer, 0.01f);
	ImGui::End();
	weapon_->Information();
#endif // USE_IMGUI
}


///-------------------------------------------/// 
/// 衝突
///-------------------------------------------///
void Player::OnCollision(Collider* collider) {

	/// ===GameCharacterの衝突=== ///
	GameCharacter::OnCollision(collider);

	// Colliderによって処理を変更
	if (collider->GetColliderName() == ColliderName::Enemy || collider->GetColliderName() == ColliderName::EnemyBullet) {

		// 無敵状態でなければダメージを受ける
		if (!invicibleInfo_.isFlag) {

			//// ===ノックバック処理=== ///
			//// 敵の位置を取得
			//Vector3 enemyPos = collider->GetTransform().translate;
			//// プレイヤーから敵への方向ベクトルを計算
			//Vector3 knockbackDirection = transform_.translate - enemyPos;
			//// Y軸は無視(水平方向のみノックバック)
			//knockbackDirection.y = 0.0f;
			//// 正規化
			//if (knockbackDirection.x != 0.0f || knockbackDirection.z != 0.0f) {
			//	knockbackDirection = Normalize(knockbackDirection);
			//}
			//// ノックバックの速度を設定(適切な値に調整してください)
			//const float knockbackSpeed = 1.5f;
			//baseInfo_.velocity = knockbackDirection * knockbackSpeed;

			// Stateを Root に変更
			ChangState(std::make_unique<RootState>());

			// ダメージ処理
			//baseInfo_.HP--;
			// 無敵状態にする
			SetInvicibleTime(0.5f);
		}
	}
}

///-------------------------------------------/// 
/// 時間を進める
///-------------------------------------------///
void Player::advanceTimer() {

	// 無敵タイマーを進める
	if (invicibleInfo_.timer > 0.0f) {
		invicibleInfo_.timer -= baseInfo_.deltaTime;
		color_ = { 1.0f, 0.0f, 0.0f, 0.5f }; // 半透明にする
		invicibleInfo_.isFlag = true;
	} else {
		invicibleInfo_.isFlag = false;
		color_ = { 1.0f, 1.0f, 1.0f, 1.0f }; // 元の色に戻す
	}

	// 突進用のタイマーを進める
	if (chargeInfo_.timer > 0.0f) {
		chargeInfo_.timer -= baseInfo_.deltaTime;
	} else {
		chargeInfo_.isPreparation = true;
	}

	// 回避用タイマーを進める
	if (avoidanceInfo_.timer > 0.0f) {
		avoidanceInfo_.timer -= baseInfo_.deltaTime;
	} else {
		avoidanceInfo_.isPreparation = true;
	}

	// 攻撃用タイマーを進める
	if (attackInfo_.timer > 0.0f) {
		attackInfo_.timer -= baseInfo_.deltaTime;
	} else {
		attackInfo_.isPreparation = true;
	}
}

///-------------------------------------------/// 
/// 減速処理
///-------------------------------------------///
void Player::ApplyDeceleration(const float& develeration) {
	// Velocityが0でないなら徐々に0にする
	if (baseInfo_.velocity.x != 0.0f) {
		// 各軸に対して減速適用
		baseInfo_.velocity.x *= develeration;
		// 小さすぎる値は完全に0にスナップ
		if (std::abs(baseInfo_.velocity.x) < 0.01f) {
			baseInfo_.velocity.x = 0.0f;
		}
	}
	if (baseInfo_.velocity.z != 0.0f) {
		baseInfo_.velocity.z *= develeration;
		if (std::abs(baseInfo_.velocity.z) < 0.01f) {
			baseInfo_.velocity.z = 0.0f;
		}
	}
}

///-------------------------------------------/// 
/// Stateの変更
///-------------------------------------------///
void Player::ChangState(std::unique_ptr<PlayerState> newState) {
	if (currentState_) {
		// 古い状態を解放  
		currentState_->Finalize();
		currentState_.reset();
	}

	// 新しい状態をセット  
	currentState_ = std::move(newState);
	// 新しい状態の初期化  
	currentState_->Enter(this, camera_);
}
