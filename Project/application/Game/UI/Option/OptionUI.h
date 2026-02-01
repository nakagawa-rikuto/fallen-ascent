#pragma once
/// ===Include=== ///
// Sprite
#include "application/Drawing/2d/Sprite.h"
// C++
#include <memory>

///=====================================================/// 
/// オプションUIクラス
///=====================================================///
class OptionUI {
public:
	OptionUI() = default;
	~OptionUI();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理(タイトル)
	/// </summary>
	void TitleUpdate();

	/// <summary>
	/// 更新処理(ゲーム中)
	/// </summary>
	void GameUpdate();

	/// <summary>
	/// 描画処理
	/// </summary>
	void TitleDraw();

	/// <summary>
	/// 描画処理
	/// </summary>
	void GameDraw();

public: /// ===Getter=== ///
	// タイトルに戻るフラグの取得
	bool GetReturnToTitle() const { return returnToTitle_; }

private: /// ===メンバ変数=== ///

	/// ===スプライト=== ///
	std::unique_ptr<Sprite> dimSprite_;          // 薄暗いオーバーレイ
	std::unique_ptr<Sprite> titleButtonSprite_;  // タイトルに戻るボタン
	std::unique_ptr<Sprite> audioButtonSprite_;  // オーディオボタン
	std::unique_ptr<Sprite> audioTextSprite_;    // 音量調整テキスト
	std::unique_ptr<Sprite> audioSliderSprite_;  // 音量調整スライダー
	std::unique_ptr<Sprite> audioHandleSprite_;  // 音量調整ハンドル

	/// ===参照スケール倍率=== ///
	Vector2 scale_ = { 1.0f, 1.0f };

	/// ===Game用=== ///
	enum class OptionState {
		Audio,
		Title,
	};

	int volumeBGM_ = 100; // BGM音量（0～100）
	bool returnToTitle_ = false; // タイトルに戻るフラグ
	OptionState optionState_ = OptionState::Audio; // オプションの状態
	int stickNo_ = 0; // コントローラー番号

	// デバッグ用ImGui
	Vector2 spritePos_ = { 592.0f, 445.0f };
	Vector2 spritePos_2_ = { 605.0f, 457.0f };
	Vector2 audioButtonPos_ = { 690.0f, 400.0f };
	Vector2 spriteSize_ = { 728.0f, 71.0f };
	Vector2 spriteSize_2_ = { 7.0f, 50.0f };

private:

	/// <summary>
	/// Audioスプライトの更新処理
	/// </summary>
	void UpdateAudioSprite();
};

