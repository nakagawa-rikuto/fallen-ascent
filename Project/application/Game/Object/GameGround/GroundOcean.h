#pragma once
/// ===Include=== ///
// Collider
#include "Engine/Collider/AABBCollider.h"
// Ocean
#include "Engine/Graphics/3d/Ocean/Ocean.h"

///=====================================================/// 
/// GroundOcean
///=====================================================///
class GroundOcean : public AABBCollider {
public: 

	GroundOcean() = default;
	~GroundOcean();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="mode">BlendModeの適用</param>
	void Draw(BlendMode mode = BlendMode::KBlendModeNormal)override;

	/// <summary>
	/// ImGui情報の表示
	/// </summary>
	void ShowImGui();

public: /// ===衝突処理=== ///

	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="collider">衝突先のCollider</param>
	void OnCollision(Collider* collider)override;

private:

	/// ===オーシャン=== ///
	std::unique_ptr<Ocean> ocean_;
};

