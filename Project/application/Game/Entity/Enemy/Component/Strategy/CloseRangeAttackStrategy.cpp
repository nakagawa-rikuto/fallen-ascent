#include "CloseRangeAttackStrategy.h"
// ImGui
#ifdef USE_IMGUI
#include <imgui.h>
#endif

///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void CloseRangeAttackStrategy::Initialize(const Config& config) {
    config_ = config;
}

///-------------------------------------------/// 
/// 攻撃開始時の処理
///-------------------------------------------///
void CloseRangeAttackStrategy::OnAttackStart() {
    currentPhase_ = AttackPhase::Charging;
    phaseTimer_ = config_.attackDuration;
}

///-------------------------------------------/// 
/// 攻撃実行処理
///-------------------------------------------///
AttackExecutionResult CloseRangeAttackStrategy::Execute(const AttackExecutionContext& context) {
    AttackExecutionResult result{};
    result.didAttack = true;

	// フェーズタイマーの更新
    phaseTimer_ -= context.deltaTime;

    switch (currentPhase_) {
    case AttackPhase::Charging:
        // 突進中
        result.velocity = context.attackerDirection * config_.chargeSpeed;

        // ターゲットとアタッカーの水平距離を計算
		Vector3 toTarget = context.targetPosition - context.attackerPosition;

		// フェーズ遷移の判定
        if (phaseTimer_ <= 0.0f || Length(toTarget) < 0.5f) {
            // 停止フェーズへ
            currentPhase_ = AttackPhase::Stopping;
            phaseTimer_ = config_.stopTime;
        }
        break;

    case AttackPhase::Stopping:
        // 停止中
        result.velocity = { 0.0f, 0.0f, 0.0f };

        if (phaseTimer_ <= 0.0f) {
            // 攻撃完了
            result.isAttackComplete = true;
        }
        break;
    }

    return result;
}

///-------------------------------------------/// 
/// 攻撃終了時の処理
///-------------------------------------------///
void CloseRangeAttackStrategy::OnAttackEnd() {
    currentPhase_ = AttackPhase::Charging;
    phaseTimer_ = 0.0f;
}

///-------------------------------------------/// 
/// ImGui情報の表示
///-------------------------------------------///
void CloseRangeAttackStrategy::Information() {
#ifdef USE_IMGUI
    if (ImGui::TreeNode("近接攻撃設定")) {
        ImGui::DragFloat("突進速度", &config_.chargeSpeed, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("攻撃時間", &config_.attackDuration, 0.01f, 0.1f, 5.0f);
        ImGui::DragFloat("停止時間", &config_.stopTime, 0.01f, 0.0f, 2.0f);
        ImGui::TreePop();
    }
#endif
}