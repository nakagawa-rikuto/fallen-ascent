#pragma once
/// ===Include=== ///
#include "Math/Vector3.h"
#include "Strategy/IAttackStrategy.h"
#include <cstdint>
#include <memory>

///=====================================================/// 
/// EnemyAttackComponent
///=====================================================///
class EnemyAttackComponent {
public:
	/// ===設定パラメータの構造体=== ///
	struct AttackConfig {
		float range = 30.0f;	    // 攻撃範囲
		float distance = 5.0f;	    // 攻撃可能距離
		float interval = 2.0f;	    // 攻撃間隔
		int32_t power = 1;		    // 攻撃力
	};

	/// ===更新用コンテキスト=== ///
	struct UpdateContext {
		Vector3 currentPosition;  // 現在の位置
		Vector3 currentDirection; // 現在の向き
		Vector3 targetPosition;   // ターゲットの位置
		float deltaTime;          // デルタタイム
	};

	/// ===更新結果=== ///
	struct UpdateResult {
		Vector3 velocity{};				// 移動ベクトル
		bool isAttacking = false;		// 攻撃中フラグ
		bool isAttackComplete = false;  // 攻撃完了フラグ
	};

public:

	EnemyAttackComponent() = default;
	~EnemyAttackComponent() = default;

	// コピー・ムーブは明示的に制御
	EnemyAttackComponent(const EnemyAttackComponent&) = delete;
	EnemyAttackComponent& operator=(const EnemyAttackComponent&) = delete;

	/// <summary>
	/// AttackConfig を使用して初期化を行います。
	/// </summary>
	/// <param name="config">初期化に使用する設定オブジェクトへの参照。省略した場合はデフォルト構築された AttackConfig が使用されます。</param>
	void Initialize(const AttackConfig& config = AttackConfig{}, std::unique_ptr<IAttackStrategy> strategy = nullptr);

	/// <summary>
	/// 攻撃開始処理
	/// </summary>
	void StartAttack();

	/// <summary>
	/// 与えられたコンテキストに基づいて更新を実行します。
	/// </summary>
	/// <param name="context">更新操作に必要な情報を含む、読み取り専用のコンテキスト参照。</param>
	/// <returns>更新の結果および状態を表す UpdateResult を返します。</returns>
	UpdateResult Update(const UpdateContext& context);

	/// <summary>
	/// ImGui情報の表示
	/// </summary>
	void Information();

public: /// ===Getter=== ///
	// AttackConfig
	AttackConfig GetConfig() const { return config_; };
	// AttackStrategy
	IAttackStrategy* GetStrategy() const { return attackStrategy_.get(); };

	// 攻撃中フラグ
	bool IsAttacking() const { return isAttacking_; };
	// クールダウンタイマー
	float GetCooldownTimer() const { return cooldownTimer_; };

public: /// ===Setter=== ///
	
	// AttackConfig
	void SetRange(float range) { config_.range = range; };
	void SetDistance(float distance) { config_.distance = distance; };
	void SetInterval(float interval) { config_.interval = interval; };
	void SetPower(int32_t power) { config_.power = power; };

	// 攻撃戦略の設定
	void SetStrategy(std::unique_ptr<IAttackStrategy> attackStrategy);

private:

	/// ===情報=== ///
	AttackConfig config_;

	/// ===インターフェイス=== ///
	std::unique_ptr<IAttackStrategy> attackStrategy_;

	bool isAttacking_ = false;
	float cooldownTimer_ = 0.0f;
};