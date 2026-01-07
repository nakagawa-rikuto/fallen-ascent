#include "CloseRangeEnemy.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// Service
#include "Engine/System/Service/ParticleService.h"
// c++
#include <cstdlib>
// ImGui
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
CloseRangeEnemy::~CloseRangeEnemy() {}

///-------------------------------------------/// 
/// GameScene用初期化
///-------------------------------------------///
void CloseRangeEnemy::InitGameScene(const Vector3& translate) {
	// CloseRangeEnemyの初期化
	Initialize();
	// 位置の設定
	transform_.translate = translate;
	// BaseEnemyの初期化
	BaseEnemy::Initialize();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void CloseRangeEnemy::Initialize() {

	/// ===Object=== ///
	// Object3dの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "player");

	/// ===GameCharacter=== ///
	// GameCharacterの初期化
	GameCharacter::Initialize();
	name_ = ColliderName::Enemy;

	/// ===AttackInfoの設定=== ///
	attackInfo_.range = 4.0f;
	attackInfo_.distance = 15.0f;
	attackInfo_.interval = 6.0f;
	attackInfo_.timer = 0.0f;
	attackInfo_.power = 1;
	attackInfo_.direction = { 0.0f, 0.0f, 0.0f };
	attackInfo_.isAttack = false;

	/// ===ChargeInfo=== ///
	chargeInfo_.moveSpeed = 0.9f;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void CloseRangeEnemy::Update() {
	// BaseEnemyの更新
	BaseEnemy::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void CloseRangeEnemy::Draw(BlendMode mode) {
	// BaseEnemyの描画
	BaseEnemy::Draw(mode);
}

///-------------------------------------------/// 
/// 更新（ImGui）
///-------------------------------------------///
void CloseRangeEnemy::Information() {
#ifdef USE_IMGUI
	ImGui::Begin("近距離攻撃のEnemy");
	BaseEnemy::Information();
	ImGui::Text("突進情報");
	ImGui::DragFloat("移動速度", &chargeInfo_.moveSpeed, 0.1f);
	ImGui::End();
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 衝突判定
///-------------------------------------------///
void CloseRangeEnemy::OnCollision(Collider* collider) {
	// BaseEnemyの当たり判定
	BaseEnemy::OnCollision(collider);
}

///-------------------------------------------/// 
/// 攻撃処理の初期化
///-------------------------------------------///
void CloseRangeEnemy::StartAttack() {
	// 攻撃開始
	attackInfo_.isAttack = true;
	// 攻撃時間の設定
	attackInfo_.timer = 1.0f;
	// 移動ベクトルを設定
	baseInfo_.velocity = attackInfo_.direction * chargeInfo_.moveSpeed;
	// パーティクル停止
	if (attackParticle_) {
		attackParticle_->Stop();
		attackParticle_ = nullptr;
	}
	// パーティクルの再生
	attackParticle_ = ParticleService::Emit("CloseEnemyAttack", transform_.translate);
	attackParticle_->SetEmitterPosition(transform_.translate);
}

///-------------------------------------------/// 
/// 攻撃処理
///-------------------------------------------///
void CloseRangeEnemy::Attack() {

	// プレイヤーとの差を計算
	Vector3 toTarget = attackInfo_.playerPos - transform_.translate;
	float length = Length(toTarget);

	// パーティクルの位置更新
	if (attackParticle_) {
		attackParticle_->SetEmitterPosition(transform_.translate);
	}

	// 攻撃終了判定
	if (length < 0.5f) { // 到達判定

		// 攻撃終了フラグをfalse
		attackInfo_.isAttack = false;
		// 移動ベクトルをリセット
		baseInfo_.velocity = { 0.0f, 0.0f, 0.0f };
		// クールダウン再設定
		attackInfo_.timer = attackInfo_.interval;
	}
}

///-------------------------------------------/// 
/// 派生用の拡張ポイント
///-------------------------------------------///
void CloseRangeEnemy::CopyTypeTuningFromThisTo(BaseEnemy* dst) const {
	if (auto* d = dynamic_cast<CloseRangeEnemy*>(dst)) {
		d->chargeInfo_.moveSpeed = this->chargeInfo_.moveSpeed;
	}
}
