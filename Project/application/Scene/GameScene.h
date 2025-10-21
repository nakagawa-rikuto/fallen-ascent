#pragma once
/// ===Includ=== ///
#include "Engine/Scene/IScene.h"
// Entity
#include "application/Game/Entity/Player/Player.h"
#include "application/Game/Entity/Enemy/Manager/EnemyManager.h"
// Object
#include "application/Game/Object/Ground/Ground.h"
// Animation
#include "application/Game/Animation/StartAnimation.h"
// Transition
#include "application/Game/Transition/SceneTransition.h"

/// ===GameSceneのフェーズ=== ///
enum class GamePhase {
	FadeIn,          // フェードイン
	StartAnimation,  // 開始アニメーション
	Game,            // ゲームプレイ中
	FadeOut          // フェードアウト
};

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
	// Line
	std::unique_ptr<Line> line_;
	// Transition
	std::unique_ptr<SceneTransition> transiton_;
	// Camera
	std::shared_ptr<GameCamera> camera_;
	// Player
	std::unique_ptr<Player> player_;
	// Enemy
	std::unique_ptr<EnemyManager> enemyManager_;
	// Ground
	std::unique_ptr<Ground> ground_;
	// StartAnimation
	std::unique_ptr<StartAnimation> startAnimation_;

	/// ===変数の宣言=== ///
	// カメラ情報
	struct CameraInfo {
		Quaternion rotate = { 0.372f, 0.0f, 0.0f, 1.055f };
	};
	CameraInfo cameraInfo_;

	/// ===フェーズ管理=== ///
	GamePhase currentPhase_ = GamePhase::FadeIn;

	/// ===FadeIn用=== ///
	float fadeInDuration_ = 1.0f;

	/// ===FadeOut用=== ///
	float fadeOutTimer_ = 0.0f;
	float fadeOutDuration_ = 1.0f;

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
	/// フェードアウト時の更新処理
	/// </summary>
	void UpdateFadeOut();

	/// <summary>
	/// フェーズ遷移処理
	/// </summary>
	void ChangePhase(GamePhase newPhase);

	/// <summary>
	/// 配置処理
	/// </summary>
	/// <param name="json_name">使用するJsonファイル名</param>
	void SpawnObjects(const std::string& json_name);
};