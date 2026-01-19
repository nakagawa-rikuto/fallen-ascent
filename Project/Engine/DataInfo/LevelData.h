#pragma once
#include <string>
#include <vector>

#include "Math/Vector3.h"

/// ===LevelData=== ///
struct LevelData {

	/// ===ClassType=== ///
	enum class ClassTypeLevel {
		None,
		Player1,
		Player2,
		Enemy1,
		Enemy2,
		Enemy3,
		Enemy4,
		Enemy5,
		Enemy6,
		Enemy7,
		Enemy8,
		Enemy9,
		Enemy10,
		Object1,
		Object2,
		Object3,
		Object4,
		Object5,
		Object6,
		Object7,
		Object8,
		Object9,
		Object10,
		Ground1,
		Ground2,
		Ground3,
		Ground4,
		Ground5,
		Ground6,
		Ground7,
		Ground8,
		Ground9,
		Ground10,
		SkyBox1,
		SkyBox2,
		SkyBox3,
		SkyBox4,
		SkyBox5,
		SkyBox6,
		SkyBox7,
		SkyBox8,
		SkyBox9,
		SkyBox10,
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