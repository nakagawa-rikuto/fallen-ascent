#include "PlayerWeapon.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"
// Service
#include "Engine/System/Service/ColliderService.h"
#include "Engine/System/Service/DeltaTimeSevice.h"

// ImGui
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
PlayerWeapon::~PlayerWeapon() {
	object3d_.reset();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
bool PlayerWeapon::GetIsAttack() const {return attackInfo_.isAttacking;}
float PlayerWeapon::GetAttackProgress() const {return attackInfo_.progress;}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// アクティブ設定
void PlayerWeapon::SetActive(bool flag) { baseInfo_.isActive = flag; }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void PlayerWeapon::Initialize() {

	// Object3dの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "MonsterBall");

	/// ===OBBCollider=== ///
	OBBCollider::Initialize();
	name_ = ColliderName::PlayerWeapon;
	obb_.halfSize = { 0.5f, 0.5f, 3.0f };

	// DeltaTime初期化
	baseInfo_.deltaTime = DeltaTimeSevice::GetDeltaTime();

	// 初期状態では非アクティブ
	SetActive(false);
	attackInfo_.isAttacking = false;
	attackInfo_.isChargeAttack = false;

	// 更新
	Update();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void PlayerWeapon::Update() {

	// DeltaTime更新
	baseInfo_.deltaTime = DeltaTimeSevice::GetDeltaTime();

	// 攻撃中でない場合は早期リターン
	if (!attackInfo_.isAttacking && !attackInfo_.isChargeAttack) {
		SetActive(false);
		OBBCollider::Update();
		return;
	}

	// タイマーを進める
	attackInfo_.timer += baseInfo_.deltaTime;
	attackInfo_.progress = attackInfo_.timer / attackInfo_.duration;

	// 攻撃終了チェック
	if (attackInfo_.progress >= 1.0f) {
		attackInfo_.isAttacking = false;
		attackInfo_.isChargeAttack = false;
		attackInfo_.progress = 1.0f;
		SetActive(false);
		ColliderService::RemoveCollider(this);
		OBBCollider::Update();
		return;
	}

	// 攻撃軌道の更新
	if (attackInfo_.isAttacking) {
		UpdateAttackTrajectory();
	} else if (attackInfo_.isChargeAttack) {
		UpdateChargeAttackTrajectory();
	}
	

	/// ===OBBCollider=== ///
	OBBCollider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void PlayerWeapon::Draw(BlendMode mode) {
	// 攻撃中のみ描画
	if (attackInfo_.isAttacking || attackInfo_.isChargeAttack) {
		OBBCollider::Draw(mode);
	}
}

///-------------------------------------------/// 
/// ImGui
///-------------------------------------------///
void PlayerWeapon::Information() {
#ifdef USE_IMGUI
	ImGui::Begin("PlayerWeapon");
	ImGui::Checkbox("IsAttacking", &attackInfo_.isAttacking);
	ImGui::Checkbox("IsChargeAttacking", &attackInfo_.isChargeAttack);
	ImGui::DragFloat("Progress", &attackInfo_.progress, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("Timer", &attackInfo_.timer, 0.01f);
	ImGui::DragFloat("Duration", &attackInfo_.duration, 0.01f);
	ImGui::Text("Arc Motion:");
	ImGui::DragFloat3("Center", &attackInfo_.arcCenter.x, 0.1f);
	ImGui::DragFloat("Radius", &attackInfo_.arcRadius, 0.1f);
	ImGui::DragFloat("Start Angle", &attackInfo_.startAngle, 1.0f);
	ImGui::DragFloat("End Angle", &attackInfo_.endAngle, 1.0f);
	//OBBCollider::Information();
	ImGui::End();
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 衝突判定
///-------------------------------------------///
void PlayerWeapon::OnCollision(Collider* collider) {
	// 攻撃中でない、または既にヒット済みの場合は処理しない
	if (!baseInfo_.isActive) {
		return;
	}

	// 敵に当たった場合
	if (collider->GetColliderName() == ColliderName::Enemy) {
		// ヒットフラグを立てる（1回の攻撃で複数ヒットを防ぐ）
		attackInfo_.hasHit = true;

		// ここでヒットエフェクトやサウンドの再生などを行う
	}
}

///-------------------------------------------/// 
/// 親子関係の設定
///-------------------------------------------///
void PlayerWeapon::SetUpParent(Player* parent) {
	player_ = parent;

	object3d_->SetParent(player_->GetModelCommon());
	//object3d_->SetParentOffset({ 0.0f, 0.0f, 2.0f });
}

///-------------------------------------------/// 
/// 攻撃開始処理
///-------------------------------------------///
void PlayerWeapon::StartAttack(
	const Vector3& startPoint,
	const Vector3& endPoint,
	float duration,
	const Quaternion& startRotation,
	const Quaternion& endRotation) {
	// 攻撃情報の設定
	attackInfo_.isAttacking = true;
	attackInfo_.timer = 0.0f;
	attackInfo_.duration = duration;
	attackInfo_.progress = 0.0f;

	attackInfo_.startPoint = startPoint;
	attackInfo_.endPoint = endPoint;
	attackInfo_.startRotation = startRotation;
	attackInfo_.endRotation = endRotation;

	// コライダーに追加
	ColliderService::AddCollider(this);

	// 扇形の軌道計算のため、円弧の中心と半径を算出
	CalculateArcParameters();

	// 当たり判定フラグをリセット
	attackInfo_.hasHit = false;

	// コライダーを有効化
	SetActive(true);

	// 初期位置を設定
	transform_.translate = startPoint;
	transform_.rotate = startRotation;
}

///-------------------------------------------/// 
/// チャージ攻撃開始処理
///-------------------------------------------///
void PlayerWeapon::StartChargeAttack(
	const Vector3& startPoint,
	const Vector3& endPoint,
	float duration,
	const Quaternion& startRotation,
	const Quaternion& endRotation) {
	// 攻撃情報の設定
	attackInfo_.isChargeAttack = true;
	attackInfo_.timer = 0.0f;
	attackInfo_.duration = duration;
	attackInfo_.progress = 0.0f;

	attackInfo_.startPoint = startPoint;
	attackInfo_.endPoint = endPoint;
	attackInfo_.startRotation = startRotation;
	attackInfo_.endRotation = endRotation;

	// コライダーに追加
	ColliderService::AddCollider(this);

	// 当たり判定フラグをリセット
	attackInfo_.hasHit = false;

	// コライダーを有効化
	SetActive(true);

	// 初期位置を設定
	transform_.translate = startPoint;
	transform_.rotate = startRotation;
}

///-------------------------------------------/// 
/// 扇形の軌道パラメータを計算
///-------------------------------------------///
void PlayerWeapon::CalculateArcParameters() {
	
	// 開始点と終了点の中点
	Vector3 midPoint = (attackInfo_.startPoint + attackInfo_.endPoint) * 0.5f;
	
	// 開始点から終了点へのベクトル
	Vector3 startToEnd = attackInfo_.endPoint - attackInfo_.startPoint;
	float chordLength = Length(startToEnd);
	
	// 弦の方向ベクトル（正規化）
	Vector3 chordDir = Normalize(startToEnd);
	
	// 上方向ベクトル
	Vector3 up = { 0.0f, 1.0f, 0.0f };
	
	// 弦に垂直な方向ベクトル
	Vector3 perpendicular = Math::Cross(up, chordDir);
	perpendicular = Normalize(perpendicular);
	
	// 扇形の半径を計算
	float halfChord = chordLength * 0.5f;
	
	// 扇形の開き角度（120度）から適切な半径を計算
	float halfAngleRad = 60.0f * Math::Pi() / 180.0f; // 60度
	
	// sin(60°) = 0.866、この関係から半径を逆算
	attackInfo_.arcRadius = halfChord / std::sin(halfAngleRad);
	
	// 扇形の中心位置を計算
	float centerOffset = attackInfo_.arcRadius * std::cos(halfAngleRad);
	
	// perpendicularの向きを確認して、プレイヤー側（扇形の頂点側）に配置
	attackInfo_.arcCenter = midPoint + perpendicular * centerOffset;
	
	// 開始角度と終了角度を計算（Y軸周りの角度）
	Vector3 startDir = Normalize(attackInfo_.startPoint - attackInfo_.arcCenter);
	Vector3 endDir = Normalize(attackInfo_.endPoint - attackInfo_.arcCenter);
	
	// XZ平面での角度を計算（atan2を使用）
	attackInfo_.startAngle = std::atan2(startDir.x, startDir.z);
	attackInfo_.endAngle = std::atan2(endDir.x, endDir.z);
	
	// 角度の範囲を正規化（最短経路を選択）
	float angleDiff = attackInfo_.endAngle - attackInfo_.startAngle;

	// [-π, π] の範囲に正規化（fmod を使用）
	angleDiff = std::fmod(angleDiff + Math::Pi(), 2.0f * Math::Pi());
	angleDiff -= Math::Pi();

	// endAngle を調整
	attackInfo_.endAngle = attackInfo_.startAngle + angleDiff;
}

///-------------------------------------------/// 
/// 攻撃軌道の更新
///-------------------------------------------///
void PlayerWeapon::UpdateAttackTrajectory() {
	// イージング関数を適用（EaseInOutQuad で自然な加速・減速）
	float t = attackInfo_.progress;
	float easedT = Easing::EaseInOutQuad(t);

	// 扇形の軌道に沿って移動：円弧上の角度を補間
	float currentAngle = Math::Lerp(attackInfo_.startAngle, attackInfo_.endAngle, easedT);
	
	// 円弧上の位置を計算
	Vector3 offset;
	offset.x = std::sin(currentAngle) * attackInfo_.arcRadius;
	offset.y = 0.0f;
	offset.z = std::cos(currentAngle) * attackInfo_.arcRadius;
	
	// Y座標は開始点と終了点を線形補間
	float currentY = Math::Lerp(
		attackInfo_.startPoint.y, 
		attackInfo_.endPoint.y, 
		easedT
	);
	
	// 最終的な位置を計算（円弧の中心 + オフセット）
	transform_.translate = attackInfo_.arcCenter + offset;
	transform_.translate.y = currentY;
	
	// 武器を常に円弧の中心を向くように回転
	Vector3 dirToCenter = Normalize(attackInfo_.arcCenter - transform_.translate);
	Vector3 up(0.0f, 1.0f, 0.0f);
	
	// 中心を向く基本回転
	Quaternion lookRotation = Math::LookRotation(dirToCenter, up);
	
	// 開始回転と終了回転の補間（滑らかな遷移）
	Quaternion interpolatedRot = Math::SLerp(
		attackInfo_.startRotation,
		attackInfo_.endRotation,
		easedT
	);
	
	// 両方の回転を合成
	transform_.rotate = Multiply(lookRotation, interpolatedRot);
}

///-------------------------------------------/// 
/// チャージ攻撃軌道の更新
///-------------------------------------------///
void PlayerWeapon::UpdateChargeAttackTrajectory() {
	// 勢いのあるイージング関数を適用（EaseInCubicで加速感を出す）
	float t = attackInfo_.progress;
	// 前半はゆっくり、後半は勢いよく振り下ろす
	float easedT = t * t * t; // EaseInCubic

	// まっすぐ線形補間で移動（扇形ではなく直線）
	transform_.translate = Math::Lerp(
		attackInfo_.startPoint,
		attackInfo_.endPoint,
		easedT
	);

	// 回転も線形補間（滑らかに回転）
	transform_.rotate = Math::SLerp(
		attackInfo_.startRotation,
		attackInfo_.endRotation,
		easedT
	);
}

