#pragma once
/// ===Include=== ///
// IScene
#include "Engine/Scene/IScene.h"
// UI
#include "application/Game/UI/Title/TitleUI.h"
// C++
#include <memory>
#include <array>

///=====================================================/// 
/// タイトルシーン
///=====================================================///
class TitleScene : public IScene {
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
	/// <summary>
	/// シーン用
	/// </summary>
	
	/// ===Class=== ///
	std::unique_ptr<TitleUI> titleUI_;     // タイトルUI

	/// ===Fade=== ///
	enum class FadeState {
		FadeIn,
		Selecting,
		FadeOut,
	};
	FadeState currentFade_ = FadeState::FadeIn;


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