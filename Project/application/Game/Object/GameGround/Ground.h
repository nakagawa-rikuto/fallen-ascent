
#pragma once
/// ===Include=== ///
#include "application/Game/Object/GameObject/GameObject.h"

///=====================================================/// 
/// Ground
///=====================================================///
class Ground : public GameObject<MiiEngine::AABBCollider> {
public:
	Ground() = default;
	~Ground();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="modelName">初期化に使用するモデルの名前。</param>
	void GameInit(const std::string& modelName);

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
	void Draw(MiiEngine::BlendMode mode = MiiEngine::BlendMode::KBlendModeNormal)override;

public:
	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="collider">衝突先のCollider</param>
	void OnCollision(MiiEngine::Collider* collider)override;
};
