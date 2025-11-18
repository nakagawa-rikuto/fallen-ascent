#pragma once
/// ===Include=== ///
#include "Engine/DataInfo/ColliderData.h"
#include "Math/sMath.h"

/// ===前方宣言=== ///
class Collider;
class SphereCollider;
class AABBCollider;
class OBBCollider;

///=====================================================/// 
/// ColliderCollision
///=====================================================///
class ColliderCollision {
public:

	ColliderCollision() = default;
	~ColliderCollision() = default;

	/// <summary>
	/// 2つのCollider間の衝突を処理します。衝突による押し戻しや位置調整などの物理的応答を行います。
	/// めり込みを解消し、接触面に配置します。
	/// </summary>
	/// <param name="colliderA">衝突する最初のコライダー。</param>
	/// <param name="colliderB">衝突する2番目のコライダー。</param>
	/// <param name="pushBackRatio">AとBの質量比。0.0f~1.0fの範囲で、0.5fは等質量を意味します。</param>
	void ProcessCollision(Collider* colliderA, Collider* colliderB, float pushBackRatio = 0.5f);

private:

	// 定数定義
	const float EPSILON = 1e-6f;

	// OBBの衝突情報
	struct SATResult {
		Vector3 axis;      // 最小侵入深度の軸
		float depth;       // 侵入深度
	};

private:

	/// <summary>
	/// 2つの球体コライダー間の衝突判定と衝突解決（位置の押し戻しなど）を行う関数。
	/// </summary>
	void SphereToSphereCollision(SphereCollider* sphereA, SphereCollider* sphereB, float pushBackRatio);

	/// <summary>
	/// 2つのOBB（向き付き境界ボックス）コライダー間の衝突判定と衝突応答（押し戻し）を行います。
	/// 分離軸判定(SAT)を使用して正確な衝突検出を実装。
	/// </summary>
	void OBBToOBBCollision(OBBCollider* obbA, OBBCollider* obbB, float pushBackRatio);

	/// <summary>
	/// 球 (SphereCollider) と OBB (OBBCollider) の衝突を判定し、衝突があれば押し戻しを適用する関数。
	/// </summary>
	void SphereToOBBCollision(SphereCollider* sphere, OBBCollider* obb, float pushBackRatio);

	/// <summary>
	/// 球と軸平行境界ボックス（AABB）間の衝突判定と解決を行う関数。
	/// </summary>
	void SphereToAABBCollision(SphereCollider* sphere, AABBCollider* aabb, float pushBackRatio);

	/// <summary>
	/// 軸整列境界ボックス (AABB) と向き付けられた境界ボックス (OBB) の衝突検出と解決を行います。
	/// </summary>
	void AABBToOBBCollision(AABBCollider* aabb, OBBCollider* obb, float pushBackRatio);

	/// <summary>
	/// 与えられた点から最も近い、指定された向き付けられた境界ボックス (OBB) 上の点を計算して返します。
	/// </summary>
	Vector3 CalculateClosestPointOnOBB(const Vector3& point, const OBB& obb);

	// ===== OBB衝突検出用のヘルパー関数 =====

	/// <summary>
	/// 分離軸判定(SAT)で使用する軸への投影を計算
	/// </summary>
	float ProjectOBBOntoAxis(const OBB& obb, const Vector3& axis);

	/// <summary>
	/// 2つのOBB間で指定された軸における重なり量を計算
	/// </summary>
	bool TestAxisOverlap(const OBB& obbA, const OBB& obbB, const Vector3& axis, float& outOverlap);

	/// <summary>
	/// 分離軸判定を使用してOBB同士の衝突情報を取得
	/// </summary>
	SATResult PerformSAT(const OBB& obbA, const OBB& obbB);
};