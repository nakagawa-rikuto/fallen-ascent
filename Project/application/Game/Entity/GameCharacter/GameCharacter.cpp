#include "GameCharacter.h"
// DeltaTime
#include "Engine/System/Service/DeltaTimeSevice.h"
#include "Engine/Collider/AABBCollider.h"
// C++
#include <algorithm>
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

	/// ===GroundInfo=== ///
	groundInfo_.isGrounded = false;
	groundInfo_.hasGroundCollision = false;

	/// ===TCollider=== ///
	TCollider::Initialize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::Update() {

	/// ===デルタタイムの取得=== ///
	baseInfo_.deltaTime = DeltaTimeSevice::GetDeltaTime();

	/// ===地面との衝突処理=== ///
	GroundCollision();

	/// ===位置の更新=== ///
	this->transform_.translate += baseInfo_.velocity;

	/// ===死亡処理=== ///
	if (baseInfo_.HP <= 0 || this->transform_.translate.y < -50.0f) {
		baseInfo_.isDead = true;
	}

	/// ===TCollider=== ///
	TCollider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::Draw(BlendMode mode) {
	/// ===TCollider=== ///
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
	ImGui::Checkbox("IsGrounded", &groundInfo_.isGrounded);
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 衝突
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::OnCollision(Collider* collider) {

	// === 早期リターン === //
	if (!collider) return;

	/// ===Colliderとの衝突処理=== ///
	if (collider->GetColliderName() == ColliderName::Ground) {

		// isGroundedがfalseなら
		if (!groundInfo_.isGrounded) {
			// 地面に接地したとみなす
			groundInfo_.isGrounded = true;
			groundInfo_.hasGroundCollision = true;

			// 地面の情報を保存
			if (collider->GetColliderType() == ColliderType::AABB) {
				// 中心点と半径サイズを取得
				AABBCollider* aabbCollider = dynamic_cast<AABBCollider*>(collider);
				if (aabbCollider) {
					AABB groundAABB = aabbCollider->GetAABB();
					groundInfo_.currentGroundCenter = (groundAABB.min + groundAABB.max) * 0.5f;
					groundInfo_.currentGroundHalfSize = (groundAABB.max - groundAABB.min) * 0.5f;
				}
			} else if (collider->GetColliderType() == ColliderType::OBB) {
				// 中心点と半径サイズを取得
				OBBCollider* obbCollider = dynamic_cast<OBBCollider*>(collider);
				if (obbCollider) {
					OBB groundOBB = obbCollider->GetOBB();
					groundInfo_.currentGroundCenter = groundOBB.center;
					groundInfo_.currentGroundHalfSize = groundOBB.halfSize;
				}
			}
		}

	} else if (collider->GetColliderName() == ColliderName::Object) {
		// Objectとの衝突処理
		//NOTE:thisを100%押し戻し
		collision_->ProcessCollision(this, collider, 0.0f);

	} else if (auto otherCharacter = dynamic_cast<GameCharacter<TCollider>*>(collider)) { //　これだと当たり判定が通った時に全て通ってしまう。

		// GameCharacterの場合の衝突処理
		//NOTE: OBBだけどSphereの押し戻しと同じ処理
		gCollision_->ProcessCollision(this, otherCharacter, 0.0f);
	}
}

///-------------------------------------------/// 
/// 地面との衝突処理
///-------------------------------------------///
template<typename TCollider>
void GameCharacter<TCollider>::GroundCollision() {

	/// ===地面から離れている場合の処理=== ///
	if (!groundInfo_.isGrounded) {
		// 重力の適用
		baseInfo_.velocity.y += baseInfo_.gravity * baseInfo_.deltaTime;

		// Y方向の速度の最大値を制限
		const float kMaxFollSpeed = -10.0f;
		// 下方向への速度を制限
		baseInfo_.velocity.y = std::clamp(baseInfo_.velocity.y, kMaxFollSpeed, 0.0f);
	}

	/// ===早期リターン=== ///
	if (!groundInfo_.hasGroundCollision) return;

	/// ===Y座標が地面を下回っているかををチェック=== ///
	float groundY = 0.0f;
	float characterHalfHeight = 0.0f;

	if (this->GetColliderType() == ColliderType::OBB) {
		OBB obb = dynamic_cast<OBBCollider*>(this)->GetOBB();
		characterHalfHeight = obb.halfSize.y;
		groundY = groundInfo_.currentGroundCenter.y + groundInfo_.currentGroundHalfSize.y + characterHalfHeight;
	} else if (this->GetColliderType() == ColliderType::Sphere) {
		Sphere sphere = dynamic_cast<SphereCollider*>(this)->GetSphere();
		characterHalfHeight = sphere.radius;
		groundY = groundInfo_.currentGroundCenter.y + groundInfo_.currentGroundHalfSize.y + characterHalfHeight;
	}

	// はみ出し分を計算
	float overlap = groundY - this->transform_.translate.y;

	// 地面より下に行かないようにする
	if (this->transform_.translate.y < groundY) {

		// はみ出し分を押し戻す
		this->transform_.translate.y += overlap;

		// 下降中なら速度を0にする
		if (baseInfo_.velocity.y < 0.0f) {
			baseInfo_.velocity.y = 0.0f;
		}
	}

	/// ===地面の範囲を出たら=== ///
	// X方向の範囲チェック
	if (this->transform_.translate.x > (groundInfo_.currentGroundCenter.x + groundInfo_.currentGroundHalfSize.x + characterHalfHeight) ||
		this->transform_.translate.x < (groundInfo_.currentGroundCenter.x - groundInfo_.currentGroundHalfSize.x - characterHalfHeight)) {
		// 地面の範囲から出たらfalseにする
		groundInfo_.isGrounded = false;
		groundInfo_.hasGroundCollision = false;
	}

	// Z方向の範囲チェック
	if (this->transform_.translate.z > (groundInfo_.currentGroundCenter.z + groundInfo_.currentGroundHalfSize.z + characterHalfHeight) ||
		this->transform_.translate.z < (groundInfo_.currentGroundCenter.z - groundInfo_.currentGroundHalfSize.z - characterHalfHeight)) {
		// 地面の範囲から出たらfalseにする
		groundInfo_.isGrounded = false;
		groundInfo_.hasGroundCollision = false;
	}
}
