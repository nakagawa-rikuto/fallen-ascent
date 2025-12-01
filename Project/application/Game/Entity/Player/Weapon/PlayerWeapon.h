#pragma once
/// ===Include=== ///
// OBB
#include "Engine/Collider/OBBCollider.h"
// Particle
//#include "application/Game/Particle/AttackTrajectoryParticle.h"

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
	/// 親子関係を設定し、指定したオフセットで子オブジェクトの位置を調整
	/// </summary>
	/// <param name="parent">親となる Player オブジェクトへのポインタ。子をこの親に関連付けます。</param>
	void SetUpParent(Player* parent);

	/// <summary>
	/// 攻撃を開始する
	/// </summary>
	/// <param name="startPoint">攻撃開始位置（ワールド座標）</param>
	/// <param name="endPoint">攻撃終了位置（ワールド座標）</param>
	/// <param name="duration">攻撃にかける時間（秒）</param>
	/// <param name="startRotation">開始時の回転（デフォルト：単位クォータニオン）</param>
	/// <param name="endRotation">終了時の回転（デフォルト：単位クォータニオン）</param>
	void StartAttack(
		const Vector3& startPoint,
		const Vector3& endPoint,
		float duration,
		const Quaternion& startRotation = Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f },
		const Quaternion& endRotation = Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f }
	);

	/// <summary>
	/// チャージ攻撃を開始する
	/// </summary>
	/// <param name="startPoint">攻撃開始位置（ワールド座標）</param>
	/// <param name="endPoint">攻撃終了位置（ワールド座標）</param>
	/// <param name="duration">攻撃にかける時間（秒）</param>
	/// <param name="startRotation">開始時の回転（デフォルト：単位クォータニオン）</param>
	/// <param name="endRotation">終了時の回転（デフォルト：単位クォータニオン）</param>
	void StartChargeAttack(
		const Vector3& startPoint,
		const Vector3& endPoint,
		float duration,
		const Quaternion& startRotation = Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f },
		const Quaternion& endRotation = Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f }
	);

public: /// ===衝突=== ///
	/// <summary>
	/// 衝突処理
	/// </summary>
	/// <param name="collider">衝突した相手の Collider オブジェクトを指すポインター。</param>
	void OnCollision(Collider* collider) override;

public: /// ===Getter=== ///
	// 攻撃中かどうか
	bool GetIsAttack() const;
	// 攻撃の進行度を取得
	float GetAttackProgress() const;

public: /// ===Setter=== ///

	// アクティブ状態の設定
	void SetActive(bool flag);

private:
	// Player
	Player* player_ = nullptr;

	// Particle
	//std::shared_ptr<AttackTrajectoryParticle> particle_;

	/// ===基本情報=== ///
	struct BaseInfo {
		bool isActive = false;					 // アクティブフラグ
		Vector3 offset = { 0.0f, 0.0f, 0.0f };   // プレイヤーからのオフセット
		Vector3 velocity = { 0.0f, 0.0f, 0.0f }; // 速度
		float deltaTime;						 // デルタタイム
	};
	BaseInfo baseInfo_;

	/// ===攻撃情報=== ///
	struct AttackInfo {
		bool isAttacking = false;		// 攻撃中フラグ
		bool isChargeAttack = false;	// チャージ攻撃中フラグ
		bool hasHit = false;			// ヒット済みフラグ（多段ヒット防止）

		float timer = 0.0f;				// 攻撃タイマー
		float duration = 0.0f;			// 攻撃の持続時間
		float progress = 0.0f;			// 攻撃の進行度（0.0～1.0）

		Vector3 startPoint;				// 攻撃開始位置
		Vector3 endPoint;				// 攻撃終了位置
		Quaternion startRotation;		// 開始時の回転
		Quaternion endRotation;			// 終了時の回転

		// 扇形軌道のパラメータ
		Vector3 arcCenter;				// 円弧の中心位置
		float arcRadius = 0.0f;			// 円弧の半径
		float startAngle = 0.0f;		// 開始角度（ラジアン）
		float endAngle = 0.0f;			// 終了角度（ラジアン）
	};
	AttackInfo attackInfo_;

private:
	/// <summary>
	/// 扇形の軌道パラメータを計算
	/// </summary>
	void CalculateArcParameters();

	/// <summary>
	/// 攻撃軌道の更新処理
	/// </summary>
	void UpdateAttackTrajectory();

	/// <summary>
	/// チャージ攻撃軌道の更新処理
	/// </summary>
	void UpdateChargeAttackTrajectory();
};