#pragma once
/// ===Include=== ///
#include "Engine/Scene/IScene.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// Stage
#include "application/Game/Object/GameStage.h"
// UI
#include "UI/TitleUI.h"
// Animation
#include "Animation/TitleSceneAnimation.h"

///=====================================================/// 
/// タイトルシーン
///=====================================================///
class TitleScene : public MiiEngine::IScene {
public:/// ===メンバ関数=== ///

	TitleScene() = default;
	~TitleScene();

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
	
	/// ===Class=== ///
	std::shared_ptr<GameCamera> camera_; // カメラ
	std::unique_ptr<Player> player_;	 // プレイヤー
	std::unique_ptr<GameStage> stage_;	 // ステージ
	std::unique_ptr<TitleUI> titleUI_;   // タイトルUI
	std::unique_ptr<TitleSceneAnimation> animation_; // タイトルシーンアニメーション

	/// ===Fade=== ///
	enum class FadeState {
		FadeIn,
		Selecting,
		FadeOut,
	};
	FadeState currentFade_ = FadeState::FadeIn;

	/// ===カメラ=== ///
	Vector3 cameraOrbitingOffset_{ 0.0f, 10.0f, -100.0f }; // カメラOffSet
	Quaternion cameraRotation_{ 0.158f, 0.0f, 0.0f, 0.987f };   // カメラ位置

	// フェードアウトフラグ
	bool isStartFadeOut_ = false;

private:/// ===プライベート関数=== ///

	/// <summary>
	/// フェードインの進行状態を更新します。
	/// </summary>
	void UpdateFadeIn();

	/// <summary>
	/// フェードアウトの状態を更新します。
	/// </summary>
	void UpdateFadeOut();

	/// <summary>
	/// プレイヤーを生成します。
	/// </summary>
	/// <param name="json">プレイヤーの設定が記述されたJSON文字列</param>
	void SpawnPlayer(const std::string& json_name);
};