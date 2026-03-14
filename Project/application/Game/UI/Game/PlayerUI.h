#pragma once
/// ===Include=== ///
#include "application/Drawing/2d/Object2d.h"

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

public: /// ===Setter=== ///
	// Playerのセット
	void SetPlayer(Player* player) { player_ = player; }

private:
	// Player
	Player* player_;

	/// ===Object2d=== ///
	// アクション
	std::shared_ptr<Object2d> moveUI_;      // 移動UI
	std::shared_ptr<Object2d> cameraUI_;    // カメラUI
	std::shared_ptr<Object2d> attackUI_;    // 攻撃UI
	std::shared_ptr<Object2d> avoidanceUI_; // 回避UI
	// コントローラー
	std::shared_ptr<Object2d> xButton_;	  // X
	std::shared_ptr<Object2d> aButton_;	  // A
	std::shared_ptr<Object2d> leftStick_;	  // 左スティック
	std::shared_ptr<Object2d> rightStick_;  // 右スティック

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

