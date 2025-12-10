#pragma once
/// ===Include=== ///
// IScene
#include "Engine/Scene/IScene.h"
#include "application/Drawing/2d/Sprite.h"
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

	/// ===スプライト=== ///
	std::unique_ptr<Sprite> bgSprite_;         // 背景
	std::unique_ptr<Sprite> bgKiriSprite_;     // 背景
	std::unique_ptr<Sprite> titleSprite_;      // タイトル
	std::unique_ptr<Sprite> startSprite_;      // 開始
	std::unique_ptr<Sprite> optionSprite_;     // オプション
	std::unique_ptr<Sprite> exitSprite_;       // 終了
	std::unique_ptr<Sprite> selectOverlay_;    // 選択中のオーバーレイ
	std::unique_ptr<Sprite> dimSprite_;        // 薄暗いオーバーレイ
	std::unique_ptr<Sprite> optionMenuSprite_; // オプションメニュー

	/// ===ポイントの値=== ///
	float startY_ = 0.0f;
	float spaceY_ = 0.0f;

	/// ===Fade=== ///
	enum class FadeState {
		FadeIn,
		Selecting,
		FadeOut,
	};
	FadeState currentFade_ = FadeState::FadeIn;

	/// ===メニュー選択=== ///
	// メニュー項目の列挙型
	enum class MenuSelection {
		Start,    // 開始
		Option,   // オプション
		Exit      // 終了
	};
	MenuSelection currentSelection_ = MenuSelection::Start; // 現在の選択
	bool isOptionOpen_ = false;      // オプション画面が開いているか

	/// ===入力制御=== ///
	bool canInput_ = true; // 入力可能フラグ（連続入力防止）

private:/// ===プライベート関数=== ///

	/// <summary>
	/// フェードインの進行状態を更新します。
	/// </summary>
	void UpdateFadeIn();

	/// <summary>
	/// 選択状態を更新します。
	/// </summary>
	void UpdateSelecting();

	/// <summary>
	/// フェードアウトの状態を更新します。
	/// </summary>
	void UpdateFadeOut();

	/// <summary>
	/// メニューの選択状態の更新処理
	/// </summary>
	void UpdateMenuSelection();

	/// <summary>
	/// メニューの選択決定処理
	/// </summary>
	void ConfirmSelection();

	/// <summary>
	/// 選択オーバーレイの位置を更新処理
	/// </summary>
	void UpdateSelectOverlayPosition();

	/// <summary>
	/// オプション画面の更新処理
	/// </summary>
	void UpdateOptionMenu();
};