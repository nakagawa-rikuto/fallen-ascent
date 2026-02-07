#include "BossEnemy.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
BossEnemy::~BossEnemy() {
	// 状態を解放
	currentState_->Finalize();
	currentState_.reset();
	// Componentの解放
	moveComponent_.reset();
	attackComponent_.reset();
	// Object3Dの解放
	object3d_.reset();
}

///-------------------------------------------/// 
/// ゲームシーンで呼び出す初期化処理
///-------------------------------------------///
void BossEnemy::InitGameScene(const Vector3& translate) {

	/// ===BossEnemyの初期化=== ///
	Initialize();

	/// ===位置の設定=== ///
	transform_.translate = translate;

	/// ===初回更新=== ///
	UpdateAnimation();
}

///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void BossEnemy::Initialize() {

	/// ===Object3Dの初期化=== ///
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::AnimationModel, "Player");

	/// ===BaseEnemyの初期化=== ///
	BaseEnemy::Initialize();

	// Stateの設定
	//ChangeState(std::make_unique<EnemyMoveState>());
	
	// HP 
	baseInfo_.HP = 50;
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
}