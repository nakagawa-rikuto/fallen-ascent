#pragma once
/// ===Include=== ///
#include "application/Game/Entity/Enemy/Base/BaseEnemy.h"

/// ===前方宣言=== ///
class LongRangeAttackStrategy;

///=====================================================/// 
/// 遠距離攻撃Enemy
///=====================================================///
class LongRangeEnemy : public BaseEnemy {
public:
	LongRangeEnemy() = default;
	~LongRangeEnemy();

	/// <summary>
	/// 初期化処理（GameScene用）
	/// </summary>
	/// <param name="translate">シーンの位置を設定する3次元平行移動ベクトルへのconst参照。シーンの初期配置やオフセットに使用されます。</param>
	void InitGameScene(const Vector3& translate)override;

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
	/// <param name="mode">描画に使用するブレンドモード。省略した場合は BlendMode::KBlendModeNormal が使用されます。</param>
	void Draw(BlendMode mode = BlendMode::KBlendModeNormal)override;

	/// <summary>
	/// ImGui情報の表示
	/// </summary>
	void Information()override;

public: /// ===衝突判定=== ///
	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="collider">衝突した相手の Collider へのポインター。衝突相手の情報を取得するために使用します。</param>
	void OnCollision(Collider* collider) override;

private: /// ===変数=== ///

	LongRangeAttackStrategy* longRangeStrategy_ = nullptr;

protected:
	/// <summary>
	/// 派生側で型固有のチューニング値をコピーするための関数
	/// </summary>
	/// <param name="dst">コピー先の BaseEnemy オブジェクトへのポインタ。nullptr でないことが期待されます。</param>
	void CopyTypeTuningFromThisTo(BaseEnemy* dst) const override;
};
