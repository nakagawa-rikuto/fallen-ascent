#include "BossEnemy.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// Service
#include "Service/Particle.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
BossEnemy::~BossEnemy() {
	// 状態を解放
	//currentState_->Finalize();
	//currentState_.reset();
	// Componentの解放
	//moveComponent_.reset();
	//attackComponent_.reset();
	// Object3Dの解放
}

///-------------------------------------------/// 
/// ゲームシーンで呼び出す初期化処理
///-------------------------------------------///
void BossEnemy::InitGameScene(const Vector3& translate) {

	/// ===BossEnemyの初期化=== ///
	Initialize();

	/// ===位置の設定=== ///
	transform_.translate = translate;
}

///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void BossEnemy::Initialize() {

	/// ===Object3Dの初期化=== ///
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "Boss");

	/// ===BaseEnemyの初期化=== ///
	BaseEnemy::Initialize();

	// Stateの設定
	//ChangeState(std::make_unique<EnemyMoveState>());
	
	// 無敵時間
	invincibleInfo_.time = 0.5f;

	// HP 
	baseInfo_.HP = 3;
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void BossEnemy::Update() {
	// 死亡している場合は更新しない
	if (isTentativeDeath_) return;

	/// ===Timerの更新=== ///
	advanceTimer();

	// Stateの更新
	/*if (currentState_) {
		currentState_->Update();
	}*/

	UpdateAnimation();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void BossEnemy::Draw(MiiEngine::BlendMode mode) {
	/// ===GameCharacterの描画=== ///
	GameCharacter::Draw(mode);
}

///-------------------------------------------/// 
/// ImGui情報の表示
///-------------------------------------------///
void BossEnemy::Information() {
	/// ===GameCharacterの情報表示=== ///
	GameCharacter::Information();
}

///-------------------------------------------/// 
/// 衝突処理
///-------------------------------------------///
void BossEnemy::OnCollision(MiiEngine::Collider* collider) {
	/// ===GameCharacterの衝突=== ///
	GameCharacter::OnCollision(collider);

	/// ===Playerとの衝突=== ///
	// Weaponとの当たり判定
	//if (collider->GetColliderName() == MiiEngine::ColliderName::PlayerWeapon) {
	//	// クールタイム中でなければノックバック処理を実行
	//	if (!invincibleInfo_.isInvincible) {
	//		// 通常攻撃の時
	//		if (player_->GetAttackComponent()->IsAttacking()) {

	//			// HPを減少
	//			baseInfo_.HP--;
	//			hitParticle_ = Service::Particle::Emit("Game", transform_.translate);

	//			// 無敵時間のセット
	//			SetInvincibleTime();
	//		}
	//	}
	//}
}

///-------------------------------------------/// 
/// 時間を進める
///-------------------------------------------///
void BossEnemy::advanceTimer() {
	if (baseInfo_.isDead) {
		// パーティクルの発生
		deathParticle_ = Service::Particle::Emit("nakagawa", transform_.translate);
		isTentativeDeath_ = true;
		if (hitParticle_ != nullptr) {
			hitParticle_->Stop();
			hitParticle_ = nullptr;
		}
	} else {
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