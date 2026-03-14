#pragma once
/// ===Include=== ///
#include "Engine/Scene/IScene.h"
// Line
#include "application/Drawing/3d/Line.h"
// UI
#include "UI/TitleUI.h"
// Animation
#include "Animation/TitleSceneAnimation.h"
// C++
#include <random>

/// ===前方宣言=== ///
namespace MiiEngine {
	class ParticleGroup;
}

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
	
	/// ===ポインタ=== ///
	// パーティクル
	MiiEngine::ParticleGroup* particle_;

	/// ===Class=== ///
	// Line
	std::vector<std::unique_ptr<Line>> lines_;
	// カメラ
	std::unique_ptr<MiiEngine::NormalCamera> camera_; 
	// タイトルUI
	std::unique_ptr<TitleUI> titleUI_;
	// タイトルシーンアニメーション
	std::unique_ptr<TitleSceneAnimation> animation_; 

	/// ===Fade=== ///
	enum class FadeState {
		FadeIn,
		Selecting,
		FadeOut,
	};
	FadeState currentFade_ = FadeState::FadeIn;

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
};