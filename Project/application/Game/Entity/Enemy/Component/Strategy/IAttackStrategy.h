#pragma once
/// ===Include=== ///
#include "Math/Vector3.h"

/// ===攻撃実行コンテキスト=== ///
struct AttackExecutionContext {
	Vector3 attackerPosition; // 攻撃者の位置
	Vector3 attackerDirection;   // 攻撃社の向き
	Vector3 targetPosition;  // 攻撃対象の位置
	float deltaTime;          // デルタタイム
};

/// ===攻撃実行結果=== ///
struct AttackExecutionResult {
	bool didAttack = false;        // 攻撃を実行したか
	bool isAttackComplete = false; // 攻撃が完了したか
	Vector3 velocity{};            // 攻撃中の移動速度（突進など）
};


///=====================================================/// 
/// IAttackStrategy(攻撃インターフェイス)
///=====================================================///
class IAttackStrategy {
public:

	virtual ~IAttackStrategy() = default;

    /// <summary>
    /// 攻撃開始時の処理
    /// </summary>
    virtual void OnAttackStart() = 0;

    /// <summary>
    /// 攻撃実行処理
    /// </summary>
    /// <param name="context">実行コンテキスト</param>
    /// <returns>実行結果</returns>
    virtual AttackExecutionResult Execute(const AttackExecutionContext& context) = 0;

    /// <summary>
    /// 攻撃終了時の処理
    /// </summary>
    virtual void OnAttackEnd() = 0;

    /// <summary>
    /// ImGui情報の表示
    /// </summary>
    virtual void Information() = 0;
};

