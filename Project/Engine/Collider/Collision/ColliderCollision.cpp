#include "ColliderCollision.h"
#include "Engine/Collider/Base/Collider.h"
#include "Engine/Collider/AABBCollider.h"
#include "Engine/Collider/SphereCollider.h"
#include "Engine/Collider/OBBCollider.h"
#include <algorithm>
#include <cmath>
#include "Math/sMath.h"

///-------------------------------------------/// 
/// 汎用衝突処理
///-------------------------------------------///
void ColliderCollision::ProcessCollision(Collider* colliderA, Collider* colliderB, float pushBackRatio) {
	// nullチェック
	if (!colliderA || !colliderB) {
		return;
	}

	// pushBackRatioを0.0~1.0にクランプ
	pushBackRatio = std::clamp(pushBackRatio, 0.0f, 1.0f);

	ColliderType typeA = colliderA->GetColliderType();
	ColliderType typeB = colliderB->GetColliderType();

	if (typeA == typeB) {
		// 同じタイプ同士の衝突処理
		switch (typeA) {
		case ColliderType::Sphere:
			SphereToSphereCollision(dynamic_cast<SphereCollider*>(colliderA), dynamic_cast<SphereCollider*>(colliderB), pushBackRatio);
			break;
		case ColliderType::OBB:
			OBBToOBBCollision(dynamic_cast<OBBCollider*>(colliderA), dynamic_cast<OBBCollider*>(colliderB), pushBackRatio);
			break;
		}
	} else {
		// 異なるタイプ同士の衝突処理
		ProcessMixdeTypeCollision(colliderA, colliderB, typeA, typeB, pushBackRatio);
	}
}

///-------------------------------------------/// 
/// 異なるタイプ同士の衝突処理
///-------------------------------------------///
void ColliderCollision::ProcessMixdeTypeCollision(Collider* colliderA, Collider* colliderB, ColliderType typeA, ColliderType typeB, float pushBackRationA) {
	/// ===異なるCollider毎の処理=== ///
	if ((typeA == ColliderType::Sphere && typeB == ColliderType::AABB) || (typeA == ColliderType::AABB && typeB == ColliderType::Sphere)) {

		// タイプによる判別
		SphereCollider* sphere = (typeA == ColliderType::Sphere) 
			? dynamic_cast<SphereCollider*>(colliderA) 
			: dynamic_cast<SphereCollider*>(colliderB);
		AABBCollider* aabb = (typeA == ColliderType::AABB) 
			? dynamic_cast<AABBCollider*>(colliderA) 
			: dynamic_cast<AABBCollider*>(colliderB);
		float ration = (typeA == ColliderType::Sphere) ? pushBackRationA : (1.0f - pushBackRationA);
		// 衝突判定による押し戻し処理
		SphereToAABBCollision(sphere, aabb, ration);

	} else if ((typeA == ColliderType::Sphere && typeB == ColliderType::OBB) || (typeA == ColliderType::OBB && typeB == ColliderType::Sphere)) {

		// タイプによる判別
		SphereCollider* sphere = (typeA == ColliderType::Sphere) 
			? dynamic_cast<SphereCollider*>(colliderA) 
			: dynamic_cast<SphereCollider*>(colliderB);
		OBBCollider* obb = (typeA == ColliderType::OBB) 
			? dynamic_cast<OBBCollider*>(colliderA) 
			: dynamic_cast<OBBCollider*>(colliderB);
		float ration = (typeA == ColliderType::Sphere) ? pushBackRationA : (1.0f - pushBackRationA);
		// 衝突判定による押し戻し処理
		SphereToOBBCollision(sphere, obb, ration);

	} else if ((typeA == ColliderType::AABB && typeB == ColliderType::OBB) || (typeA == ColliderType::OBB && typeB == ColliderType::AABB)) {

		// タイプによる判別
		AABBCollider* aabb = (typeA == ColliderType::AABB) 
			? dynamic_cast<AABBCollider*>(colliderA) 
			: dynamic_cast<AABBCollider*>(colliderB);
		OBBCollider* obb = (typeA == ColliderType::OBB) 
			? dynamic_cast<OBBCollider*>(colliderA) 
			: dynamic_cast<OBBCollider*>(colliderB);
		float ration = (typeA == ColliderType::AABB) ? pushBackRationA : (1.0f - pushBackRationA);
		// 衝突判定による押し戻し処理
		AABBToOBBCollision(aabb, obb, ration);
	 }
}

///-------------------------------------------/// 
/// 球と球との衝突処理
///-------------------------------------------///
void ColliderCollision::SphereToSphereCollision(SphereCollider* sphereA, SphereCollider* sphereB, float pushBackRatio) {
	Vector3 posA = sphereA->GetTransform().translate;
	Vector3 posB = sphereB->GetTransform().translate;

	Vector3 direction = posB - posA;
	float distance = Length(direction);

	float radiusA = sphereA->GetSphere().radius;
	float radiusB = sphereB->GetSphere().radius;
	float totalRadius = radiusA + radiusB;

	// 正規化
	if (distance < EPSILON) {
		direction = Vector3(0.0f, 1.0f, 0.0f);
		distance = EPSILON;
	} else {
		direction = Normalize(direction);
	}

	// めり込み深度を計算
	float penetrationDepth = totalRadius - distance;

	// めり込んでいる場合のみ押し戻し (マージンは含めない)
	if (penetrationDepth > 0.0f) {
		float moveA = penetrationDepth * (1.0f - pushBackRatio);
		float moveB = penetrationDepth * pushBackRatio;

		posA -= direction * moveA;
		posB += direction * moveB;

		sphereA->SetTranslate(posA);
		sphereB->SetTranslate(posB);
	}
}

///-------------------------------------------/// 
/// OBBとOBBとの衝突処理
///-------------------------------------------///
void ColliderCollision::OBBToOBBCollision(OBBCollider* obbA, OBBCollider* obbB, float pushBackRatio) {
	// 位置の取得
	Vector3 posA = obbA->GetTransform().translate;
	Vector3 posB = obbB->GetTransform().translate;

	// 分離軸判定を実行
	SATResult result = PerformSAT(obbA->GetOBB(), obbB->GetOBB());

	// 軸の方向を確認(AからBへのベクトルと同じ向きにする)
	Vector3 centerDiff = posB - posA;
	float centerDiffLength = Length(centerDiff);

	// 中心が重なっている場合の処理
	if (centerDiffLength < EPSILON) {
		// 最も信頼できる軸を使用(通常はY軸)
		result.axis = Vector3(0.0f, 1.0f, 0.0f);
	} else {
		// 軸の向きを中心間ベクトルに合わせる
		if (Dot(result.axis, centerDiff) < 0.0f) {
			result.axis = -result.axis;
		}
	}

	// マージン
	const float PushMargin = 0.5f;

	// めり込んでいる場合のみ押し戻し 
	if (result.depth > 0.0f) {
		// マージンを考慮した調整深度
		float adjustDepth = result.depth * PushMargin;

		float moveAABB = adjustDepth * (1.0f - pushBackRatio);
		float moveOBB = adjustDepth * pushBackRatio;

		posA -= result.axis * moveAABB;
		posB += result.axis * moveOBB;

		obbA->SetTranslate(posA);
		obbB->SetTranslate(posB);
	}
}

///-------------------------------------------/// 
/// 球とOBBとの衝突処理
///-------------------------------------------///
void ColliderCollision::SphereToOBBCollision(SphereCollider* sphere, OBBCollider* obb, float pushBackRatio) {
	Vector3 obbPos = obb->GetTransform().translate;
	Vector3 spherePos = sphere->GetTransform().translate;
	float sphereRadius = sphere->GetSphere().radius;

	// 球体の中心からOBB上の最も近い点を計算
	Vector3 closestPoint = CalculateClosestPointOnOBB(spherePos, obb->GetOBB());
	Vector3 direction = spherePos - closestPoint;
	float distance = Length(direction);

	// 方向の正規化
	if (distance < EPSILON) {
		// OBBの中心から球の中心への方向を使用
		direction = spherePos - obb->GetOBB().center;
		float centerDist = Length(direction);

		if (centerDist < EPSILON) {
			// 完全に重なっている場合、上方向に押し出す
			direction = Vector3(0.0f, 1.0f, 0.0f);
		} else {
			direction = Normalize(direction);
		}

		// 内部にいる場合、距離は0として扱う
		distance = 0.0f;
	} else {
		direction = Normalize(direction);
	}

	// めり込み深度を計算
	float penetrationDepth = sphereRadius - distance;

	// めり込んでいる場合のみ押し戻し
	if (penetrationDepth > 0.0f) {
		float moveSphere = penetrationDepth * pushBackRatio;
		float moveOBB = penetrationDepth * (1.0f - pushBackRatio);

		spherePos += direction * moveSphere;
		obbPos -= direction * moveOBB;

		sphere->SetTranslate(spherePos);
		obb->SetTranslate(obbPos);
	}
}

///-------------------------------------------/// 
/// 球とAABBとの衝突処理
///-------------------------------------------///
void ColliderCollision::SphereToAABBCollision(SphereCollider* sphere, AABBCollider* aabb, float pushBackRatio) {
	Vector3 aabbPos = aabb->GetTransform().translate;
	Vector3 spherePos = sphere->GetTransform().translate;
	float sphereRadius = sphere->GetSphere().radius;

	// AABB上の最近点計算
	AABB aabbData = aabb->GetAABB();
	Vector3 closestPoint;
	closestPoint.x = std::clamp(spherePos.x, aabbData.min.x, aabbData.max.x);
	closestPoint.y = std::clamp(spherePos.y, aabbData.min.y, aabbData.max.y);
	closestPoint.z = std::clamp(spherePos.z, aabbData.min.z, aabbData.max.z);

	// 球体の中心から最も近い点への方向
	Vector3 direction = spherePos - closestPoint;
	float distance = Length(direction);

	// 方向の正規化
	if (distance < EPSILON) {
		// 球の中心がAABB内部にある場合
		Vector3 aabbCenter = (aabbData.min + aabbData.max) * 0.5f;
		direction = spherePos - aabbCenter;
		float centerDist = Length(direction);

		if (centerDist < EPSILON) {
			direction = Vector3(0.0f, 1.0f, 0.0f);
		} else {
			direction = Normalize(direction);
		}

		// 内部にいる場合、距離は0として扱う
		distance = 0.0f;
	} else {
		direction = Normalize(direction);
	}

	// めり込み深度を計算
	float penetrationDepth = sphereRadius - distance;

	// めり込んでいる場合のみ押し戻し
	if (penetrationDepth > 0.0f) {
		float moveSphere = penetrationDepth * pushBackRatio;
		float moveAABB = penetrationDepth * (1.0f - pushBackRatio);

		spherePos += direction * moveSphere;
		aabbPos -= direction * moveAABB;

		sphere->SetTranslate(spherePos);
		aabb->SetTranslate(aabbPos);
	}
}

///-------------------------------------------/// 
/// AABBとOBBとの衝突処理
///-------------------------------------------///
void ColliderCollision::AABBToOBBCollision(AABBCollider* aabb, OBBCollider* obb, float pushBackRatio) {
	// AABBをOBBとして扱う
	OBB aabbAsOBB;
	AABB aabbData = aabb->GetAABB();
	aabbAsOBB.center = (aabbData.min + aabbData.max) * 0.5f;
	aabbAsOBB.halfSize = (aabbData.max - aabbData.min) * 0.5f;

	// 軸は単位軸
	aabbAsOBB.axis[0] = Vector3(1.0f, 0.0f, 0.0f);
	aabbAsOBB.axis[1] = Vector3(0.0f, 1.0f, 0.0f);
	aabbAsOBB.axis[2] = Vector3(0.0f, 0.0f, 1.0f);

	// SATで衝突判定
	SATResult result = PerformSAT(aabbAsOBB, obb->GetOBB());

	// 位置の取得
	Vector3 aabbPos = aabb->GetTransform().translate;
	Vector3 obbPos = obb->GetTransform().translate;

	// 軸の方向を確認
	Vector3 centerDiff = obbPos - aabbPos;
	if (Dot(result.axis, centerDiff) < 0.0f) {
		result.axis = -result.axis;
	}

	// マージン
	const float PushMargin = 0.5f;

	// めり込んでいる場合のみ押し戻し 
	if (result.depth > 0.0f) {
		// マージンを考慮した調整深度
		float adjustDepth = result.depth * PushMargin;

		float moveAABB = adjustDepth * (1.0f - pushBackRatio);
		float moveOBB = adjustDepth * pushBackRatio;

		aabbPos -= result.axis * moveAABB;
		obbPos += result.axis * moveOBB;

		aabb->SetTranslate(aabbPos);
		obb->SetTranslate(obbPos);
	}
}

///-------------------------------------------/// 
/// 分離軸判定(SAT)の実装
///-------------------------------------------///
ColliderCollision::SATResult ColliderCollision::PerformSAT(const OBB& obbA, const OBB& obbB) {
	SATResult result;
	result.depth = FLT_MAX;
	result.axis = Vector3(0.0f, 1.0f, 0.0f); // デフォルト軸

	// テストする軸のリスト
	Vector3 testAxes[15];
	int axisCount = 0;

	// OBB Aの3軸
	for (int i = 0; i < 3; ++i) {
		testAxes[axisCount++] = obbA.axis[i];
	}

	// OBB Bの3軸
	for (int i = 0; i < 3; ++i) {
		testAxes[axisCount++] = obbB.axis[i];
	}

	// 外積による9軸
	for (int i = 0; i < 3; ++i) {
		for (int j = 0; j < 3; ++j) {
			Vector3 cross = Math::Cross(obbA.axis[i], obbB.axis[j]);
			float crossLength = Length(cross);

			// ほぼ平行な軸はスキップ(安定性向上)
			const float PARALLEL_THRESHOLD = 0.01f; // EPSILONより大きい値
			if (crossLength > PARALLEL_THRESHOLD) {
				testAxes[axisCount++] = cross / crossLength;
			}
		}
	}

	// 最小の重なりを持つ軸を見つける
	//bool hasCollision = true;

	for (int i = 0; i < axisCount; ++i) {
		float overlap;
		if (!TestAxisOverlap(obbA, obbB, testAxes[i], overlap)) {
			// 分離軸が見つかった = 衝突していない
			result.depth = 0.0f;
			return result;
		}

		// 最小の重なりを記録(より安定した軸を選択)
		if (overlap < result.depth) {
			result.depth = overlap;
			result.axis = testAxes[i];
		}
	}

	return result;
}

///-------------------------------------------/// 
/// 軸への投影
///-------------------------------------------///
float ColliderCollision::ProjectOBBOntoAxis(const OBB& obb, const Vector3& axis) {
	// OBBの各軸の寄与を計算
	float projection = 0.0f;
	for (int i = 0; i < 3; ++i) {
		float extent = (i == 0) ? obb.halfSize.x : (i == 1) ? obb.halfSize.y : obb.halfSize.z;
		projection += extent * std::abs(Dot(obb.axis[i], axis));
	}
	return projection;
}

///-------------------------------------------/// 
/// 軸上での重なりテスト
///-------------------------------------------///
bool ColliderCollision::TestAxisOverlap(const OBB& obbA, const OBB& obbB, const Vector3& axis, float& outOverlap) {
	// 軸が正規化されていることを確認
	float axisLength = Length(axis);
	if (axisLength < EPSILON) {
		outOverlap = 0.0f;
		return true;  // 無効な軸は衝突とみなす
	}
	Vector3 normalizedAxis = axis / axisLength;

	// 各OBBの中心を軸に投影
	float centerA = Dot(obbA.center, normalizedAxis);
	float centerB = Dot(obbB.center, normalizedAxis);

	// 各OBBの範囲を計算
	float radiusA = ProjectOBBOntoAxis(obbA, normalizedAxis);
	float radiusB = ProjectOBBOntoAxis(obbB, normalizedAxis);

	// 中心間の距離
	float distance = std::abs(centerB - centerA);

	// 重なり判定
	float totalRadius = radiusA + radiusB;

	// マージン付き判定(安定性向上)
	const float COLLISION_MARGIN = 0.0001f;
	if (distance > totalRadius + COLLISION_MARGIN) {
		return false;  // 分離している
	}

	outOverlap = totalRadius - distance;
	return true;
}

///-------------------------------------------/// 
/// OBB上の最近点計算
///-------------------------------------------///
Vector3 ColliderCollision::CalculateClosestPointOnOBB(const Vector3& point, const OBB& obb) {
	Vector3 dir = point - obb.center;
	Vector3 closest = obb.center;

	// OBB空間における最近点を計算
	for (int i = 0; i < 3; ++i) {
		float axisExtent = (i == 0) ? obb.halfSize.x : (i == 1) ? obb.halfSize.y : obb.halfSize.z;
		float dist = Dot(dir, obb.axis[i]);
		dist = std::clamp(dist, -axisExtent, axisExtent);
		closest += obb.axis[i] * dist;
	}

	return closest;
}