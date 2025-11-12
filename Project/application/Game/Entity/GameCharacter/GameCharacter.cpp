#include "GameCharacter.h"
// DeltaTime
#include "Engine/System/Service/DeltaTimeSevice.h"
// ImGui
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

/// ===テンプレート候補=== ///
template class GameCharacter<OBBCollider>;
template class GameCharacter<SphereCollider>;

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
template<typename TCollider>
GameCharacter<TCollider>::~GameCharacter() {
	this->object3d_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::Initialize() {
	/// ===GameCharacterCollision=== ///
	collision_ = std::make_unique<GameCharacterCollision>();

	/// ===BaseInfoの初期化設定=== ///
	baseInfo_.velocity = { 0.0f, 0.0f, 0.0f };
	baseInfo_.deltaTime = DeltaTimeSevice::GetDeltaTime();
	baseInfo_.gravity = -9.8f;
	baseInfo_.isDead = false;

	/// ===OBBCollider=== ///
	TCollider::Initialize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::Update() {
	baseInfo_.deltaTime = DeltaTimeSevice::GetDeltaTime();

	/// ===位置の更新=== ///
	this->transform_.translate += baseInfo_.velocity;

	/// ===死亡処理=== ///
	if (baseInfo_.HP <= 0) {
		baseInfo_.isDead = true;
	}

	/// ===OBBCollider=== ///
	TCollider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::Draw(BlendMode mode) {
	/// ===OBBCollider=== ///
	TCollider::Draw(mode);
}

///-------------------------------------------/// 
/// ImGui
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::Information() {
#ifdef USE_IMGUI
	TCollider::Information();
	ImGui::Text("GameCharacterInfo");
	ImGui::DragFloat3("Velocity", &baseInfo_.velocity.x, 0.1f);
	ImGui::DragFloat("DeltaTime", &baseInfo_.deltaTime, 0.01f, 0.0f, 1.0f);
	ImGui::Checkbox("IsDead", &baseInfo_.isDead);
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 衝突
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::OnCollision(Collider* collider) {

	// === 早期リターン === //
	if (!collider) return;

	// 衝突相手が GameCharacter のときだけ処理が通る
	if (auto otherCharacter = dynamic_cast<GameCharacter<TCollider>*>(collider)) {
		collision_->ProcessCollision(this, otherCharacter, 1.0f);
	}

	// Wall と衝突した場合の処理
	if (collider->GetColliderName() == ColliderName::Wall) {
		collision_->HandleCharacterWallCollision(this, collider, &baseInfo_.velocity);
	}
}
