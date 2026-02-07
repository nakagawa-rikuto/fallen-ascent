#pragma once
/// ===Include=== ///
// c++
#include <list>
// Collider
#include "Engine/Collider/Base/Collider.h"

/// ===前方宣言=== ///
class SphereCollider;
class AABBCollider;
class OBBCollider;

namespace MiiEngine {
	///=====================================================/// 
	/// ColliderManager
	///=====================================================///
	class ColliderManager {
	public:

		ColliderManager() = default;
		~ColliderManager();

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize();

		/// <summary>
		/// リセット処理
		/// </summary>
		void Reset();

		/// <summary>
		/// コライダーの削除処理
		/// </summary>
		/// <param name="collider">削除対象の Collider へのポインタ。</param>
		void RemoveCollider(Collider* collider);

		/// <summary>
		/// コライダーの追加処理
		/// </summary>
		/// <param name="collider">追加する Collider オブジェクトへのポインタ。nullptr の扱いは実装依存です。</param>
		void AddCollider(Collider* collider);

		/// <summary>
		/// 二つのコライダー間の衝突判定処理
		/// </summary>
		/// <param name="colliderA">衝突判定対象の最初の Collider へのポインタ。</param>
		/// <param name="colliderB">衝突判定対象の2番目の Collider へのポインタ。</param>
		void CheckPairCollision(Collider* colliderA, Collider* colliderB);

		/// <summary>
		/// 全てのコライダー間の衝突判定処理
		/// </summary>
		void CheckAllCollisions();

	public: /// ===Setter=== ///
		// Lightの設定
		void SetLight(LightType type);
		void SetLightData(LightInfo light);

	private: /// ===変数=== ///

		// コライダーのリスト
		std::list<Collider*> colliders_;

		// 衝突判定関数の型定義
		using CollisionFunc = bool (ColliderManager::*)(Collider*, Collider*);
		// 衝突関数ディスパッチテーブル
		static constexpr int TypeCount = (int)ColliderType::Count;
		CollisionFunc collisionTable_[TypeCount][TypeCount];

	private: /// ===関数=== ///

		// 各当たり判定関数（Collider*を受け取るラッパー）
		bool Sphere_Sphere(Collider* a, Collider* b);
		bool AABB_AABB(Collider* a, Collider* b);
		bool OBB_OBB(Collider* a, Collider* b);
		bool Sphere_AABB(Collider* a, Collider* b);
		bool AABB_Sphere(Collider* a, Collider* b);
		bool AABB_OBB(Collider* a, Collider* b);
		bool OBB_AABB(Collider* a, Collider* b);
		bool Sphere_OBB(Collider* a, Collider* b);
		bool OBB_Sphere(Collider* a, Collider* b);

		/// <summary>
		/// 2つの球形コライダー (SphereCollider) 間で衝突が発生しているかを判定
		/// </summary>
		/// <param name="a">判定対象の最初の球形コライダーへのポインター。nullptr を渡すと未定義動作になる可能性があります。</param>
		/// <param name="b">判定対象の2番目の球形コライダーへのポインター。nullptr を渡すと未定義動作になる可能性があります。</param>
		/// <returns>衝突している場合は true、衝突していない場合は false を返します。</returns>
		bool SphereToSphereCollision(class SphereCollider* a, class SphereCollider* b);

		/// <summary>
		/// 2つの軸整列境界ボックス（AABB）同士が衝突しているかを判定
		/// </summary>
		/// <param name="a">判定対象の最初の AABBCollider へのポインタ。</param>
		/// <param name="b">判定対象の2番目の AABBCollider へのポインタ。</param>
		/// <returns>衝突している場合は true、そうでない場合は false を返します。</returns>
		bool AABBToAABBCollision(class AABBCollider* a, class AABBCollider* b);

		/// <summary>
		/// 2つのOBB（向き付き境界ボックス）コライダー間の衝突を判定
		/// </summary>
		/// <param name="a">衝突判定対象の最初のOBBColliderへのポインタ。</param>
		/// <param name="b">衝突判定対象の2番目のOBBColliderへのポインタ。</param>
		/// <returns>衝突が検出された場合は true、検出されなければ false を返します。</returns>
		bool OBBToOBBCollision(class OBBCollider* a, class OBBCollider* b);

		/// <summary>
		/// 球（Sphere）と軸平行境界ボックス（AABB）間の衝突判定を行う関数。
		/// </summary>
		/// <param name="sphere">判定対象の球コライダーへのポインタ。中心位置と半径を保持していることを想定する（nullptr を渡さないこと）。</param>
		/// <param name="aabb">判定対象の軸平行境界ボックス（AABB）コライダーへのポインタ。最小/最大座標などを保持していることを想定する（nullptr を渡さないこと）。</param>
		/// <returns>球とAABBが重なっている場合は true、重なっていない場合は false を返す。</returns>
		bool SphereToAABBCollision(class SphereCollider* sphere, class AABBCollider* aabb);

		/// <summary>
		/// AABB（軸に沿った境界ボックス）と OBB（任意方向の境界ボックス）間の衝突を判定
		/// </summary>
		/// <param name="aabb">判定対象の軸方向に整列した境界ボックスを表す AABBCollider へのポインタ。</param>
		/// <param name="obb">判定対象の任意方向の境界ボックスを表す OBBCollider へのポインタ。</param>
		/// <returns>衝突している場合は true、衝突していない場合は false を返します。</returns>
		bool AABBToOBBCollision(class AABBCollider* aabb, class OBBCollider* obb);

		/// <summary>
		/// 球コライダーと OBB（向き付き境界ボックス）コライダー間の衝突を判定
		/// </summary>
		/// <param name="sphere">判定対象の球を表す SphereCollider へのポインタ。有効なポインタを渡すことを前提とします。</param>
		/// <param name="obb">判定対象の OBB（向き付き境界ボックス）を表す OBBCollider へのポインタ。有効なポインタを渡すことを前提とします。</param>
		/// <returns>球と OBB が交差している場合は true、交差していない場合は false を返します。</returns>
		bool SphereToOBBCollision(class SphereCollider* sphere, class OBBCollider* obb);

		/// <summary>
		/// 分離軸定理（SAT）に基づいて、2つのOBB（向き付き境界ボックス）間の衝突（交差）を判定します。
		/// </summary>
		/// <param name="a">判定対象の最初のOBB。const参照で渡され、関数内で変更されません。</param>
		/// <param name="b">判定対象の2番目のOBB。const参照で渡され、関数内で変更されません。</param>
		/// <returns>2つのOBBが交差している場合はtrue、交差していない場合はfalseを返します。</returns>
		bool OBBSATCollision(const OBB& a, const OBB& b);

		/// <summary>
		/// OBB（向き付き境界ボックス）を指定した軸に射影し、軸上の最小・最大スカラー値を計算します。
		/// </summary>
		/// <param name="obb">射影される向き付き境界ボックス（const参照）。</param>
		/// <param name="axis">射影に用いる軸ベクトル（const参照）。通常は単位ベクトルを渡しますが、非正規化ベクトルも扱えます。</param>
		/// <param name="outMin">出力パラメーター。軸上の最小射影値を格納する参照。</param>
		/// <param name="outMax">出力パラメーター。軸上の最大射影値を格納する参照。</param>
		void ProjectOBBOntoAxis(const OBB& obb, const Vector3& axis, float& outMin, float& outMax);
	};
}

