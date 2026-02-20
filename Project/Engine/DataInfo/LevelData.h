#pragma once
#include <string>
#include <vector>

#include "Math/Vector3.h"

/// ===LevelData=== ///
struct LevelData {

	/// ===ClassType=== ///
	enum class ClassTypeLevel {
		None,
		Player,
		Enemy,
		Object,
		Ground
	};

	/// ===ColliderType=== ///
	enum class ColliderTypeLevel {
		None,
		Sphere,
		AABB,
		OBB,
	};

	/// ===JSONObjectData=== ///
	struct JsonObjectData {
		std::string fileName;			// モデルファイル名
		ClassTypeLevel classType;		// クラス種類
		ColliderTypeLevel colliderType; // コライダー種類
		Vector3 translation;			// 座標
		Vector3 rotation;				// 回転
		Vector3 scaling;				// スケーリング
		Vector3 colliderInfo1;			// コライダー情報A
		Vector3 colliderInfo2;			// コライダー情報B
	};

	// vector配列
	std::vector<JsonObjectData> objects;
};