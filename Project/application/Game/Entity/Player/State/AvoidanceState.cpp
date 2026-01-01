#include "AvoidanceState.h"
// Player, Camera
#include "application/Game/Entity/Player/Player.h"
// State
#include "RootState.h"

///-------------------------------------------/// 
/// コンストラクタ
///-------------------------------------------///
AvoidanceState::AvoidanceState(const Vector3& direction) { direction_ = direction;}

///-------------------------------------------/// 
/// 開始時に呼び出す
///-------------------------------------------///
void AvoidanceState::Enter(Player * player, GameCamera* camera) {
	// 引数の取得
	player_ = player;
	camera_ = camera;

	// 回避処理を開始
	player_->GetAvoidanceComponent()->StartAvoidance();
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void AvoidanceState::Update(Player * player, GameCamera* camera) {
	// 引数の取得
	player_ = player;
	camera_ = camera;

	/// ===回避処理=== ///
	PlayerAvoidanceComponent::UpdateContext context{
		.inputDirection = direction_,
		.deltaTime = player_->GetDeltaTime()
	};
	// 更新
	auto result = player_->GetAvoidanceComponent()->Update(context);

	// 結果を反映
	player_->SetVelocity(result.velocity);

	/// ===タイマーが時間を超えたら=== ///
	if (player_->GetAvoidanceComponent()->GetState().timer <= 0.0f) {
		// 状態をリセット
		player_->GetAvoidanceComponent()->ResetState();
		// Stateの変更
		player_->ChangState(std::make_unique<RootState>());
	}
}

///-------------------------------------------/// 
/// 終了時に呼び出す
///-------------------------------------------///
void AvoidanceState::Finalize() {
	PlayerState::Finalize();	
}
