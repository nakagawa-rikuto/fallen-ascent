#include "EnemyAttackComponent.h"
#include "application/Game/Entity/Enemy/Base/BaseEnemy.h"
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI

///-------------------------------------------/// 
/// Setter: 攻撃戦略の設定
///-------------------------------------------///
void EnemyAttackComponent::SetStrategy(std::unique_ptr<IAttackStrategy> attackStrategy) { attackStrategy_ = std::move(attackStrategy); }


///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void EnemyAttackComponent::Initialize(const AttackConfig& config, std::unique_ptr<IAttackStrategy> strategy) {
	config_ = config;
	attackStrategy_ = std::move(strategy);
	isAttacking_ = false;
	cooldownTimer_ = 0.0f;
}

///-------------------------------------------/// 
/// 攻撃開始処理
///-------------------------------------------///
void EnemyAttackComponent::StartAttack() {
	if (!isAttacking_ && cooldownTimer_ <= 0.0f && attackStrategy_) {
		isAttacking_ = true;
		attackStrategy_->OnAttackStart();
	}
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
EnemyAttackComponent::UpdateResult EnemyAttackComponent::Update(const UpdateContext& context) {
	UpdateResult result{};

	/// ===クールダウンタイマーの更新=== ///
	if (cooldownTimer_ > 0.0f) {
		cooldownTimer_ -= context.deltaTime;
	}

	/// ===攻撃中の処理=== ///
	if (isAttacking_ && attackStrategy_) {
		AttackExecutionContext execContext{
			.attackerPosition = context.currentPosition,
			.attackerDirection = context.currentDirection,
			.targetPosition = context.targetPosition,
			.deltaTime = context.deltaTime
		};

		auto execResult = attackStrategy_->Execute(execContext);

		result.velocity = execResult.velocity;
		result.isAttacking = true;

		// 攻撃完了チェック
		if (execResult.isAttackComplete) {
			attackStrategy_->OnAttackEnd();
			isAttacking_ = false;
			cooldownTimer_ = config_.interval;
			result.isAttackComplete = true;
		}
	}

	return result;
}

///-------------------------------------------/// 
/// ImGui情報の表示
///-------------------------------------------///
void EnemyAttackComponent::Information() {
#ifdef USE_IMGUI
	if (ImGui::TreeNode("攻撃情報")) {
		ImGui::DragFloat("攻撃範囲", &config_.range, 0.1f, 0.0f, 180.0f);
		ImGui::DragFloat("攻撃可能距離", &config_.distance, 0.1f, 0.0f, 50.0f);
		ImGui::DragFloat("攻撃間隔", &config_.interval, 0.1f, 0.1f, 10.0f);
		ImGui::DragInt("攻撃力", &config_.power, 1, 1, 100);

		ImGui::Separator();
		ImGui::Text("状態");
		ImGui::Text("攻撃中: %s", isAttacking_ ? "Yes" : "No");
		ImGui::Text("クールダウン: %. 2f", cooldownTimer_);

		// 戦略固有の設定
		if (attackStrategy_) {
			attackStrategy_->Information();
		}

		ImGui::TreePop();
	}
#endif
}

