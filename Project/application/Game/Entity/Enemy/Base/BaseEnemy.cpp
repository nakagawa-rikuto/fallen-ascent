#include "BaseEnemy.h"
// Camera
#include "application/Game/Camera/GameCamera.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// Service
#include "Engine/System/Service/CameraService.h"
#include "Engine/System/Service/ColliderService.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"


///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
BaseEnemy::~BaseEnemy() {
	ColliderService::RemoveCollider(this);
	object3d_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void BaseEnemy::Initialize() {
	// カメラの取得
	camera_ = CameraService::GetActiveCamera().get();

	/// ===GameCharacter=== ///
	GameCharacter::Initialize();
	name_ = ColliderName::Enemy;

	// ColliderServiceに登録
	ColliderService::AddCollider(this);
}

///-------------------------------------------/// 
/// アニメーション時の更新処理
///-------------------------------------------///
void BaseEnemy::UpdateAnimation() {
	// GameCharacterの更新
	GameCharacter::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void BaseEnemy::Draw(BlendMode mode) {
	/// ===GameCharacterの描画=== ///
	GameCharacter::Draw(mode);
}

///-------------------------------------------/// 
/// 当たり判定
///-------------------------------------------///
void BaseEnemy::OnCollision(Collider* collider) {
	// 攻撃用のフラグを立てる
	isCollision_ = true;

	/// ===GameCharacterの衝突=== ///
	GameCharacter::OnCollision(collider);

	// 早期リターン
	if (baseInfo_.isDead) {
		return;
	}
}

///-------------------------------------------/// 
/// 回転更新関数
///-------------------------------------------///
void BaseEnemy::UpdateRotationTowards(const Vector3& direction, float slerpT) {
	// 方向ベクトルのY成分を0にして水平方向のみを考慮
	Vector3 horizontalDir = { direction.x, 0.0f, direction.z };

	// 値が小さすぎる場合は何もしない
	if (Length(horizontalDir) < 0.01f) return;

	// forward方向からターゲットクォータニオンを作成(Y軸回転のみ)
	Quaternion targetRotation = Math::LookRotation(horizontalDir, Vector3(0.0f, 1.0f, 0.0f));

	// Quaternionの内積で類似度を計算
	float dotProduct = Dot(transform_.rotate, targetRotation);
	float absDot = std::abs(dotProduct);

	// 0.9999以上なら十分近い
	if (absDot > 0.99f) {
		isRotationComplete_ = true;
		return; // 既に向いているので処理終了
	}
	// 回転が完了していない場合はフラグをリセット
	isRotationComplete_ = false;

	// SLerp補間
	Quaternion result = Math::SLerp(transform_.rotate, targetRotation, slerpT);
	transform_.rotate = Normalize(result); // 正規化でスケール崩れ防止
}