#include "AABBCollider.h"
#ifdef USE_IMGUI
#include "imgui.h"
#endif // USE_IMGUI

namespace MiiEngine {
	///-------------------------------------------/// 
	/// 初期化
	///-------------------------------------------///
	void AABBCollider::Initialize() {
		type_ = ColliderType::AABB;

		// Colliderの初期化
		Collider::Initialize();
	}

	///-------------------------------------------/// 
	/// 更新
	///-------------------------------------------///
	void AABBCollider::Update() {

		/// ===Object3Dの更新=== ///
		object3d_->SetTranslate(transform_.translate);
		object3d_->SetRotate(transform_.rotate);
		object3d_->SetScale(transform_.scale);
		object3d_->SetColor(color_);

		// Colliderの更新処理
		Collider::Update();
	}

	///-------------------------------------------/// 
	/// 描画
	///-------------------------------------------///
	void AABBCollider::Draw(BlendMode mode) {

		// Line
	#ifdef _DEBUG
		// デバッグ時のみ描画
		line_->DrawAABB(aabb_, lineColor_);

	#endif // DEBUG

		// Colliderの描画処理
		Collider::Draw(mode);
	}

	///-------------------------------------------/// 
	/// 情報
	///-------------------------------------------///
	void AABBCollider::Information() {
	#ifdef USE_IMGUI
		Collider::Information();
		ImGui::Text("OBBInfo");
		ImGui::DragFloat3("Min", &aabb_.min.x, 0.1f);
		ImGui::DragFloat3("Max", &aabb_.max.x, 0.1f);
	#endif // USE_IMGUI
	}

	///-------------------------------------------/// 
	/// 衝突処理
	///-------------------------------------------///
	void AABBCollider::OnCollision(Collider* collider) { collider; }

	///-------------------------------------------/// 
	/// Setter
	///-------------------------------------------///
	void AABBCollider::SetAABB(const AABB& aabb) {
		// translateとrotateの情報を元にAABBを設定
		AdjustmentAABB(aabb, transform_.translate, transform_.rotate);
	}

	///-------------------------------------------/// 
	/// Getter
	///-------------------------------------------///
	AABB AABBCollider::GetAABB() const {
		return aabb_;
	}

	///-------------------------------------------/// 
	/// 現在の位置と回転に合わせてAABBを調整調整する 
	///-------------------------------------------///
	void AABBCollider::AdjustmentAABB(const AABB& aabb, const Vector3& translate, const Quaternion& rotate) {
		// ローカルAABBの8頂点を列挙
		Vector3 corners[8] = {
			{ aabb.min.x, aabb.min.y, aabb.min.z },
			{ aabb.max.x, aabb.min.y, aabb.min.z },
			{ aabb.min.x, aabb.max.y, aabb.min.z },
			{ aabb.max.x, aabb.max.y, aabb.min.z },
			{ aabb.min.x, aabb.min.y, aabb.max.z },
			{ aabb.max.x, aabb.min.y, aabb.max.z },
			{ aabb.min.x, aabb.max.y, aabb.max.z },
			{ aabb.max.x, aabb.max.y, aabb.max.z },
		};

		// 回転・移動を適用してワールド空間のAABBを再計算
		Vector3 worldMin = { FLT_MAX,  FLT_MAX,  FLT_MAX };
		Vector3 worldMax = { -FLT_MAX, -FLT_MAX, -FLT_MAX };

		for (const auto& corner : corners) {
			// クォータニオンで頂点を回転してtranslateを加算
			Vector3 worldPos = translate + Math::RotateVector(corner, rotate);

			worldMin.x = (std::min)(worldMin.x, worldPos.x);
			worldMin.y = (std::min)(worldMin.y, worldPos.y);
			worldMin.z = (std::min)(worldMin.z, worldPos.z);

			worldMax.x = (std::max)(worldMax.x, worldPos.x);
			worldMax.y = (std::max)(worldMax.y, worldPos.y);
			worldMax.z = (std::max)(worldMax.z, worldPos.z);
		}

		// 調整済みAABBをセット
		aabb_ = { worldMin, worldMax };
	}
}