#include "Collision.h"
// Manager
#include "Engine/System/Managers/ColliderManager.h"
// Locator
#include "Locator.h"

using namespace MiiEngine;
namespace Service {
	///-------------------------------------------/// 
	/// 追加
	///-------------------------------------------///
	void Collision::AddCollider(Collider* collider) { Locator::GetColliderManager()->AddCollider(collider); }

	///-------------------------------------------/// 
	/// 削除
	///-------------------------------------------///
	void Collision::RemoveCollider(Collider* collider) { Locator::GetColliderManager()->RemoveCollider(collider); }

	///-------------------------------------------/// 
	/// リセット
	///-------------------------------------------///
	void Collision::Reset() { Locator::GetColliderManager()->Reset(); }

	///-------------------------------------------/// 
	/// Setter
	///-------------------------------------------///
	// LightTYpe
	void Collision::SetLight(LightType lightType) { Locator::GetColliderManager()->SetLight(lightType); }

	// LightData
	void Collision::SetLightData(LightInfo light) { Locator::GetColliderManager()->SetLightData(light); }
}