#pragma once
/// ===Include=== ///
/// ===GameCharacter=== ///
#include "application/Game/Entity/GameCharacter/GameCharacter.h"
// State
#include "State/Base/BossStateBase.h"
// Component
#include "Component/Move/BossMoveComponent.h"
#include "Component/Attack/BossAttackComponent.h"
//C++
#include <random>

/// ===前方宣言=== ///
class Player;
class ParticleGroup;

///=====================================================/// 
/// BossEnemy
///=====================================================///
class BossEnemy : GameCharacter<OBBCollider> {
public:

	BossEnemy() = default;
	~BossEnemy();
	
	/// <summary>
	/// ゲームシーンで呼び出す初期化処理の純粋仮想関数
	/// </summary>
	/// <param name="translate">シーンの位置を指定する平行移動ベクトル。const参照で渡される。</param>
	virtual void InitGameScene(const Vector3& translate);

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize()override;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update()override;

	/// <summary>
	/// アニメーション時の更新処理
	/// </summary>
	void UpdateAnimation();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="mode">描画に使用するブレンドモード。既定値は BlendMode::KBlendModeNormal。</param>
	virtual void Draw(BlendMode mode = BlendMode::KBlendModeNormal)override;

	/// <summary>
	/// ImGui情報の表示
	/// </summary>
	virtual void Information()override;

public: /// ===衝突判定=== ///
	
	/// <summary>
	/// 衝突時の処理
	/// </summary>
	/// <param name="collider">衝突した相手を表す Collider へのポインター。</param>
	void OnCollision(Collider* collider) override;

public: /// ===Getter=== ///

public: /// ===Setter=== ///

private:
	/// ===ポインタ=== ///
	GameCamera* camera_ = nullptr; // カメラ
	Player* player_ = nullptr;	   // プレイヤー

	/// ===State=== ///
	std::unique_ptr<BossStateBase> currentState_; // 現在のState

	/// ===Component=== ///
	std::unique_ptr<BossMoveComponent> moveComponent_;		// 移動コンポーネント
	std::unique_ptr<BossAttackComponent> attackComponent_;	// 攻撃コンポーネント
};

