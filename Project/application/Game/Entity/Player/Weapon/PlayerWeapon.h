#pragma once
/// ===Include=== ///
#include "Engine/Collider/OBBCollider.h"

/// ===前方宣言=== ///
class Player;

///=====================================================/// 
/// PlayerWeapon
///=====================================================///
class PlayerWeapon : public OBBCollider {
public:
	PlayerWeapon() = default;
	~PlayerWeapon() override;

	/// <summary>
	/// 初期化処理（GameScene）
	/// </summary>
	/// <param name="player">初期化する Player オブジェクトへのポインタ。</param>
	void InitPlayer(Player* player);

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="mode">描画に使用するブレンドモード。</param>
	void Draw(BlendMode mode) override;

	/// <summary>
	/// ImGui情報の表示
	/// </summary>
	void Information() override;

	/// <summary>
	/// 攻撃処理
	/// </summary>
	/// <param name="startPoint">攻撃の開始位置を表す3次元ベクトル（例: ワールド空間の座標）。</param>
	/// <param name="endPoint">攻撃の終了位置を表す3次元ベクトル（例: ワールド空間の座標）。</param>
	/// <param name="time">攻撃にかける時間（秒）。float 型で指定します。</param>
	void Attack(const Vector3& startPoint, const Vector3& endPoint, float time);

public: /// ===衝突=== ///
	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="collider">衝突した相手の Collider オブジェクトを指すポインター。</param>
	void OnCollision(Collider* collider) override;

public: /// ===Getter=== ///
	// 攻撃フラグ
	bool GetIsAttack() const;

private:
	// Player
	Player* player_ = nullptr;

	/// ===基本情報=== ///
	struct BaseInfo {
		Vector3 offset_ = { 0.0f, 0.0f, 0.0f }; // プレイヤーからのオフセット
		Vector3 velocity_ = { 0.0f, 0.0f, 0.0f }; // 速度
	};
	BaseInfo baseInfo_;

	/// ===攻撃情報=== ///
	struct AttackInfo {
		bool isAttack = false;
	};

private: 

	/// <summary>
	/// 攻撃の回転を計算する処理
	/// </summary>
	/// <param name="startPoint">攻撃の開始位置を表す3Dベクトル（const参照）。</param>
	/// <param name="endPoint">攻撃の目標または終了位置を表す3Dベクトル（const参照）。</param>
	/// <param name="time">回転を計算するための時間。経過時間（秒）や正規化された係数など、文脈に依存する。</param>
	void CalculateAttackRotation(const Vector3& startPoint, const Vector3& endPoint, float time);
};

