#include "LongRangeEnemy.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// Strategy
#include "application/Game/Entity/Enemy/Component/Strategy/LongRangeAttackStrategy.h"
// Service
#include "Engine/System/Service/ParticleService.h"


// ImGui
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
LongRangeEnemy::~LongRangeEnemy() {
	object3d_.reset();
}

///-------------------------------------------/// 
/// GameScene用初期化
///-------------------------------------------///
void LongRangeEnemy::InitGameScene(const Vector3& translate) {
	// CloseRangeEnemyの初期化
	Initialize();
	// 位置の設定
	transform_.translate = translate;

	/// ===BaseEnemy初期化=== ///
	BaseEnemy::Initialize();
	
	/// ===遠距離攻撃の作成=== ///
	auto longStrategy = std::make_unique<LongRangeAttackStrategy>();
	LongRangeAttackStrategy::Config strategyConfig{
		.shootInterval = 0.5f,
		.reloadTime = 3.0f,
		.maxBullets = 3
	};
	longStrategy->Initialize(strategyConfig);
	longRangeStrategy_ = longStrategy.get(); // ポインタを保存

	/// ===AttackConfigの作成=== ///
	EnemyAttackComponent::AttackConfig attackConfig {
		.range = 3.0f,
		.distance = 30.0f,
		.interval = 8.0f,
		.power = 1
	};

	// 攻撃コンポーネントに戦略を設定
	InitializeAttackComponent(attackConfig, std::move(longStrategy));
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void LongRangeEnemy::Initialize() {

	/// ===Object3d=== ///
	// Object3dの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "player");

	/// ===GameCharacter=== ///
	// GameCharacterの初期化
	GameCharacter::Initialize();
	name_ = ColliderName::Enemy;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void LongRangeEnemy::Update() {
	// BaseEnemyの更新
	BaseEnemy::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void LongRangeEnemy::Draw(BlendMode mode) {
	// Enemyの描画
	BaseEnemy::Draw(mode);

	// 弾の描画
	if (longRangeStrategy_) {
		longRangeStrategy_->DrawBullets();
	}
}

///-------------------------------------------/// 
/// 更新（ImGui）
///-------------------------------------------///
void LongRangeEnemy::Information() {
#ifdef USE_IMGUI
	ImGui::Begin("遠距離攻撃のEnemy");
	BaseEnemy::Information();
	ImGui::End();
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 衝突判定
///-------------------------------------------///
void LongRangeEnemy::OnCollision(Collider* collider) {
	// BaseEnemyの当たり判定
	BaseEnemy::OnCollision(collider);
}

///-------------------------------------------/// 
/// 派生用の拡張ポイント
///-------------------------------------------///
void LongRangeEnemy::CopyTypeTuningFromThisTo(BaseEnemy* dst) const {
	auto* longEnemy = dynamic_cast<LongRangeEnemy*>(dst);
	if (!longEnemy) return;

	// 遠距離攻撃戦略の設定をコピー
	auto* srcStrategy = dynamic_cast<LongRangeAttackStrategy*>(attackComponent_->GetStrategy());
	auto* dstStrategy = dynamic_cast<LongRangeAttackStrategy*>(longEnemy->attackComponent_->GetStrategy());

	if (srcStrategy && dstStrategy) {
		dstStrategy->SetConfig(srcStrategy->GetConfig());
	}
}
