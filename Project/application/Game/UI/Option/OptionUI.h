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
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private: /// ===メンバ変数=== ///

	/// ===スプライト=== ///
	std::unique_ptr<Sprite> dimSprite_;        // 薄暗いオーバーレイ
	std::unique_ptr<Sprite> optionMenuSprite_; // オプションメニュー

	/// ===参照スケール倍率=== ///
	Vector2 scale_ = { 1.0f, 1.0f };
};

