#include "EnemyMoveComponent.h"
// BaseEnemy
#include "application/Game/Entity/Enemy/Base/BaseEnemy.h"
// C++
#include <cassert>
// ImGui
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI


///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void EnemyMoveComponent::Initialize(const Vector3& center, const MoveConfig& config) {
	// ランダムエンジンの初期化
	std::seed_seq seed{
		static_cast<uint32_t>(std::time(nullptr)),
		static_cast<uint32_t>(reinterpret_cast<uintptr_t>(this)),
	};
	randomEngine_.seed(seed);

	// 設定の初期化
	config_ = config;

	// 状態の初期化
	state_.timer = 1.0f;
	state_.rangeCenter = center;
	state_.direction = { 0.0f, 0.0f, 0.0f };
	state_.isWaiting = false;
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
EnemyMoveComponent::UpdateResult EnemyMoveComponent::Update(const UpdateContext& context) {

	UpdateResult result{};

	/// ===タイマー=== ///
	if (state_.timer > 0.0f) {
		state_.timer -= context.deltaTime;
	}

	/// ===計算=== ///
	// 移動範囲の中心との方向ベクトルを計算
	Vector3 toCenter = state_.rangeCenter - context.currentPosition;
	// 中心からの距離を取得
	float distanceFromCenter = Length(toCenter);

	// 移動ベクトルを設定
	result.velocity = state_.direction * config_.speed;

	/// ===移動処理=== ///
	if (state_.isWaiting) { // 待機中だったら
		// 移動しない
		result.velocity = { 0.0f, 0.0f, 0.0f };

		// 方向を回転
		if (!context.isRotationComplete) {
			result.targetDirection = state_.direction;
			result.needsRotation = true;
		}
		
		// 回転し終わったら
		if (context.isRotationComplete && state_.timer <= 0.0f) {
			// ランダムな時間を設定
			std::uniform_real_distribution<float> timeDist(1.0f, config_.interval);
			state_.timer = timeDist(randomEngine_);

			// 移動ベクトルを設定
			result.velocity = state_.direction * config_.speed;
			// 待機フラグを解除
			state_.isWaiting = false;
			// 回転完了フラグをリセット
			result.shouldResetRotationFlag = true;
			result.needsRotation = false;
		}

	} else if (distanceFromCenter > config_.range) { // 範囲外に出た場合
		// 方向の設定と待機処理の準備
		PreparNextMove(toCenter);
		result.velocity = { 0.0f, 0.0f, 0.0f };

	} else if (state_.timer <= 0.0f && !state_.isWaiting) {
		// ランダムな角度と距離を生成
		std::uniform_real_distribution<float> angleDist(0.0f, 2.0f * Math::Pi());
		std::uniform_real_distribution<float> distanceDist(0.0f, config_.range);
		// ランダムな値の設定
		float angle = angleDist(randomEngine_);
		float distance = distanceDist(randomEngine_);

		// 方向ベクトルを円から算出
		Vector3 offset = {
			std::cos(angle) * distance,
			0.0f,
			std::sin(angle) * distance
		};

		// 移動先の座標を計算
		Vector3 target = state_.rangeCenter + offset;
		target.y = context.currentPosition.y;

		// 方向の設定と待機処理の準備
		PreparNextMove(target - context.currentPosition);
		result.velocity = { 0.0f, 0.0f, 0.0f };
	}

	return result;
}

///-------------------------------------------/// 
/// ImGui情報の表示
///-------------------------------------------///
void EnemyMoveComponent::Information() {
#ifdef USE_IMGUI
	if (ImGui::TreeNode("移動情報")) {
		ImGui::DragFloat("移動速度", &config_.speed, 0.01f, 0.0f, 10.0f);
		ImGui::DragFloat("移動範囲", &config_.range, 0.1f, 0.1f, 100.0f);
		ImGui::DragFloat("移動間隔", &config_.interval, 0.1f, 0.1f, 10.0f);
		ImGui::DragFloat("待機時間", &config_.waitTime, 0.01f, 0.0f, 5.0f);
		ImGui::DragFloat("回転速度", &config_.rotationSpeed, 0.01f, 0.01f, 1.0f);

		ImGui::Separator();
		ImGui::Text("状態");
		ImGui::Text("タイマー:  %.2f", state_.timer);
		ImGui::Text("待機中: %s", state_.isWaiting ? "Yes" : "No");

		ImGui::TreePop();
	}
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 設定の適用
///-------------------------------------------///
void EnemyMoveComponent::ApplyConfig(const MoveConfig& newconfig) {
#ifdef _DEBUG
	if (newconfig.speed < 0.0f || newconfig.range <= 0.0f) {
		// デバッグビルドでのみエラー
		assert(false && "Invalid MoveConfig");
	}
#endif
	config_ = newconfig;
}

///-------------------------------------------/// 
/// 方向の設定と待機時間の設定
///-------------------------------------------///
void EnemyMoveComponent::PreparNextMove(const Vector3& vector) {
	Vector3 dir = Normalize(vector);
	dir.y = 0.0f; // Y成分を0にしてXZ平面での方向ベクトルを作成
	state_.direction = Normalize(dir); // 方向を保存

	// 待機時間を設定
	state_.timer = config_.waitTime; // 待機時間を設定
	// 待機フラグを立てる
	state_.isWaiting = true;
}
