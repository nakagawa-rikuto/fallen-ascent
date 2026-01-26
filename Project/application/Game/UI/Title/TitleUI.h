#pragma once
/// ===Include=== ///
// Sprite
#include "application/Drawing/2d/Sprite.h"
#include "application/Game/UI/Option/OptionUI.h"
// C++
#include <memory>

///=====================================================/// 
/// タイトルシーンのUIクラス
///=====================================================///
class TitleUI {
public:
	TitleUI() = default;
	~TitleUI();

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

	// オプションUI
	std::unique_ptr<OptionUI> optionUI_;

	/// ===スプライト=== ///
	std::unique_ptr<Sprite> bgSprite_;         // 背景
	std::unique_ptr<Sprite> bgKiriSprite_;     // 背景(霧)
	std::unique_ptr<Sprite> titleSprite_;      // タイトル

	/// ===セレクトスプライト=== ///
	struct SelectSprite {
		std::unique_ptr<Sprite> start_;      // 開始
		std::unique_ptr<Sprite> option_;     // オプション
		std::unique_ptr<Sprite> exit_;       // 終了
		std::unique_ptr<Sprite> overlay_;    // 選択中のオーバーレイ
	};
	SelectSprite selectSprite_;

	/// ===基準値=== ///
	Vector2 startPos_{}; // メニュー開始Y座標
	float spaceY_ = 0.0f; // メニュー項目間のスペース

	/// ===参照スケール倍率=== ///
	Vector2 scale_ = { 1.0f, 1.0f };

	/// ===メニュー選択=== ///
	// メニュー項目の列挙型
	enum class MenuSelection {
		Start,    // 開始
		Option,   // オプション
		Exit      // 終了
	};
	MenuSelection currentSelection_ = MenuSelection::Start; // 現在の選択

	/// ===フラグ=== ///
	bool isOptionOpen_ = false;      // オプション画面が開いているか
	bool isGameStart_ = false;      // ゲーム開始フラグ

public: /// ===Getter=== ///
	/// <summary>
	/// 選択スプライトを取得します。
	/// </summary>
	/// <returns>選択スプライトへの参照。</returns>
	SelectSprite& GetSelectSprite() { return selectSprite_; }

	/// <summary>
	/// ゲームが開始されているかどうかを取得します。
	/// </summary>
	/// <returns>ゲームが開始されている場合は true、それ以外の場合は false。</returns>
	bool IsGameStart() const { return isGameStart_; }

private:
	/// <summary>
	/// 選択状態を更新します。
	/// </summary>
	void UpdateSelecting();

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

