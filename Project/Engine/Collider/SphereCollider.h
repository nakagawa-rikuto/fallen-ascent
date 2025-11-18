#pragma once
/// ===Include=== ///
#include "Engine/Collider/Base/Collider.h"

///=====================================================/// 
/// SphereCollider
///=====================================================///
class SphereCollider : public Collider {
public:

	SphereCollider() = default;
	~SphereCollider() = default;

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
	/// <param name="mode">描画に使用するブレンドモード。BlendMode 型の値で合成方法を指定します。</param>
	virtual void Draw(BlendMode mode) override;

	/// <summary>
	/// ImGui情報の更新
	/// </summary>
	virtual void Information()override;

public: /// ===衝突=== ///
	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="collider">衝突対象のコライダへのポインタ。衝突したオブジェクトの情報にアクセスするために使用される。</param>
	virtual void OnCollision(Collider* collider) override = 0;

public: /// ===Setter=== ///
	// Sphereの設定
	void SetRadius(const float& radius);

public: /// ===Getter=== ///
	// Sphereの取得
	Sphere GetSphere()const;

protected:
	Sphere sphere_;
};

