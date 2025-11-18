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
	/// ===ColliderCollision=== ///
	collision_ = std::make_unique<ColliderCollision>();
	gCollision_ = std::make_unique<GameCharacterCollision>();

	/// ===BaseInfoの初期化設定=== ///
	baseInfo_.velocity = { 0.0f, 0.0f, 0.0f };
	baseInfo_.deltaTime = DeltaTimeSevice::GetDeltaTime();
	baseInfo_.gravity = -9.8f;
	baseInfo_.isDead = false;
	baseInfo_.isGrounded = true;

	/// ===TCollider=== ///
	TCollider::Initialize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::Update() {
	// デルタタイムの取得
	baseInfo_.deltaTime = DeltaTimeSevice::GetDeltaTime();

	// 地面に接地していない場合のみ重力を適用
	if (!baseInfo_.isGrounded) {
		baseInfo_.velocity.y += baseInfo_.gravity * baseInfo_.deltaTime;
	}

	/// ===位置の更新=== ///
	this->transform_.translate += baseInfo_.velocity;

	/// ===死亡処理=== ///
	if (baseInfo_.HP <= 0) {
		baseInfo_.isDead = true;
	}

	/// ===OBBCollider=== ///
	TCollider::Update();

	// 先にColliderManagerの処理が入るから終わってからリセット
	//baseInfo_.isGrounded = false;
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
	ImGui::DragFloat("Gravity", &baseInfo_.gravity, 0.1f, -20.0f, 0.0f);
	ImGui::Checkbox("IsDead", &baseInfo_.isDead);
	ImGui::Checkbox("IsGrounded", &baseInfo_.isGrounded);
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 衝突
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::OnCollision(Collider* collider) {

	// === 早期リターン === //
	if (!collider) return;

	// 地面との衝突処理
	if (collider->GetColliderName() == ColliderName::Ground) {
		// 地面と衝突しているフラグを立てる
		baseInfo_.isGrounded = true;
		// 地面の高さを保存
		groundHeight_ = collider->GetTransform().translate.y;

		// 地面との衝突処理を即座に実行
		GroundCollision();

	} else if (auto otherCharacter = dynamic_cast<GameCharacter<TCollider>*>(collider)) { //　これだと当たり判定が通った時に全て通ってしまう。

		// GameCharacterの場合の衝突処理
		//NOTE: OBBだけどSphereの押し戻しと同じ処理
		gCollision_->ProcessCollision(this, otherCharacter, 0.0f);

	} else if (collider->GetColliderName() == ColliderName::Object) {
		// 地面と衝突しているフラグを立てる
		baseInfo_.isGrounded = true;
		// StageObjectとの衝突を処理
		collision_->ProcessCollision(this, collider, 0.0f);
	}
}

///-------------------------------------------/// 
/// 地面衝突処理
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::GroundCollision() {
	// 接地していない場合は処理しない
	if (!baseInfo_.isGrounded) return;

	float effectiveGroundY;

	// コライダーの半径を考慮した地面の高さ
	if (this->GetColliderType() == ColliderType::OBB) {
		// OBBの場合は半サイズを使用
		GameCharacter<OBBCollider>* obb = dynamic_cast<GameCharacter<OBBCollider>*>(this);
		effectiveGroundY = groundHeight_ + obb->GetOBB().halfSize.y;
	} else {
		// Sphereの場合は半径を使用
		GameCharacter<SphereCollider>* sphere = dynamic_cast<GameCharacter<SphereCollider>*>(this);
		effectiveGroundY = groundHeight_ + sphere->GetSphere().radius;
	}

	// キャラクターが地面より下にいる場合、地面の高さに補正
	if (this->transform_.translate.y < effectiveGroundY) {
		this->transform_.translate.y = effectiveGroundY;
		// Y方向の速度をリセット
		if (baseInfo_.velocity.y < 0.0f) {
			baseInfo_.velocity.y = 0.0f;
		}
	}
}