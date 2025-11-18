#pragma once
/// ===Include=== ///
#include "Engine/Collider/Base/Collider.h"

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
	virtual void OnCollision(Collider* collider) override = 0;

public: /// ===Setter=== ///
	// AABBの設定
	void SetAABB(const AABB& aabb);

public: /// ===Getter=== ///
	// AABBの取得
	AABB GetAABB() const;

protected:
	AABB aabb_;
};

