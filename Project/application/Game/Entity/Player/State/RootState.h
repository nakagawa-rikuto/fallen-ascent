#pragma once
/// ===Include=== ///
#include "Base/PlayerState.h"

///=====================================================/// 
/// プレイヤーの通常状態のステート
///=====================================================///
class RootState : public PlayerState {
public:

	~RootState() override = default;

	/// <summary>
	/// ステートに入った時に呼ばれる処理
	/// </summary>
	/// <param name="player">処理対象の Player オブジェクトへのポインタ。</param>
	/// <param name="camera">使用する GameCamera オブジェクトへのポインタ。</param>
	void Enter(Player* player, GameCamera* camera) override;

	/// <summary>
	/// ステート時の更新処理
	/// </summary>
	/// <param name="player">更新対象の Player オブジェクトを指すポインタ。</param>
	/// <param name="camera">更新対象の GameCamera オブジェクトを指すポインタ。</param>
	void Update(Player* player, GameCamera* camera) override;

	/// <summary>
	/// ステートの終了処理
	/// </summary>
	void Finalize() override;
};

