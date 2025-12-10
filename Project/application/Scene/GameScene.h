#pragma once
/// ===Includ=== ///
#include "Engine/Scene/IScene.h"
// Entity
#include "application/Game/Entity/Player/Player.h"
#include "application/Game/Entity/Enemy/Manager/EnemyManager.h"
#include "application/Game/Entity/Enemy/CloseRange/CloseRangeEnemy.h"
#include "application/Game/Entity/Enemy/LongRange/LongRangeEnemy.h"
// Object
#include "application/Game/Object/GameStage.h"
// Animation
#include "application/Game/Animation/StartAnimation.h"
#include "application/Game/Animation/GameOverAnimation.h"
#include "application/Game/Animation/GameClearAnimation.h"

///=====================================================/// 
/// ゲームシーン
///=====================================================///
class GameScene : public IScene {
public:/// ===メンバ関数=== ///

	GameScene() = default;
	~GameScene();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:/// ===メンバ変数=== ///
	/// <summary>
	/// シーン用
	/// </summary>

	/// ===Classの宣言=== /// 
	// Camera
	std::shared_ptr<GameCamera> camera_;
	// Player
	std::unique_ptr<Player> player_;
	// Enemy
	std::unique_ptr<EnemyManager> enemyManager_;
	// Enemy
	std::unique_ptr<CloseRangeEnemy> closeRangeEnemy_;
	std::unique_ptr<LongRangeEnemy> longRangeEnemy_;
	// Stage
	std::unique_ptr<GameStage> stage_;
	// StartAnimation
	std::unique_ptr<StartAnimation> startAnimation_;
	// GameOverAnimation
	std::unique_ptr<GameOverAnimation> gameOverAnimation_;
	// GameClearAnimation
	std::unique_ptr<GameClearAnimation> gameClearAnimation_;

	/// ===変数の宣言=== ///
	// カメラ情報
	struct CameraInfo {
		Quaternion rotate = { 0.372f, 0.0f, 0.0f, 1.055f };
	};
	CameraInfo cameraInfo_;

	/// ===フェーズ管理=== ///
	// ゲームの進行フェーズ
	enum class FadeState {
		FadeIn,				// フェードイン
		StartAnimation,		// 開始アニメーション
		Game,				// ゲームプレイ中
		GameOverAnimation,  // ゲームオーバー
		GameClearAnimation  // フェードアウト
	};
	FadeState currentPhase_ = FadeState::FadeIn;

private: /// ===メンバ関数=== ///

	/// <summary>
	/// フェードイン時の更新処理
	/// </summary>
	void UpdateFadeIn();

	/// <summary>
	/// 開始アニメーション時の更新処理
	/// </summary>
	void UpdateStartAnimation();

	/// <summary>
	/// ゲーム時の更新処理
	/// </summary>
	void UpdateGame();

	/// <summary>
	/// ゲームオーバー時のアニメーションを更新処理
	/// </summary>
	void UpdateGameOverAnimation();

	/// <summary>
	/// フェードアウト時の更新処理
	/// </summary>
	void UpdateGameClearAnimtaion();

	/// <summary>
	/// 配置処理
	/// </summary>
	/// <param name="json_name">使用するJsonファイル名</param>
	void SpawnEntity(const std::string& json_name);
};