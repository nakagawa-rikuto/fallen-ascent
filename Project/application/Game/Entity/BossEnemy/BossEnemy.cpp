#include "BossEnemy.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
BossEnemy::~BossEnemy() {
	// 状態を解放
	currentState_->Finalize();
	currentState_.reset();
	object3d_.reset();
}

///-------------------------------------------/// 
/// ゲームシーンで呼び出す初期化処理
///-------------------------------------------///
void BossEnemy::InitGameScene(const Vector3& translate) {
	// 位置の設定
	transform_.translate = translate;
}

///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void BossEnemy::Initialize() {
	// GameCharacterの初期化
	GameCharacter::Initialize();
	// Stateの設定
	//ChangeState(std::make_unique<EnemyMoveState>());
	// HP 
	baseInfo_.HP = 50;
	// object3dの更新を一回行う
	UpdateAnimation();
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void BossEnemy::Update() {
	// GameCharacterの更新
	GameCharacter::Update();
	// Stateの更新
	if (currentState_) {
		currentState_->Update();
	}
}

///-------------------------------------------/// 
/// アニメーション時の更新処理
///-------------------------------------------///
void BossEnemy::UpdateAnimation() {
	// GameCharacterの更新
	GameCharacter::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void BossEnemy::Draw(BlendMode mode) {
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
void BossEnemy::OnCollision(Collider* collider) {
	/// ===GameCharacterの衝突=== ///
	GameCharacter::OnCollision(collider);
}