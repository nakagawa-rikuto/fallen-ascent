#pragma once
/// ===Include=== ///
// Collider
#include "Engine/Collider/OBBCollider.h"
#include "Engine/Collider/AABBCollider.h"
#include "Engine/Collider/SphereCollider.h"
#include "Engine/Collider/ColliderConcept.h"

///=====================================================/// 
/// GameObject
///=====================================================///
template<typename TCollider> requires MiiEngine::IsCollider<TCollider>
class GameObject : public TCollider {
public:

	GameObject() = default;
	virtual ~GameObject();

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
	/// <param name="mode">描画に使用するブレンドモード。</param>
	virtual void Draw(MiiEngine::BlendMode mode) override;

	/// <summary>
	/// ImGui情報の表示
	/// </summary>
	virtual void Information() override;

public: /// ===衝突判定=== ///
	/// <summary>
	/// 衝突時の処理
	/// </summary>
	/// <param name="collider">衝突した相手を表す Collider へのポインター。</param>
	virtual void OnCollision(MiiEngine::Collider* collider) override;
};

