#pragma once
/// ===Include=== ///
// GameCharacter
#include "application/Game/Entity/GameCharacter/GameCharacter.h"
// State
#include "application/Game/Entity/Enemy/State/Base/EnemyState.h"
// Component
#include "application/Game/Entity/Enemy/Component/EnemyMoveComponent.h"
#include "application/Game/Entity/Enemy/Component/EnemyAttackComponent.h"

/// ===前方宣言=== ///
class Player;

///=====================================================/// 
/// Enemy
///=====================================================///
class BaseEnemy : public GameCharacter<OBBCollider> {
private: /// ===型定義=== ///

	/// ===ノックバック情報=== ///
	struct KnockbackInfo {
		float cooldownTimer;		// クールタイム用タイマー
		float cooldownDuration;		// クールタイムの持続時間
		float knockbackForce;		// ノックバックの強さ
		float hitColorDuration;		// 赤色表示の持続時間
		float hitColorTimer;		// 赤色表示用タイマー
		Vector4 originalColor;		// 元の色
		bool isInCooldown;			// クールタイム中かどうか
	};

public:
	BaseEnemy() = default;
	virtual ~BaseEnemy();

	/// <summary>
	/// ゲームシーンで呼び出す初期化処理の純粋仮想関数
	/// </summary>
	/// <param name="translate">シーンの位置を指定する平行移動ベクトル。const参照で渡される。</param>
	virtual void InitGameScene(const Vector3& translate) = 0;

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
	/// ImGui情報の表std示
	/// </summary>
	virtual void Information()override;

	/// <summary>
	/// このオブジェクトのチューニング設定を指定した情報を他のBaseEnemyオブジェクトにコピー
	/// </summary>
	/// <param name="enemy">コピー先の BaseEnemy オブジェクトへのポインター。</param>
	void CopyTuningTo(BaseEnemy* enemy) const;

public: /// ===衝突判定=== ///

	/// <summary>
   /// 衝突時の処理
   /// </summary>
   /// <param name="collider">衝突した相手を表す Collider へのポインター。</param>
	void OnCollision(Collider* collider) override;

public: /// ===State用関数=== ///
	
	/// <summary>
	/// オブジェクトが攻撃可能かどうかを判定する関数。
	/// </summary>
	/// <returns>攻撃可能な場合は true、そうでない場合は false。</returns>
	bool CheckAttackable();

	/// <summary>
	/// 敵の状態を変更し、新しい状態オブジェクトの所有権を受け取る関数。
	/// </summary>
	/// <param name="nextState">次の状態を指す std::unique_ptr。所有権は関数にムーブされ、呼び出し側は所有権を失います。</param>
	void ChangeState(std::unique_ptr<EnemyState> nextState);

	/// <summary>
	/// 回転の更新処理
	/// </summary>
	/// <param name="direction">回転目標の方向</param>
	/// <param name="lerpT">回転完了までの時間</param>
	void UpdateRotationTowards(const Vector3& direction, float lerpT);

public: /// ===Getter=== ///
	// Component
	EnemyMoveComponent& GetMovementComponent() { return *moveComponent_; };
	EnemyAttackComponent& GetAttackComponent() { return *attackComponent_; };

	// Player
	Player* GetPlayer() const { return player_; };

	// tentativeDeah
	bool GetTentativeDeath() const { return isTentativeDeath_; };
	// isRotationComplete
	bool GetIsRotationComplete() const { return isRotationComplete_; };
	// isCollision
	bool GetIsCollision() const { return isCollision_; };

public: /// ===Setter=== ///
	// Player
	void SetPlayer(Player* player) { player_ = player; };
	// isRotationComplete
	void SetIsRotationComplete(bool flag) { isRotationComplete_ = flag; };

protected: /// ===変数の宣言=== ///

	GameCamera* camera_ = nullptr; // カメラ
	Player* player_ = nullptr; // Player

	/// ===State=== ///
	std::unique_ptr<EnemyState> currentState_;

	/// ===コンポーネント=== ///
	std::unique_ptr<EnemyMoveComponent> moveComponent_;
	std::unique_ptr<EnemyAttackComponent> attackComponent_;

	// 回転完了フラグ
	bool isRotationComplete_ = false;

	// 衝突フラグ
	bool isCollision_ = false;

private:
	// ノックバック情報
	KnockbackInfo knockbackInfo_;

	// 消えるまでの時間
	float disappearTimer_ = 2.0f;
	bool isTentativeDeath_ = false;

protected: /// ===関数の宣言=== ///

	/// <summary>
	/// 指定した攻撃戦略を用いて攻撃コンポーネントを初期化する。
	/// </summary>
	/// <param name="strategy">IAttackStrategy を指す std::unique_ptr。所有権は関数に移動され、この戦略でコンポーネントを初期化する。nullptr を渡した場合の挙動は実装による。</param>
	void InitializeAttackComponent(EnemyAttackComponent::AttackConfig config, std::unique_ptr<IAttackStrategy> strategy);

	/// <summary>
	/// 派生側で型固有のチューニング値をコピーするための関数
	/// </summary>
	/// <param name="enemy">親のBaseEnemy</param>
	virtual void CopyTypeTuningFromThisTo(BaseEnemy* enemy) const = 0;

private:

	/// <summary>
	/// タイマーを進める処理
	/// </summary>
	void advanceTimer();

};