#include "LongRangeAttackStrategy.h"
// ImGui
#ifdef USE_IMGUI
#include <imgui.h>
#endif

///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void LongRangeAttackStrategy::Initialize(const Config& config) {
	config_ = config;
}

///-------------------------------------------/// 
/// 攻撃開始時の処理
///-------------------------------------------///
void LongRangeAttackStrategy::OnAttackStart() {
	shootTimer_ = 0.0f;
	bulletsShot_ = 0;
	isReloading_ = false;
}

///-------------------------------------------/// 
/// 攻撃実行処理
///-------------------------------------------///
AttackExecutionResult LongRangeAttackStrategy::Execute(const AttackExecutionContext& context) {
	AttackExecutionResult result{};
	result.didAttack = true;
	result.velocity = { 0.0f, 0.0f, 0.0f };  // 遠距離攻撃中は移動しない

	shootTimer_ -= context.deltaTime;

	if (!isReloading_) {
		if (shootTimer_ <= 0.0f && bulletsShot_ < config_.maxBullets) {
			// 弾を発射
			ShootBullet(context);
			bulletsShot_++;
			shootTimer_ = config_.shootInterval;
		}

		if (bulletsShot_ >= config_.maxBullets) {
			// リロード開始
			isReloading_ = true;
			shootTimer_ = config_.reloadTime;
		}
	} else {
		// リロード中
		if (shootTimer_ <= 0.0f) {
			// 攻撃完了
			result.isAttackComplete = true;
		}
	}

	// 弾の更新
	UpdateBullets();

	return result;
}

///-------------------------------------------/// 
/// 攻撃終了時の処理
///-------------------------------------------///
void LongRangeAttackStrategy::OnAttackEnd() {
	shootTimer_ = 0.0f;
	bulletsShot_ = 0;
	isReloading_ = false;
}

///-------------------------------------------/// 
/// Information
///-------------------------------------------///
void LongRangeAttackStrategy::Information() {
#ifdef USE_IMGUI
	if (ImGui::TreeNode("遠距離攻撃設定")) {
		ImGui::DragFloat("発射間隔", &config_.shootInterval, 0.01f, 0.1f, 2.0f);
		ImGui::DragFloat("リロード時間", &config_.reloadTime, 0.1f, 1.0f, 10.0f);
		ImGui::DragInt("最大弾数", &config_.maxBullets, 1, 1, 10);
		ImGui::TreePop();
	}
#endif
}

///-------------------------------------------/// 
/// 弾の更新
///-------------------------------------------///
void LongRangeAttackStrategy::UpdateBullets() {
    // LongRangeEnemyBullet の完全な型が必要なため、cppでインクルード必須
    bullets_.remove_if([](const std::unique_ptr<LongRangeEnemeyBullet>& bullet) {
        return !bullet->GetIsAlive();
    });

    for (auto& bullet : bullets_) {
        bullet->Update();
    }
}

///-------------------------------------------/// 
/// 弾の描画
///-------------------------------------------///
void LongRangeAttackStrategy::DrawBullets() {
	for (auto& bullet : bullets_) {
		bullet->Draw();
	}
}

///-------------------------------------------/// 
/// 弾を発射する
///-------------------------------------------///
void LongRangeAttackStrategy::ShootBullet(const AttackExecutionContext& context) {
	auto bullet = std::make_unique<LongRangeEnemeyBullet>();
	bullet->Initialize();
	bullet->Create(context.attackerPosition, context.attackerDirection);
	bullets_.push_back(std::move(bullet));
}
