#pragma once
/// ===Include=== ///
#include "application/Drawing/2d/Sprite.h"

/// ===前方宣言=== ///
class Player;

///-------------------------------------------/// 
/// プレイヤーUI
///-------------------------------------------///
class PlayerUI {
public:

	PlayerUI() = default;
	~PlayerUI();

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

public: /// ===Setter=== ///
	// Playerのセット
	void SetPlayer(Player* player) { player_ = player; }

private:
	// Player
	Player* player_;

	/// ===Sprite=== ///
	// アクション
	std::shared_ptr<Sprite> moveUI_;      // 移動UI
	std::shared_ptr<Sprite> cameraUI_;    // カメラUI
	std::shared_ptr<Sprite> attackUI_;    // 攻撃UI
	std::shared_ptr<Sprite> avoidanceUI_; // 回避UI
	// コントローラー
	std::shared_ptr<Sprite> xButton_;	  // X
	std::shared_ptr<Sprite> aButton_;	  // A
	std::shared_ptr<Sprite> leftStick_;	  // 左スティック
	std::shared_ptr<Sprite> rightStick_;  // 右スティック

	/// ===位置=== ///
	Vector2 moveUIPos_ = { 100.0f, 500.0f };      // 移動UI位置
	Vector2 cameraUIPos_ = { 300.0f, 500.0f };    // カメラUI位置

	/// ===Color=== ///
	Vector4 activeColor_ = { 1.0f, 0.0f, 0.0f, 1.0f };   // アクティブカラー

	/// ===フラグ=== ///
	struct ColorChange {
		bool attackUI = false;    // 攻撃UIアクティブフラグ
		bool avoidanceUI = false; // 回避UIアクティブフラグ
	};
	ColorChange colorChange_;

	/// ===参照スケール倍率=== ///
	Vector2 scale_ = { 1.0f, 1.0f }; // スケール

private:

	/// <summary>
	/// スプライトの更新処理
	/// </summary>
	void SpriteUpdate();

	/// <summary>
	/// 色の更新処理
	/// </summary>
	void ColorUpdate();
};

