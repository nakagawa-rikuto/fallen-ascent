#pragma once
/// ===Include=== ///
#include "PlayerUI.h"

/// ===前方宣言=== ///
class Player;

///=====================================================/// 
/// GameSceneUI
///=====================================================///
class GameSceneUI {
public:

	GameSceneUI() = default;
	~GameSceneUI() = default;

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
	void SetPlayer(Player* player);

private:

	/// ===Class=== ///
	// PlayerUI
	std::unique_ptr<PlayerUI> playerUI_;
};

