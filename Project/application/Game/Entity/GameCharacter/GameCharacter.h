#pragma once
/// ===Include=== ///
// Collider
#include "Engine/Collider/OBBCollider.h"
#include "Engine/Collider/SphereCollider.h"
#include "Engine/Collider/ColliderConcept.h"
// GameCharacterCollision
#include "GameCharacterCollision.h"

///-------------------------------------------/// 
/// GameCharacterBase
///-------------------------------------------///
template<typename TCollider> requires IsCollider<TCollider>
class GameCharacter : public TCollider {
public:

    GameCharacter();
    virtual ~GameCharacter();

    /// <summary>
    /// 初期化処理
    /// </summary>
    virtual void Initialize() override;

    /// <summary>
    /// 更新処理
    /// </summary>
    virtual void Update() override;

    /// <summary>
    /// 描画処理
    /// </summary>
    /// <param name="mode">描画に使用するブレンドモード。</param>
    virtual void Draw(BlendMode mode) override;

    /// <summary>
    /// ImGui情報の表示
    /// </summary>
    virtual void Information() override;

    /// <summary>
    /// 更新処理の前処理
    /// </summary>
    void PreUpdate();
   

public: /// ===衝突判定=== ///
    /// <summary>
    /// 衝突時の処理
    /// </summary>
    /// <param name="collider">衝突した相手を表す Collider へのポインター。</param>
    virtual void OnCollision(Collider* collider) override;

public: /// ===Getter=== ///
    // Velocity
    Vector3 GetVelocity() const { return baseInfo_.velocity; }
    // isDead
    bool GetIsDead() const { return baseInfo_.isDead; };
    // deltaTime
    float GetDeltaTime() const { return baseInfo_.deltaTime; };
    // HP
    uint32_t GetHP() const { return baseInfo_.HP; }
   
public: /// ===Setter=== ///
    // Velocity
    void SetVelocity(const Vector3 vel) { baseInfo_.velocity = vel; };
    // isDead
    void SetIsDead(const bool isDead) { baseInfo_.isDead = isDead; };
protected:
    // Collisionクラス
    std::unique_ptr<ColliderCollision> collision_;
    std::unique_ptr<GameCharacterCollision> gCollision_;

    /// ===基本情報=== ///
    struct BaseInfo {
        Vector3 velocity{};     // 速度
        float deltaTime = 0.0f; // 時間
		float gravity = -9.8f;  // 重力
		uint32_t HP = 10;       // 体力
        bool isDead = false;    // 死亡フラグ
    };
    BaseInfo baseInfo_;

private: /// ===地面との衝突処理=== ///

    /// ===GroundInfo=== ///
    struct GroundInfo {
		ColliderType currentGroundType;    // 現在の地面のコライダータイプ
        Vector3 currentGroundFirst{};      // 現在の地面の情報1
		Vector3 currentGroundSecond{};     // 現在の地面の情報2
        float currentGroundYPos = 0.0f;    // 現在の地面のY軸
        bool isGrounded = false;		   // 地面接地フラグ
    };
	GroundInfo groundInfo_;

    // キャラクターの半サイズY軸
    Vector3 characterHalfSize_{};

    /// <summary>
    /// 指定した中心位置と半サイズを持つオブジェクトの地面との衝突を検出して処理します。
    /// </summary>
    void GroundCollision();
  
    /// <summary>
    ///  地面に衝突した際の処理
    /// </summary>
    /// <param name="collider">衝突したコライダーへのポインタ。</param>
    void GroundOnCollision(Collider* collider);

};

