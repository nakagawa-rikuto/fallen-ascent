#include "LongRangeEnemyBullet.h"
// Service
#include "Engine/System/Service/ParticleService.h"
#include "Engine/System/Service/ColliderService.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
LongRangeEnemyBullet::~LongRangeEnemyBullet() {
	ColliderService::RemoveCollider(this);
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void LongRangeEnemyBullet::Initialize() {
	// Object3dの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "player");
	// Object3dの初期設定
	object3d_->SetTranslate(transform_.translate);

	// Sphereの設定
	SphereCollider::Initialize();
	name_ = ColliderName::EnemyBullet;
	SphereCollider::SetRadius(0.5f);

	// コライダーに追加
	ColliderService::AddCollider(this);

	object3d_->Update();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void LongRangeEnemyBullet::Update() {
	if (isAlive_) {
		/// ===移動処理=== ///
		Move();

		/// ===タイマー=== ///
		PromoteTimer();

		// 位置更新
		transform_.translate += info_.velocity;
		
		// Particleの位置更新
		if (bulletParticle_) {
			bulletParticle_->SetEmitterPosition(transform_.translate);
		}

		// コライダーの更新
		SphereCollider::Update();
	}
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void LongRangeEnemyBullet::Draw(BlendMode mode) {
	if (isAlive_) {
		SphereCollider::Draw(mode);
	}
}

///-------------------------------------------/// 
/// 生成
///-------------------------------------------///
void LongRangeEnemyBullet::Create(const Vector3& pos, const Vector3& vel) {
	// 初期化処理
	Initialize();
	// 位置と方向の設定
	transform_.translate = pos;
	info_.direction = vel;

	// 生存フラグをtrue
	isAlive_ = true;
	// 生存時間の設定
	lifeTimer_ = lifeTime_;

	// パーティクル停止
	if (bulletParticle_) {
		bulletParticle_->Stop();
		bulletParticle_ = nullptr;
	}
	// パーティクルの再生
	bulletParticle_ = ParticleService::Emit("LongEnemyAttack", transform_.translate);
	bulletParticle_->SetEmitterPosition(transform_.translate);
}

///-------------------------------------------///  
/// 衝突判定
///-------------------------------------------///
void LongRangeEnemyBullet::OnCollision(Collider* collider) {
	collider;
}

///-------------------------------------------/// 
/// 移動処理
///-------------------------------------------///
void LongRangeEnemyBullet::Move() {
	info_.velocity = info_.direction * speed_;
}

///-------------------------------------------/// 
/// タイマーを進める
///-------------------------------------------///
void LongRangeEnemyBullet::PromoteTimer() {
	// ライフタイマーの減少
	lifeTimer_ -= 1.0f / 60.0f;
	if (lifeTimer_ <= 0.0f) {
		// 生存時間が0.0fになったら削除。
		isAlive_ = false;
	}
}