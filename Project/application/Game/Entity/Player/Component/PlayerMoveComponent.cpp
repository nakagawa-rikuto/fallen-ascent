#include "PlayerMoveComponent.h"
// Math
#include "Math/sMath.h"
// C++
#include <cassert>
// ImGui
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI

///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void PlayerMoveComponent::Initialize(const MoveConfig& config) {
	// 設定の初期化
	config_ = config;

	// 現在の移動方向の初期化
	currentDirection_ = { 0.0f, 0.0f, 0.0f };
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
PlayerMoveComponent::UpdateResult PlayerMoveComponent::Update(const UpdateContext & context) {
	
	UpdateResult result{};

	/// ===値の取得=== ///
	currentDirection_ = context.inputDirection;

	/// ===Velocityに反映=== ///
	result.velocity = currentDirection_ * config_.speed;

	/// ===移動方向に沿って回転=== ///
	// 方向が変更されたら
	if (Length(currentDirection_) > 0.01f) {
		// 現在のYaw角(Y軸の回転)を取得
		float currentYaw = Math::GetYAngle(context.currentRotation);
		// 入力方向から目標のYaw角を取得
		float targetYaw = std::atan2(currentDirection_.x, currentDirection_.z);
		// 差分を [-π, π] に正規化 
		float diff = Math::NormalizeAngle(targetYaw - currentYaw);
		// イージング補間（短い方向へ回転）
		float easedYaw = currentYaw + diff * (context.deltaTime * 10.0f);
		// Quaternionに再変換
		result.targetRotation = Math::MakeRotateAxisAngle({ 0, 1, 0 }, easedYaw);
	} else {
		// 入力がない場合は現在の回転を維持
		result.targetRotation = context.currentRotation;
	}

	return result;
}

///-------------------------------------------/// 
/// ImGui情報の表示
///-------------------------------------------///
void PlayerMoveComponent::Information() {
#ifdef USE_IMGUI
	if (ImGui::TreeNode("移動情報")) {
		ImGui::DragFloat("移動速度", &config_.speed, 0.01f);
		ImGui::DragFloat("回転速度", &config_.rotationSpeed, 0.01f);
		ImGui::DragFloat("減速率", &config_.deceleration, 0.01f, 0.0f, 1.0f);

		ImGui::Separator();
		ImGui::Text("現在の移動方向");
		ImGui::Text("X: %.2f", currentDirection_.x);
		ImGui::Text("Y: %.2f", currentDirection_.y);
		ImGui::Text("Z: %.2f", currentDirection_.z);
		ImGui::TreePop();
	}
#endif // USE_IMGUI
}