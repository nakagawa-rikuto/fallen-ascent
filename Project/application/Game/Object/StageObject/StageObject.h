#pragma once
/// ===Include=== ///
#include "application/Game/Object/GameObject/GameObject.h"
// C++
#include <string>
#include <memory>

///=====================================================/// 
/// StageObject
///=====================================================///
class StageObject : public GameObject<AABBCollider> {
public:
	StageObject() = default;
	~StageObject();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="modelName">初期化に使用するモデルの名前。</param>
	void GameInit(const std::string& modelName);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="mode">描画に使用するブレンドモードを指定します。</param>
	void Draw(BlendMode mode) override;

public: /// ===OnCollision=== ///
	/// <summary>
	/// 他のコライダーとの衝突が発生したときに呼び出されるオーバーライドされたハンドラ。
	/// </summary>
	/// <param name="collider">衝突した相手の Collider へのポインタ。衝突の詳細を問い合わせしたり、衝突応答を処理するために使用される。</param>
	void OnCollision(Collider* collider) override;
};

