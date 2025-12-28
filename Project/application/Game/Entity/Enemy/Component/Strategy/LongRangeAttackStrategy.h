#pragma once
/// ===Include=== ///
#include "IAttackStrategy.h"
// Bullet
#include "application/Game/Entity/Enemy/LongRange/LongRangeEnemeyBullet.h"
// C++
#include <list>
#include <memory>

///=====================================================/// 
/// LongRangeAttackStrategy(遠距離攻撃インターフェイス)
///=====================================================///
class LongRangeAttackStrategy : public IAttackStrategy {
public:
    /// ===設定パラメータの構造体=== ///
    struct Config {
        float shootInterval = 0.5f;   // 発射間隔
        float reloadTime = 3.0f;      // リロード時間
        int maxBullets = 3;           // 最大弾数
    };

    LongRangeAttackStrategy() = default;
    ~LongRangeAttackStrategy() override = default;

    /// <summary>
    /// 指定された設定 (Config) を使用して初期化を行います。
    /// </summary>
    /// <param name="config">初期化に使用する設定を保持する Config 型の定数参照。</param>
    void Initialize(const Config& config);

    /// <summary>
    /// 攻撃開始時の処理を行うメソッド。派生クラスでオーバーライドして具体的な動作（初期化や通知など）を実装する。
    /// </summary>
    void OnAttackStart() override;

    /// <summary>
    /// 与えられた AttackExecutionContext に基づいて攻撃処理を実行する。基底クラスの仮想関数をオーバーライドする実装。
    /// </summary>
    /// <param name="context">攻撃の実行に必要な状態やパラメータを保持するコンテキスト。const参照で渡され、関数内で変更されない。</param>
    /// <returns>AttackExecutionResult 型の値。攻撃実行の結果（成功／失敗、ダメージ量、発生した効果など）を表す。</returns>
    AttackExecutionResult Execute(const AttackExecutionContext& context) override;

    /// <summary>
    /// 攻撃の終了時に呼び出される、基底クラスの仮想メソッドをオーバーライドするメンバー関数。
    /// </summary>
    void OnAttackEnd() override;

    /// <summary>
    /// 基底クラスの仮想関数をオーバーライドして情報を表示するメンバ関数。
    /// </summary>
    void Information() override;

    /// <summary>
    /// 経過時間に基づいて弾（弾丸）の状態を更新します。
    /// </summary>
    /// <param name="deltaTime">前フレームからの経過時間（秒）。弾の位置移動、アニメーション、寿命などの更新に使用されます。</param>
    void UpdateBullets();
    void DrawBullets();

public: /// ===Getter=== ///
	// Config
    const Config& GetConfig() const { return config_; }

public: /// ===Setter=== ///
	// Config
    void SetConfig(const Config& config) { config_ = config; }

private:
    /// ===設定=== ///
    Config config_;

	/// ===攻撃=== ///
	float shootTimer_ = 0.0f;  // 発射タイマー
	int bulletsShot_ = 0;      // 発射した弾数
	bool isReloading_ = false; // リロード中フラグ
    
	/// ===弾管理=== ///
    std::list<std::unique_ptr<LongRangeEnemeyBullet>> bullets_;

private:
    /// <summary>
    /// 弾丸を発射する処理を実行する。
    /// </summary>
    /// <param name="context">攻撃の実行に必要な情報を含むコンテキスト。</param>
    void ShootBullet(const AttackExecutionContext& context);
};

