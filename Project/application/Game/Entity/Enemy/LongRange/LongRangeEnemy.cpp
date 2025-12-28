#include "LongRangeEnemy.h"
// Player
#include "application/Game/Entity/Player/Player.h"
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
	// Bulletのクリア
	bulletInfo_.bullets_.clear();
}

///-------------------------------------------/// 
/// GameScene用初期化
///-------------------------------------------///
void LongRangeEnemy::InitGameScene(const Vector3& translate) {
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
void LongRangeEnemy::Initialize() {

	/// ===Object3d=== ///
	// Object3dの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "player");

	/// ===GameCharacter=== ///
	// GameCharacterの初期化
	GameCharacter::Initialize();
	name_ = ColliderName::Enemy;

	/// ===AttackInfoの設定=== ///
	attackInfo_.range = 3.0f;
	attackInfo_.distance = 30.0f;
	attackInfo_.interval = 8.0f;
	attackInfo_.timer = 1.0;
	attackInfo_.power = 1;
	attackInfo_.direction = { 0.0f, 0.0f, 0.0f };
	attackInfo_.isAttack = false;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void LongRangeEnemy::Update() {

	// BaseEnemyの更新
	BaseEnemy::Update();

	// 弾を更新（null ならスキップ）
	for (auto& bullet : bulletInfo_.bullets_) {
		if (bullet) bullet->Update();
	}

	// 死んだ弾をリストから削除
	bulletInfo_.bullets_.remove_if(
		[](const std::unique_ptr<LongRangeEnemeyBullet>& b) {
			return !b || !b->GetIsAlive();
		}
	);
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void LongRangeEnemy::Draw(BlendMode mode) {
	// Enemyの描画
	BaseEnemy::Draw(mode);

	// バレットの描画
	for (auto& bullet : bulletInfo_.bullets_) {
		if (bullet) bullet->Draw(mode);
	}
}

///-------------------------------------------/// 
/// 更新（ImGui）
///-------------------------------------------///
void LongRangeEnemy::Information() {
#ifdef USE_IMGUI
	ImGui::Begin("LongRangeEnemy");
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
/// 攻撃処理の初期化
///-------------------------------------------///
void LongRangeEnemy::StartAttack() {
	// 攻撃開始
	attackInfo_.isAttack = true;
	// ターゲットの位置を保存
	attackInfo_.playerPos = player_->GetTransform().translate;
	// 攻撃時間の設定
	attackInfo_.timer = 1.0f;
}

///-------------------------------------------/// 
/// 攻撃処理
///-------------------------------------------///
void LongRangeEnemy::Attack() {
	// 早期リターン
	if (!player_) return;

	// 弾の生成
	auto bullet = std::make_unique<LongRangeEnemeyBullet>();
	bullet->Create(transform_.translate, attackInfo_.direction);
	bulletInfo_.bullets_.push_back(std::move(bullet));

	// フラグをfalse
	attackInfo_.isAttack = false;
	// クールダウン再設定
	attackInfo_.timer = attackInfo_.interval; 
}

///-------------------------------------------/// 
/// 派生用の拡張ポイント
///-------------------------------------------///
void LongRangeEnemy::CopyTypeTuningFromThisTo(BaseEnemy* dst) const {
	if (auto* d = dynamic_cast<LongRangeEnemy*>(dst)) {
		d->bulletInfo_.interval = this->bulletInfo_.interval;
		d->bulletInfo_.reloadTime = this->bulletInfo_.reloadTime;
		// isShot / isHit / bullets_ はランタイム値なのでコピーしない
	}
}
