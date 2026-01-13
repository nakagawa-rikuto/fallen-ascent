#pragma once
/// ===Include=== ///

///-------------------------------------------/// 
/// プレイヤーUI
///-------------------------------------------///
class PlayerUI {
public:
	PlayerUI() = default;
	~PlayerUI() = default;

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
};

