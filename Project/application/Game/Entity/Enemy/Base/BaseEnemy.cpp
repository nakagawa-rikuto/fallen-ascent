#include "BaseEnemy.h"
// Camera
#include "application/Game/Camera/GameCamera.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// State
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
	object3d_.reset();
	ColliderService::RemoveCollider(this);
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// タイマー
void BaseEnemy::SetTimer(StateType type, float time) {
	// typeに応じてタイマーを設定
	switch (type) {
	case StateType::Move:
		moveInfo_.timer = time;
		break;
	case StateType::Attack:
		attackInfo_.timer = time;
		break;
	}
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void BaseEnemy::Initialize() {

	// ランダムエンジンの初期化
	std::seed_seq seed{
		static_cast<uint32_t>(std::time(nullptr)),
		static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this)),
	};
	randomEngine_.seed(seed);

	// オブジェクトの色設定
	color_ = { 1.0f, 0.0f, 1.0f, 1.0f };

	// カメラの取得
	camera_ = CameraService::GetActiveCamera().get();

	// Stateの設定
	ChangeState(std::make_unique<EnemyMoveState>());

	// コライダーに追加
	ColliderService::AddCollider(this);

	// ノックバック情報の初期化
	knockbackInfo_.cooldownTimer = 0.0f;
	knockbackInfo_.cooldownDuration = 0.5f;  // 0.5秒のクールタイム
	knockbackInfo_.knockbackForce = 1.5f;    // ノックバックの強さ
	knockbackInfo_.hitColorDuration = 0.1f;  // 0.2秒間赤く表示
	knockbackInfo_.hitColorTimer = 1.0f;
	knockbackInfo_.originalColor = color_;   // 元の色を保存
	knockbackInfo_.isInCooldown = true;

	disappearTimer_ = 3.0f;

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
	// GameChareacterの更新
	GameCharacter::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void BaseEnemy::Draw(BlendMode mode) {
	/// ===GameCahracterの描画=== ///
	GameCharacter::Draw(mode);
}

///-------------------------------------------/// 
/// ImGui
///-------------------------------------------///
void BaseEnemy::Information() {
#ifdef USE_IMGUI
	// MoveInfo
	ImGui::Text("移動情報");
	ImGui::DragFloat("移動速度", &moveInfo_.speed, 0.1f);
	ImGui::DragFloat("移動範囲", &moveInfo_.range, 0.1f);
	ImGui::DragFloat("移動間隔", &moveInfo_.interval, 0.1f);
	ImGui::DragFloat("待機時間", &moveInfo_.waitTime, 0.1f);

	// AttackInfo
	ImGui::Text("攻撃情報");
	ImGui::DragFloat("攻撃可能距離", &attackInfo_.distance, 0.1f);
	ImGui::DragFloat("攻撃範囲", &attackInfo_.range, 0.1f);
	ImGui::DragFloat("攻撃間隔", &attackInfo_.interval, 0.1f);
	ImGui::DragInt("パワー", &attackInfo_.power, 1);

	// KnockbackInfo
	ImGui::Text("ノックバック情報");
	ImGui::DragFloat("クールタイムの持続時間", &knockbackInfo_.cooldownDuration, 0.01f);
	ImGui::DragFloat("ノックバックの強さ", &knockbackInfo_.knockbackForce, 0.1f);
	ImGui::DragFloat("色変更時間", &knockbackInfo_.hitColorDuration, 0.01f);
	ImGui::Checkbox("クール中かどうか", &knockbackInfo_.isInCooldown);
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
	enemy->moveInfo_.speed = moveInfo_.speed;
	enemy->moveInfo_.range = moveInfo_.range;
	enemy->moveInfo_.interval = moveInfo_.interval;
	enemy->moveInfo_.waitTime = moveInfo_.waitTime;

	// ===== Attack 系(設計値) ===== //
	enemy->attackInfo_.distance = attackInfo_.distance;
	enemy->attackInfo_.range = attackInfo_.range;
	enemy->attackInfo_.interval = attackInfo_.interval;
	enemy->attackInfo_.power = attackInfo_.power;

	// ===== Knockback 系(設計値) ===== //
	enemy->knockbackInfo_.cooldownDuration = knockbackInfo_.cooldownDuration;
	enemy->knockbackInfo_.knockbackForce = knockbackInfo_.knockbackForce;
	enemy->knockbackInfo_.hitColorDuration = knockbackInfo_.hitColorDuration;

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
		if (!knockbackInfo_.isInCooldown) {
			// 通常攻撃の時
			if (player_->GetStateFlag(actionType::kAttack)) {
				// Stateを移動状態に変更
				ChangeState(std::make_unique<EnemyMoveState>());

				// プレイヤーの位置を取得
				Vector3 playerPos = player_->GetTransform().translate;

				// 敵からプレイヤーへの方向ベクトルを計算
				Vector3 toEnemy = transform_.translate - playerPos;
				toEnemy.y = 0.0f; // Y軸は無視

				// 正規化してノックバック方向を決定
				if (Length(toEnemy) > 0.001f) {
					toEnemy = Normalize(toEnemy);

					// ノックバック速度を設定
					baseInfo_.velocity = toEnemy * knockbackInfo_.knockbackForce;
				}

				// 色を赤に変更
				color_ = Vector4(1.0f, 0.0f, 0.0f, 1.0f); // 赤色
				
				// HPを減少
				baseInfo_.HP--;
				ParticleService::Emit("Game", transform_.translate);

				// タイマーをセット
				knockbackInfo_.hitColorTimer = knockbackInfo_.hitColorDuration;
				knockbackInfo_.cooldownTimer = knockbackInfo_.cooldownDuration;
				knockbackInfo_.isInCooldown = true;

			// チャージ攻撃の時
			} else if (player_->GetStateFlag(actionType::kCharge)) {

			}
		}
	}
}

///-------------------------------------------/// 
/// 移動処理の開始処理
///-------------------------------------------///
void BaseEnemy::CommonMoveInit() {
	// 速度をリセット
	baseInfo_.velocity = { 0.0f, 0.0f, 0.0f };
}

///-------------------------------------------/// 
/// 移動処理の共通部分
///-------------------------------------------///
void BaseEnemy::CommonMove() {
	// 移動範囲の中心との方向ベクトルを計算(XZ平面)
	Vector3 toCenter = moveInfo_.rangeCenter - transform_.translate;
	// 中心からの距離を取得
	float distanceFromCenter = Length(toCenter);

	/// ===移動処理=== ///
	if (moveInfo_.isWating) { // 待機中だったら

		// 待機中は移動しない
		baseInfo_.velocity = { 0.0f, 0.0f, 0.0f }; 

		// 向き方向に回転
		UpdateRotationTowards(moveInfo_.direction, 0.1f);

		// 回転し終わったら
		if (isRotationComplete_) { // ここできちんと回転し終わるまで動かさないようにする。
			// ランダムな時間を設定
			std::uniform_real_distribution<float> timeDist(1.0f, moveInfo_.interval);
			moveInfo_.timer = timeDist(randomEngine_);

			// 移動ベクトルを設定
			baseInfo_.velocity = moveInfo_.direction * moveInfo_.speed;
			moveInfo_.isWating = false;		// 待機フラグを解除
			isRotationComplete_ = false;   // 回転完了フラグをリセット
		}

	} else if (distanceFromCenter > moveInfo_.range) { // 範囲外に出た場合

		// 方向の設定と待機処理の準備
		PreparNextMove(toCenter);

	} else if (moveInfo_.timer <= 0.0f && !moveInfo_.isWating) { // 範囲内でタイマーが0になったら

		// ランダムな角度と距離を生成
		std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * Math::Pi());
		std::uniform_real_distribution<float> distanceDist(0.0f, moveInfo_.range);
		// ランダムな値の設定
		float angle = angleDist(randomEngine_);
		float distance = distanceDist(randomEngine_);

		// 方向ベクトルを円から算出
		Vector3 offset = {
			std::cos(angle) * distance,
			0.0f,
			std::sin(angle) * distance
		};

		// 移動先の座標を計算
		Vector3 target = moveInfo_.rangeCenter + offset;
		target.y = transform_.translate.y;

		// 方向の設定と待機処理の準備
		PreparNextMove(target - transform_.translate);
	}
}

///-------------------------------------------/// 
/// // 方向の設定と待機時間の設定
///-------------------------------------------///
void BaseEnemy::PreparNextMove(const Vector3& vector) {
	Vector3 dir = Normalize(vector);
	dir.y = 0.0f; // Y成分を0にしてXZ平面での方向ベクトルを作成
	moveInfo_.direction = Normalize(dir); // 方向を保存

	// 待機時間を設定
	moveInfo_.timer = moveInfo_.waitTime; // 待機時間を設定
	// 待機フラグをtrueに設定
	moveInfo_.isWating = true;
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
	if (Length(direction) < 0.001f) return;

	// forward方向からターゲットクォータニオンを作成
	Quaternion targetRotation = Math::LookRotation(direction, Vector3(0.0f, 1.0f, 0.0f));

	// SLerp補間
	Quaternion result = Math::SLerp(transform_.rotate, targetRotation, slerpT);
	transform_.rotate = Normalize(result); // 正規化でスケール崩れ防止

	// 回転が完了したかをチェック
	Quaternion diff = transform_.rotate - targetRotation;
	float rotationDiff = Math::Norm(diff);

	// 回転差分が十分小さければ完了フラグを立てる
	if (rotationDiff < 0.01f) {
		isRotationComplete_ = true;
	}
}

///-------------------------------------------/// 
/// 時間を進める
///-------------------------------------------///
void BaseEnemy::advanceTimer() {

	if (baseInfo_.isDead) {
		// 消えるまでの時間を進める
		disappearTimer_ -= baseInfo_.deltaTime;
		if (disappearTimer_ <= 0) {
			ParticleService::Emit("nakagawa", transform_.translate);
			isTentativeDeath_ = true;
		}
	} else {

		// 移動タイマーを進める
		moveInfo_.timer -= baseInfo_.deltaTime;

		// 攻撃用のタイマーを進める
		if (attackInfo_.timer > 0.0f) {
			attackInfo_.timer -= baseInfo_.deltaTime;
		}

		// ノックバッククールタイムの更新
		if (knockbackInfo_.isInCooldown) {
			knockbackInfo_.cooldownTimer -= baseInfo_.deltaTime;

			// クールタイムが終了したら解除
			if (knockbackInfo_.cooldownTimer <= 0.0f) {
				knockbackInfo_.isInCooldown = false;
				knockbackInfo_.cooldownTimer = 0.0f;
			}
		}

		// ヒット時の赤色表示タイマーの更新
		if (knockbackInfo_.hitColorTimer > 0.0f) {
			knockbackInfo_.hitColorTimer -= baseInfo_.deltaTime;

			// 赤色表示時間が終了したら元の色に戻す
			if (knockbackInfo_.hitColorTimer <= 0.0f) {
				color_ = knockbackInfo_.originalColor;
				knockbackInfo_.hitColorTimer = 0.0f;
			}
		}
	}
}