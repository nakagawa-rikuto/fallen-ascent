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
	Vector2 centerScale_ = { 1.0f, 1.0f }; // 中央のスケール
	Vector2 sideScale_ = { 0.6f, 0.6f };   // 側面のスケール

	/// ===メニュー選択=== ///
	enum class MenuSelection {
		Start,    // 開始
		Option,   // オプション
		Exit,     // 終了
		Count	  // メニュー項目の数
	};
	static constexpr size_t selectionIndex_ = static_cast<size_t>(MenuSelection::Count); // メニュー項目数
	MenuSelection currentSelection_ = MenuSelection::Start; // 現在の選択

	/// ===フラグ=== ///
	bool isOptionOpen_ = false;      // オプション画面が開いているか
	bool isGameStart_ = false;      // ゲーム開始フラグ

	/// ===アニメーションのY座標=== ///
	struct AnimationPosY {
		float itemStartY[selectionIndex_] = {};	// メニュー項目の開始Y座標
		float itemTargetY[selectionIndex_] = {};	// メニュー項目の目標Y座標
		float itemCurrentY[selectionIndex_] = {};	// メニュー項目の現在Y座標
	};

	/// ===アニメーションのスケール=== ///
	struct AnimationScale {
		Vector2 itemStartScale[selectionIndex_] = {};   // メニュー項目の開始スケール
		Vector2 itemTargetScale[selectionIndex_] = {};  // メニュー項目の目標スケール
		Vector2 itemCurrentScale[selectionIndex_] = {}; // メニュー項目の現在スケール
	};

	/// ===アニメーション用=== ///
	struct SlotAnimation {
		float animationTimer = 0.0f;	// アニメーションタイマー
		float animationSpeed = 0.5f;	// アニメーション速度
		bool isAnimating = false;		// アニメーション中かどうか
		AnimationPosY posY;				// Y座標のアニメーションデータ
		AnimationScale scale;			// スケールのアニメーションデータ
	};
	SlotAnimation slotAnimation_;

	// スロットの各スロットが表すメニュー項目のインデックス
	int slotIndex_[selectionIndex_] = { 2, 0, 1 }; 

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
	/// アニーメーションを更新
	/// </summary>
	/// <param name="deltaTime">デルタタイム</param>
	void UpdateAnimation(float deltaTime);

	/// <summary>
	/// スプライトの位置更新
	/// </summary>
	void UpdateSlotSprites();

	/// <summary>
	/// 選択状態を更新します。
	/// </summary>
	void UpdateSelecting();

	/// <summary>
	/// メニューの選択状態の更新処理
	/// </summary>
	void UpdateMenuSelection();

	/// <summary>
	/// アニメーションの開始
	/// </summary>
	void StartSlotAnimation(bool flag);

	/// <summary>
	/// メニューの選択決定処理
	/// </summary>
	void ConfirmSelection();

	/// <summary>
	/// オプション画面の更新処理
	/// </summary>
	void UpdateOptionMenu();
};

