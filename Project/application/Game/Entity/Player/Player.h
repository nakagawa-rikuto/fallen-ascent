#pragma once
/// ===Include=== ///
// GameCharacter
#include "application/Game/Entity/GameCharacter/GameCharacter.h"
// Component
#include "Component/PlayerMoveComponent.h"
#include "Component/PlayerAvoidanceComponent.h"
#include "Component/PlayerAttackComponent.h"
// State
#include "State/Base/PlayerState.h"
// Weapon
#include "application/Game/Entity/Player/Weapon/PlayerWeapon.h"

/// ===前方宣言=== ///
class Enemy;

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
	PlayerWeapon* GetWeapon() const { return weapon_.get(); };

	// Component
	PlayerMoveComponent* GetMoveComponent() const { return moveComponent_.get(); };
	PlayerAvoidanceComponent* GetAvoidanceComponent() const { return avoidanceComponent_.get(); };
	PlayerAttackComponent* GetAttackComponent() const { return attackComponent_.get(); };

	// StickState
	Vector2 GetLeftStickState() const { return stickState_.leftStick; };
	Vector2 GetRightStickState() const { return stickState_.rightStick; };

public: /// ===Setter=== ///

	// Camera
	void SetCameraTargetPlayer();
	// 無敵時間の設定
	void SetInvincibleTime(const float& time);
	// Gravityの設定
	void SetGravity(const float& gravity) { baseInfo_.gravity = gravity; };

public: /// ===State用関数=== ///

	/// <summary>
	/// 減速処理
	/// </summary>
	/// <param name="deceleration">適用する減速量を表す浮動小数点数への const 参照。</param>
	void ApplyDeceleration(const float& deceleration);

	/// <summary>
	/// Stateの変更処理
	/// </summary>
	/// <param name="newState">新しい状態を表す std::unique_ptr<PlayerState>。この引数は呼び出し側からムーブされ、所有権は関数に移ります。</param>
	void ChangState(std::unique_ptr<PlayerState> newState);

private: /// ===変数の宣言=== ///
	// カメラ
	GameCamera* camera_ = nullptr; 

	/// ===Weapon=== ///
	std::unique_ptr<PlayerWeapon> weapon_;

	/// ===Component=== ///
	std::unique_ptr<PlayerMoveComponent> moveComponent_;
	std::unique_ptr<PlayerAvoidanceComponent> avoidanceComponent_;
	std::unique_ptr<PlayerAttackComponent> attackComponent_;

	/// ===State=== ///
	std::unique_ptr<PlayerState> currentState_;

	/// ===無敵時間の情報=== ///
	struct InvincibleInfo {
		float time = 1.0f;   // 無敵時間
		float timer = 0.0f;  // 無敵タイマー
		bool isFlag = false; // 無敵フラグ
	};
	InvincibleInfo invincibleInfo_;

	/// ===StickState=== ///
	struct ControllerStickState {
		Vector2 leftStick = {0.0f, 0.0f};
		Vector2 rightStick = {0.0f, 0.0f};
	};
	ControllerStickState stickState_;

private:

	/// <summary>
	/// タイマーを進める関数
	/// </summary>
	void advanceTimer();
};