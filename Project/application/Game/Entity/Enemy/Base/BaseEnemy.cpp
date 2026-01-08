#include "BaseEnemy.h"
// Camera
#include "application/Game/Camera/GameCamera.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// State
#include "application/Game/Entity/Enemy/State/EnemyHitReactionState.h"
#include "application/Game/Entity/Enemy/State/EnemyMoveState.h"
// Service
#include "Engine/System/Service/InputService.h"
#include "Engine/System/Service/ParticleService.h"
#include "Engine/System/Service/CameraService.h"
#include "Engine/System/Service/ColliderService.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"
// ImGui
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI


///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
BaseEnemy::~BaseEnemy() {
	// 状態を解放
	currentState_->Finalize();
	currentState_.reset();
	ColliderService::RemoveCollider(this);
	object3d_.reset();
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void BaseEnemy::SetInvincibleTime() {
	invincibleInfo_.timer = invincibleInfo_.time;
	invincibleInfo_.isInvincible = true;
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void BaseEnemy::Initialize() {
	/// ===Componentの生成=== ///
	moveComponent_ = std::make_unique<EnemyMoveComponent>();
	hitReactionComponent_ = std::make_unique<EnemyHitReactionComponent>();
	// MoveComponentの初期化
	EnemyMoveComponent::MoveConfig moveConfig{
			.speed = 0.05f,
			.range = 20.0f,
			.interval = 5.0f,
			.waitTime = 1.5f,
			. rotationSpeed = 0.1f
	};
	moveComponent_->Initialize(transform_.translate, moveConfig);
	// HitReactionComponentの初期化
	EnemyHitReactionComponent::KnockBackConfig hitReactionConfig{
		.knockBackForce = 1.5f,
		.slowdownFactor = 0.5f,
		.slowdownDuration = 0.2f
	};
	hitReactionComponent_->Initialize(hitReactionConfig);

	// オブジェクトの色設定
	color_ = { 1.0f, 0.0f, 1.0f, 1.0f };

	// カメラの取得
	camera_ = CameraService::GetActiveCamera().get();

	// Stateの設定
	ChangeState(std::make_unique<EnemyMoveState>());

	// コライダーに追加
	ColliderService::AddCollider(this);
	
	// 無敵情報の初期化
	invincibleInfo_.time = 0.5f;
	invincibleInfo_.timer = 0.0f;
	invincibleInfo_.isInvincible = false;

	// 消滅タイマー
	disappearTimer_ = 3.0f;
	isTentativeDeath_ = false;

	// HP 
	baseInfo_.HP = 2;

	// object3dの更新を一回行う
	UpdateAnimation();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void BaseEnemy::Update() {
	// 死亡している場合は更新しない
	if (isTentativeDeath_) return;

	/// ===Timerの更新=== ///
	advanceTimer();

	/// ===Stateの更新=== ///
	if (currentState_) {
		// 各Stateの更新
		currentState_->Update(this);
	}

	UpdateAnimation();
}

///-------------------------------------------/// 
/// アニメーション時の更新処理
///-------------------------------------------///
void BaseEnemy::UpdateAnimation() {
	// GameCharacterの更新
	GameCharacter::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void BaseEnemy::Draw(BlendMode mode) {
	/// ===GameCharacterの描画=== ///
	GameCharacter::Draw(mode);
}

///-------------------------------------------/// 
/// ImGui
///-------------------------------------------///
void BaseEnemy::Information() {
#ifdef USE_IMGUI
	// MoveComponent
	moveComponent_->Information();

	// AttackInfo
	ImGui::Text("攻撃情報");
	ImGui::DragFloat("攻撃可能距離", &attackInfo_.distance, 0.1f);
	ImGui::DragFloat("攻撃範囲", &attackInfo_.range, 0.1f);
	ImGui::DragFloat("攻撃間隔", &attackInfo_.interval, 0.1f);
	ImGui::DragInt("パワー", &attackInfo_.power, 1);

	// HitReactionComponent
	hitReactionComponent_->Information();

	// InvincibleInfo
	ImGui::Separator();
	ImGui::Text("無敵情報");
	ImGui::DragFloat("無敵時間", &invincibleInfo_.time, 0.01f, 0.0f, 5.0f);
	ImGui::Text("無敵タイマー: %.2f", invincibleInfo_.timer);
	ImGui::Text("無敵中: %s", invincibleInfo_.isInvincible ? "true" : "false");

#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 変更した値をコピー
///-------------------------------------------///
void BaseEnemy::CopyTuningTo(BaseEnemy* enemy) const {
	if (!enemy) return;

	// ===== Object系 ===== //
	enemy->color_ = color_;

	// ===== Move 系(設計値) ===== //
	enemy->moveComponent_->ApplyConfig(moveComponent_->GetConfig());

	// ===== Attack 系(設計値) ===== //
	enemy->attackInfo_.distance = attackInfo_.distance;
	enemy->attackInfo_.range = attackInfo_.range;
	enemy->attackInfo_.interval = attackInfo_.interval;
	enemy->attackInfo_.power = attackInfo_.power;

	// ===== Knockback 系(設計値) ===== //
	enemy->hitReactionComponent_->ApplyConfig(hitReactionComponent_->GetConfig());

	// 型固有の値を派生側でコピー
	this->CopyTypeTuningFromThisTo(enemy);
}

///-------------------------------------------/// 
/// 当たり判定
///-------------------------------------------///
void BaseEnemy::OnCollision(Collider* collider) {

	// 攻撃用のフラグを立てる
	isCollision_ = true;

	/// ===GameCharacterの衝突=== ///
	GameCharacter::OnCollision(collider);

	// 早期リターン
	if (baseInfo_.isDead) {
		return;
	}

	// Weaponとの当たり判定
	if (collider->GetColliderName() == ColliderName::PlayerWeapon) {
		// クールタイム中でなければノックバック処理を実行
		if (!invincibleInfo_.isInvincible) {
			// 通常攻撃の時
			if (player_->GetAttackComponent()->IsAttacking()) {
				// StateをHitReaction変更
				ChangeState(std::make_unique<EnemyHitReactionState>());

				// ノックバック方向の計算
				Vector3 toWeapon = collider->GetTransform().translate - transform_.translate;
				toWeapon.y = 0.0f; // Y軸は無視
				toWeapon = -Normalize(toWeapon); // 反転して正規化
				hitReactionComponent_->OnHit(toWeapon);

				// HPを減少
				baseInfo_.HP--;
				hitParticle_ = ParticleService::Emit("Game", transform_.translate);

				// 無敵時間のセット
				SetInvincibleTime();
			}
		}
	}
}

///-------------------------------------------/// 
/// 攻撃可能かチェック
///-------------------------------------------///
bool BaseEnemy::CheckAttackable() {

	// 敵の前方向ベクトル(Y軸回転を使用)
	float yaw = transform_.rotate.y;
	Vector3 forward = {
		std::sinf(yaw),
		0.0f,
		std::cosf(yaw)
	};

	Vector3 toPlayer = player_->GetTransform().translate - transform_.translate;;
	toPlayer.y = 0.0f;
	float distance = Length(toPlayer);

	// 距離判定
	if (distance <= attackInfo_.distance) {
		Vector3 dirToPlayer = Normalize(toPlayer);
		float dot = Dot(forward, dirToPlayer);
		float angleToPlayer = std::acos(dot); // ラジアン

		// 角度が範囲内かチェック
		if (angleToPlayer <= attackInfo_.range) {
			return true;
		} else {
			return false;
		}
	} else {
		return false;
	}
}

///-------------------------------------------/// 
/// Stateの変更
///-------------------------------------------///
void BaseEnemy::ChangeState(std::unique_ptr<EnemyState> newState) {
	if (currentState_) {
		// 古い状態を解放
		currentState_->Finalize();
		currentState_.reset();
	}

	// 新しい状態をセット
	currentState_ = std::move(newState);
	// 新しい状態を初期化
	currentState_->Enter(this);
}

///-------------------------------------------/// 
/// 回転更新関数
///-------------------------------------------///
void BaseEnemy::UpdateRotationTowards(const Vector3& direction, float slerpT) {
	// 方向ベクトルのY成分を0にして水平方向のみを考慮
	Vector3 horizontalDir = { direction.x, 0.0f, direction.z };

	// 値が小さすぎる場合は何もしない
	if (Length(horizontalDir) < 0.01f) return;

	// forward方向からターゲットクォータニオンを作成(Y軸回転のみ)
	Quaternion targetRotation = Math::LookRotation(horizontalDir, Vector3(0.0f, 1.0f, 0.0f));

	// Quaternionの内積で類似度を計算
	float dotProduct = Dot(transform_.rotate, targetRotation);
	float absDot = std::abs(dotProduct);

	// 0.9999以上なら十分近い
	if (absDot > 0.99f) {
		isRotationComplete_ = true;
		return; // 既に向いているので処理終了
	}
	// 回転が完了していない場合はフラグをリセット
	isRotationComplete_ = false;

	// SLerp補間
	Quaternion result = Math::SLerp(transform_.rotate, targetRotation, slerpT);
	transform_.rotate = Normalize(result); // 正規化でスケール崩れ防止
}

///-------------------------------------------/// 
/// 時間を進める
///-------------------------------------------///
void BaseEnemy::advanceTimer() {

	if (baseInfo_.isDead) {
		// 消えるまでの時間を進める
		disappearTimer_ -= baseInfo_.deltaTime;
		if (disappearTimer_ <= 0) {
			deathParticle_ = ParticleService::Emit("nakagawa", transform_.translate);
			isTentativeDeath_ = true;
			if (hitParticle_ != nullptr) {
				hitParticle_->Stop();
				hitParticle_ = nullptr;
			}
		}
	} else {
		// 攻撃用のタイマーを進める
		if (attackInfo_.timer > 0.0f) {
			attackInfo_.timer -= baseInfo_.deltaTime;
		}

		// 無敵タイマーを進める
		if (invincibleInfo_.timer > 0.0f) {
			invincibleInfo_.timer -= baseInfo_.deltaTime;
			invincibleInfo_.isInvincible = true;
		} else {
			invincibleInfo_.isInvincible = false;
			invincibleInfo_.timer = 0.0f;
		}

	}
}