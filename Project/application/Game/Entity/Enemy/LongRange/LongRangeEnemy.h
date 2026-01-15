#pragma once
/// ===Include=== ///
#include "application/Game/Entity/Enemy/Base/BaseEnemy.h"
#include "application/Game/Entity/Enemy/LongRange/LongRangeEnemyBullet.h"
#include <list>

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

	/// <summary>
	/// 攻撃処理の初期化
	/// </summary>
	void StartAttack()override;

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack()override;

public: /// ===衝突判定=== ///
	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="collider">衝突した相手の Collider へのポインター。衝突相手の情報を取得するために使用します。</param>
	void OnCollision(Collider* collider) override;

private: /// ===変数=== ///

	/// ===Bullet情報=== ///
	struct BulletInfo {
		std::list<std::unique_ptr<LongRangeEnemyBullet>> bullets_;
		float interval = 0.0f;   // インターバル
		float reloadTime = 3.0f; // リロード時間
		bool isShot = false;	 // 発射フラグ
		bool isHit = false;		 // ヒット時のフラグ
	};
	BulletInfo bulletInfo_;

#ifdef USE_IMGUI
protected:
	/// <summary>
	/// 派生側で型固有のチューニング値をコピーするための関数
	/// </summary>
	/// <param name="dst">コピー先の BaseEnemy オブジェクトへのポインタ。nullptr でないことが期待されます。</param>
	void CopyTypeTuningFromThisTo(BaseEnemy* dst) const override;
#endif // USE_IMGUI
};
