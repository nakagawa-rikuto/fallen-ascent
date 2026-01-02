#include "PlayerAvoidanceComponent.h"
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
void PlayerAvoidanceComponent::Initialize(const AvoidanceConfig& config) {
	// 設定の初期化
	config_ = config;

	// 初期化
	acceleration_ = 0.1f;
	direction_ = { 0.0f, 0.0f, 0.0f };
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
PlayerAvoidanceComponent::UpdateResult PlayerAvoidanceComponent::Update(const UpdateContext& context) {
	
	UpdateResult result{};

	// 加速度の減少
	acceleration_ -= context.deltaTime * config_.activeTime;
	// 速度の設定
	float speed = config_.speed * acceleration_;

	/// ===Velocityに反映=== ///
	result.velocity.x += context.inputDirection.x * speed;
	result.velocity.z += context.inputDirection.z * speed;

	return result;
}

///-------------------------------------------/// 
/// タイマーの更新処理
///-------------------------------------------///
void PlayerAvoidanceComponent::UpdateTimer(const float deltaTime) {
	if (state_.timer > 0.0f) {
		state_.timer -= deltaTime;
	} else {
		state_.isPreparation = true;
	}
}

///-------------------------------------------/// 
/// ImGui情報の表示
///-------------------------------------------///
void PlayerAvoidanceComponent::Information() {
#ifdef USE_IMGUI
	if (ImGui::TreeNode("回避情報")) {
		ImGui::DragFloat("回避速度", &config_.speed, 0.01f);
		ImGui::DragFloat("有効時間", &config_.activeTime, 0.01f);
		ImGui::DragFloat("クールタイム", &config_.coolTime, 0.01f);
		ImGui::DragFloat("無敵時間", &config_.invincibleTime, 0.01f);
		ImGui::Separator();
		ImGui::TreePop();
	}
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 回避処理の開始
///-------------------------------------------///
void PlayerAvoidanceComponent::StartAvoidance() {
	// 状態の初期化
	acceleration_ = 0.1f;
	state_.timer = config_.activeTime;
	state_.isActive = true;
}

///-------------------------------------------/// 
/// 状態のリセット
///-------------------------------------------///
void PlayerAvoidanceComponent::ResetState() {
	state_.isActive = false;
	state_.timer = config_.coolTime;
	state_.isPreparation = false;
}
