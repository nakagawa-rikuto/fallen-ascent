#pragma once
/// ===Include=== ///
#include "Engine/Collider/Base/Collider.h"

namespace MiiEngine {
	///=====================================================/// 
	/// AABB
	///=====================================================///
	class AABBCollider : public Collider {
	public:

		AABBCollider() = default;
		~AABBCollider() = default;

		/// <summary>
		/// 初期化処理
		/// </summary>
		virtual void Initialize() override;

		/// <summary>
		/// 更新処理
		/// </summary>
		virtual void Update() override;

		/// <summary>
		/// 描画処理
		/// </summary>
		/// <param name="mode">描画時に使用するブレンドモードを指定する値（BlendMode 型）。</param>
		virtual void Draw(BlendMode mode) override;

		/// <summary>
		/// ImGui情報の更新
		/// </summary>
		virtual void Information()override;

	public: /// ===衝突=== ///
		/// <summary>
		/// 衝突処理
		/// </summary>
		/// <param name="collider">衝突した相手の Collider オブジェクトへのポインター。衝突に関する情報を取得・処理するために使用します。</param>
		virtual void OnCollision(Collider* collider) override;

	public: /// ===Setter=== ///
		// AABBの設定
		void SetAABB(const AABB& aabb);

	public: /// ===Getter=== ///
		// AABBの取得
		AABB GetAABB() const;

	private:
		AABB aabb_;

	private:

		/// <summary>
		/// 現在の位置と回転に合わせてAABBを合わせる関数
		/// </summary>
		/// <param name="aabb">設定する軸並行境界ボックスへの参照。</param>
		/// <param name="translate">平行移動ベクトル。</param>
		/// <param name="rotate">回転を表すクォータニオン。</param>
		void AdjustmentAABB(const AABB& aabb, const Vector3& translate, const Quaternion& rotate);
	};
}

