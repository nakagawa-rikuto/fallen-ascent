#pragma once
/// ===前方宣言=== ///
class Player;
class GameCamera;

///=====================================================/// 
/// プレイヤーのベースステート
///=====================================================///
class PlayerState {
public:
	virtual ~PlayerState() = default;

	/// <summary>
	/// ステートに入った時に呼ばれる処理、純粋仮想関数
	/// </summary>
	/// <param name="player">Enter 処理の対象となる Player オブジェクトへのポインタ。</param>
	/// <param name="camera">処理時に参照または操作する GameCamera オブジェクトへのポインタ。</param>
	virtual void Enter(Player* player, GameCamera* camera) = 0;

	/// <summary>
	/// ステート時の更新処理、純粋仮想関数
	/// </summary>
	/// <param name="player">更新対象のプレイヤーを指すポインタ。</param>
	/// <param name="camera">現在のゲームカメラを指すポインタ。</param>
	virtual void Update(Player* player, GameCamera* camera) = 0;

	/// <summary>
	/// ステートの終了処理
	/// </summary>
	virtual void Finalize();

protected:

	Player* player_ = nullptr; // Player
	GameCamera* camera_ = nullptr; // Camera
};
