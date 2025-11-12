#pragma once
/// ===Include=== ///
// GameCharacter
#include "application/Game/Entity/GameCharacter/GameCharacter.h"
// State
#include "State/Base/PlayerState.h"
// Weapon
#include "application/Game/Entity/Player/Weapon/PlayerWeapon.h"

/// ===前方宣言=== ///
class Enemy;

/// ===StateType=== ///
enum class actionType {
	kAvoidance,   // 回避状態
	kCharge,      // 突進状態
	kAttack       // 攻撃状態
};

///=====================================================/// 
/// Player
///=====================================================///
class Player : public GameCharacter<OBBCollider> {
public:

	Player() = default;
	~Player();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize()override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update()override;

	/// <summary>
	/// アニメーション時の更新処理
	/// </summary>
	void UpdateAnimation();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="mode"></param>
	void Draw(BlendMode mode = BlendMode::KBlendModeNormal)override;

	/// <summary>
	/// ImGui情報の表示
	/// </summary>
	void Information() override;

public: /// ===衝突判定=== ///
	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="collider">衝突した相手の Collider へのポインター。</param>
	void OnCollision(Collider* collider) override;

public: /// ===Getter=== ///

	// Weapon
	PlayerWeapon* GetWeapon() const;

	// フラグ
	bool GetStateFlag(actionType type) const;
	bool GetpreparationFlag(actionType type) const;
	
	// タイマー
	float GetTimer(actionType type);

public: /// ===Setter=== ///

	// Camera
	void SetCameraTargetPlayer();

	// フラグ
	void SetStateFlag(actionType type, bool flag);
	void SetpreparationFlag(actionType type, bool flag);
	
	// タイマーの設定
	void SetTimer(actionType type, const float& timer);
	void SetInvicibleTime(const float& time);

public: /// ===State用関数=== ///

	/// <summary>
	/// 減速処理
	/// </summary>
	/// <param name="develeration">適用する減速量を表す浮動小数点数への const 参照。</param>
	void ApplyDeceleration(const float& develeration);

	/// <summary>
	/// Stateの変更処理
	/// </summary>
	/// <param name="newState">新しい状態を表す std::unique_ptr<PlayerState>。この引数は呼び出し側からムーブされ、所有権は関数に移ります。</param>
	void ChangState(std::unique_ptr<PlayerState> newState);

private: /// ===変数の宣言=== ///

	GameCamera* camera_ = nullptr; // カメラ

	/// ===Weapon=== ///
	std::unique_ptr<PlayerWeapon> weapon_;

	/// ===State=== ///
	std::unique_ptr<PlayerState> currentState_;

	float collisionTimer_ = 0.0f; // 衝突タイマー

	/// ===無敵時間の情報=== ///
	struct InvicibleInfo {
		float time = 1.0f;   // 無敵時間
		float timer = 0.0f;  // 無敵タイマー
		bool isFlag = false; // 無敵フラグ
	};
	InvicibleInfo invicibleInfo_;

	/// ===溜め攻撃=== ///
	struct ChargeInfo {
		float timer = 0.0f;
		bool isPreparation = false; // 溜め攻撃の準備フラグ
		bool isFlag = false;        // 溜め攻撃のフラグ
	};
	ChargeInfo chargeInfo_;

	/// ===回避情報=== ///
	struct AvoidanceInfo {
		float timer = 0.0f;         // 回避のタイマー
		bool isPreparation = false; // 回避の準備フラグ
		bool isFlag = false;        // 回避のフラグ
	};
	AvoidanceInfo avoidanceInfo_;

	/// ===攻撃情報=== ///
	struct AttackInfo {
		float timer = 0.0f;         // 攻撃のタイマー
		bool isPreparation = false; // 攻撃の準備フラグ
		bool isFlag = false;        // 攻撃のフラグ
	};
	AttackInfo attackInfo_;

	/// ===移動情報=== ///
	struct MoveInfo {
		float speed = 0.5f;               // 移動速度
		Vector3 direction = { 0.0f, 0.0f, 0.0f };
	};
	MoveInfo moveInfo_;

private:

	/// <summary>
	/// タイマーを進める関数
	/// </summary>
	void advanceTimer();
};