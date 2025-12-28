#pragma once
/// ===Incude=== ///
#include "IAttackStrategy.h"

///=====================================================/// 
/// CloseRangeAttackStrategy(近接攻撃インターフェイス)
///=====================================================///
class CloseRangeAttackStrategy : public IAttackStrategy {
public:
    /// ===コンフィグ=== ///
    struct Config {
        float chargeSpeed = 0.9f;     // 突進速度
        float attackDuration = 1.0f;  // 攻撃時間
        float stopTime = 0.5f;        // 攻撃後の停止時間
    };

    CloseRangeAttackStrategy() = default;
    ~CloseRangeAttackStrategy() override = default;

    /// <summary>
    /// 指定された設定を使用して初期化を行います。
    /// </summary>
    /// <param name="config">初期化に使用する設定を保持する Config 型の定数参照。</param>
    void Initialize(const Config& config);

    /// <summary>
    /// 攻撃の開始時に呼び出されるハンドラ。基底クラスの仮想関数をオーバーライドしていることを示す。
    /// </summary>
    void OnAttackStart() override;

    /// <summary>
    /// 攻撃を実行し、その結果を返すオーバーライドメソッド。
    /// </summary>
    /// <param name="context">攻撃の実行に必要な情報を含むコンテキスト。const参照で渡される。</param>
    /// <returns>AttackExecutionResult。攻撃実行の成否や詳細な結果を表す値。</returns>
    AttackExecutionResult Execute(const AttackExecutionContext& context) override;

    /// <summary>
    /// 攻撃の終了時の処理を行う。基底クラスの仮想関数をオーバーライドしていることを示す。
    /// </summary>
    void OnAttackEnd() override;

    /// <summary>
    /// ImGui を使用してユーザーインターフェイスを描画する、基底クラスの仮想関数をオーバーライドする宣言。
    /// </summary>
    void Information() override;

public: /// ===Getter=== ///
	// Config
    const Config& GetConfig() const { return config_; }

public: /// ===Setter=== ///
	// Config
    void SetConfig(const Config& config) { config_ = config; }

private:
	// 設定
    Config config_;

    /// ===攻撃フェーズ=== ///
    enum class AttackPhase {
        Charging,   // 突進中
        Stopping    // 停止中
    };
    AttackPhase currentPhase_ = AttackPhase::Charging;

	// フェーズタイマー
    float phaseTimer_ = 0.0f;
};

