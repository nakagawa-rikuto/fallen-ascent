#include "CloseRangeEnemy.h"
// Strategy
#include "application/Game/Entity/Enemy/Component/Strategy/CloseRangeAttackStrategy.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// Service
#include "Engine/System/Service/ParticleService.h"
#include "Engine/System/Service/ColliderService.h"
// c++
#include <cstdlib>
#include <ctime>
// ImGui
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
CloseRangeEnemy::~CloseRangeEnemy() {
	ColliderService::RemoveCollider(this);
	object3d_.reset();
}

///-------------------------------------------/// 
/// GameScene用初期化
///-------------------------------------------///
void CloseRangeEnemy::InitGameScene(const Vector3& translate) {
	// CloseRangeEnemyの初期化
	Initialize();
	// 位置の設定
	transform_.translate = translate;
	
	/// ===BaseEnemy初期化=== ///
	BaseEnemy::Initialize();

	/// ===近距離攻撃の作成=== ///
	auto closeStrategy = std::make_unique<CloseRangeAttackStrategy>();
	CloseRangeAttackStrategy::Config strategyConfig{
		.chargeSpeed = 0.9f,
		.attackDuration = 1.0f,
		.stopTime = 0.5f
	};
	closeStrategy->Initialize(strategyConfig);

	/// ===AttackConfigの作成=== ///
	EnemyAttackComponent::AttackConfig attackConfig {
		.range = 4.0f,
		.distance = 15.0f,
		.interval = 6.0f,
		.power = 1
	};

	// 攻撃コンポーネントに戦略を設定
	InitializeAttackComponent(attackConfig, std::move(closeStrategy));
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
/// 派生用の拡張ポイント
///-------------------------------------------///
void CloseRangeEnemy::CopyTypeTuningFromThisTo(BaseEnemy* dst) const {
	dst;
}
