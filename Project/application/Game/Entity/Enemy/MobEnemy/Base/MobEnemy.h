#pragma once
/// ===include=== ///
// BaseEnemy
#include "application/Game/Entity\Enemy/Base/BaseEnemy.h"
// State
#include "application/Game/Entity/Enemy/MobEnemy/State/Base/EnemyState.h"
// Component
#include "application/Game/Entity/Enemy/MobEnemy/Component/EnemyMoveComponent.h"
#include "application/Game/Entity/Enemy/MobEnemy/Component/EnemyHitReactionComponent.h"
// C++
#include <random>

/// ===前方宣言=== ///
class ParticleGroup;

///=====================================================/// 
/// MobEnemy
///=====================================================///
class MobEnemy : public BaseEnemy {
private: /// ===型定義=== ///

	/// ===攻撃情報=== ///
	struct AttackInfo {
		float timer;		// タイマー
		float range;		// 攻撃範囲(回転の情報から±)
		float distance;		// 攻撃可能距離
		float interval;		// 攻撃間隔
		int32_t power;		// 待機時間

		Vector3 direction;  // 攻撃方向
		Vector3 targetPos;  // プレイヤーの位置

		bool isAttack;		// 攻撃フラグ
	};

	/// ===無敵時間情報=== ///
	struct InvincibleInfo {
		float time = 0.5f;			// 無敵時間
		float timer = 0.0f;			// 無敵タイマー
		bool isInvincible = false;	// 無敵フラグ
	};

public:

	MobEnemy() = default;
	virtual ~MobEnemy();

	/// <summary>
	/// ゲームシーンを初期化
	/// </summary>
	/// <param name="translate">初期位置を表す3次元ベクトル。</param>
	virtual void InitGameScene(const Vector3& translate) override = 0;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update() override;

	/// <summary>
	/// 描画を実行します。
	/// </summary>
	/// <param name="mode">使用するブレンドモード。デフォルトは通常ブレンドモードです。</param>
	virtual void Draw(BlendMode mode = BlendMode::KBlendModeNormal) override;

	/// <summary>
	/// ImGui情報を表示
	/// </summary>
	virtual void Information() override;

	/// <summary>
	/// 攻撃処理の初期化
	/// </summary>
	virtual void StartAttack() = 0;

	/// <summary>
	/// 攻撃処理
	/// </summary>
	virtual void Attack() = 0;

public: /// ===衝突=== ///

	/// <summary>
	/// 衝突が発生したときに呼び出されます。
	/// </summary>
	/// <param name="collider">衝突したコライダーへのポインター。</param>
	virtual void OnCollision(Collider* collider) override;

public: /// ===その他関数=== ///

	// Stateの変更
	void ChangeState(std::unique_ptr<EnemyState> nextState);

	/// <summary>
	/// 攻撃可能かどうかを確認します。
	/// </summary>
	/// <returns>攻撃可能な場合は true、それ以外の場合は false。</returns>
	bool CheckAttackable();

public: /// ===Getter=== ///

	// Component
	EnemyMoveComponent& GetMovementComponent() const { return *moveComponent_; };
	EnemyHitReactionComponent& GetHitReactionComponent() const { return *hitReactionComponent_; };
	// AttackInfo
	AttackInfo GetAttackInfo()const { return attackInfo_; };
	// tentativeDeath
	bool GetTentativeDeath() const { return isTentativeDeath_; };

public: /// ===Setter=== ///
	
	// AttackDirection
	void SetAttackDirection(const Vector3& dir) { attackInfo_.direction = dir; };
	void SetTargetPos(const Vector3& pos) { attackInfo_.targetPos = pos; };
	// 無敵時間の設定
	void SetInvincibleTime();

protected: /// ===変数の宣言=== ///

	// 攻撃情報
	AttackInfo attackInfo_;

	// 無敵時間情報
	InvincibleInfo invincibleInfo_;

	// 衝突フラグ
	bool isCollision_ = false;

private:
	/// ===State=== ///
	std::unique_ptr<EnemyState> currentState_;

	/// ===コンポーネント=== ///
	std::unique_ptr<EnemyMoveComponent> moveComponent_;
	std::unique_ptr<EnemyHitReactionComponent> hitReactionComponent_;

	// Particle
	ParticleGroup* hitParticle_ = nullptr;
	ParticleGroup* deathParticle_ = nullptr;

	// 消えるまでの時間
	float disappearTimer_ = 2.0f;
private:

	/// <summary>
	/// タイマーを進める処理
	/// </summary>
	void advanceTimer();
};

