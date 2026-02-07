#include "PlayerHand.h"
// Player
#include "application/Game/Entity/Player/Player.h"
// Math
#include "Math/sMath.h"
#include "Math/EasingMath.h"
// Service
#include "Service/DeltaTime.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
PlayerHand::~PlayerHand() {
	object3d_.reset();
}

///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void PlayerHand::Initialize() {
	// モデルの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "PlayerHand");

	/// ===OBBCollider=== ///
	OBBCollider::Initialize();
	name_ = MiiEngine::ColliderName::None;
	OBBCollider::SetHalfSize({ 0.5f, 0.5f, 3.0f });

	attackInfo_.isAttacking = false;

	// DeltaTime初期化
	baseInfo_.deltaTime = Service::DeltaTime::GetDeltaTime();

	// 初回更新
	Update();
}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void PlayerHand::Update() {
	// DeltaTime更新
	baseInfo_.deltaTime = Service::DeltaTime::GetDeltaTime();

	// 攻撃中でない場合は早期リターン
	if (!attackInfo_.isAttacking) {
		OBBCollider::Update();
		return;
	}

	// タイマーを進める
	attackInfo_.timer += baseInfo_.deltaTime;
	attackInfo_.progress = attackInfo_.timer / attackInfo_.duration;

	// 攻撃終了チェック
	if (attackInfo_.progress >= 1.0f) {
		attackInfo_.isAttacking = false;
		attackInfo_.progress = 1.0f;
		OBBCollider::Update();
		return;
	}

	// ベジェ曲線に沿った攻撃軌道の更新
	UpdateBezierTrajectory();

	/// ===OBBCollider=== ///
	OBBCollider::Update();
}

///-------------------------------------------/// 
/// 描画処理
///-------------------------------------------///
void PlayerHand::Draw(MiiEngine::BlendMode mode) {
	// 攻撃中のみ描画
	if (attackInfo_.isAttacking) {
		OBBCollider::Draw(mode);
	}
}

///-------------------------------------------/// 
/// ImGui情報の表示
///-------------------------------------------///
void PlayerHand::Information() {}

///-------------------------------------------/// 
/// 衝突処理
///-------------------------------------------///
void PlayerHand::OnCollision(Collider* collider) {
	collider;
}

///-------------------------------------------/// 
/// 親子関係を設定し、指定したオフセットで子オブジェクトの位置を調整
///-------------------------------------------///
void PlayerHand::SetUpParent(Player* parent) {
	player_ = parent;
	// 親子関係の設定
	object3d_->SetParent(player_->GetModelCommon());
}

///-------------------------------------------/// 
/// 攻撃の開始処理（ベジェ曲線）
///-------------------------------------------///
void PlayerHand::StartAttack(
	const std::vector<MiiEngine::BezierControlPointData>& trajectoryPoints,
	float duration) {

	// 制御点が2点未満の場合は処理を行わない
	if (trajectoryPoints.size() < 2) {
		return;
	}

	// 攻撃情報の設定
	attackInfo_.isAttacking = true;
	attackInfo_.timer = 0.0f;
	attackInfo_.duration = duration;
	attackInfo_.progress = 0.0f;

	// ベジェ曲線の制御点を保存
	attackInfo_.trajectoryPoints = trajectoryPoints;

	// 初期位置と回転を設定
	transform_.translate = trajectoryPoints.front().position;
	transform_.rotate = trajectoryPoints.front().rotation;
}

///-------------------------------------------/// 
/// ベジェ曲線上の位置を計算
///-------------------------------------------///
Vector3 PlayerHand::CalculateBezierPoint(const std::vector<MiiEngine::BezierControlPointData>& controlPoints, float t) {
	size_t n = controlPoints.size();

	// 制御点が2点の場合は線形補間
	if (n == 2) {
		return Math::Lerp(controlPoints[0].position, controlPoints[1].position, t);
	}

	// 制御点が3点の場合は2次ベジェ曲線
	if (n == 3) {
		float u = 1.0f - t;
		float uu = u * u;
		float tt = t * t;

		Vector3 point = {};
		point.x = uu * controlPoints[0].position.x +
			2.0f * u * t * controlPoints[1].position.x +
			tt * controlPoints[2].position.x;
		point.y = uu * controlPoints[0].position.y +
			2.0f * u * t * controlPoints[1].position.y +
			tt * controlPoints[2].position.y;
		point.z = uu * controlPoints[0].position.z +
			2.0f * u * t * controlPoints[1].position.z +
			tt * controlPoints[2].position.z;

		return point;
	}

	// 制御点が4点の場合は3次ベジェ曲線
	if (n == 4) {
		float u = 1.0f - t;
		float uu = u * u;
		float uuu = uu * u;
		float tt = t * t;
		float ttt = tt * t;

		Vector3 point = {};
		point.x = uuu * controlPoints[0].position.x +
			3.0f * uu * t * controlPoints[1].position.x +
			3.0f * u * tt * controlPoints[2].position.x +
			ttt * controlPoints[3].position.x;
		point.y = uuu * controlPoints[0].position.y +
			3.0f * uu * t * controlPoints[1].position.y +
			3.0f * u * tt * controlPoints[2].position.y +
			ttt * controlPoints[3].position.y;
		point.z = uuu * controlPoints[0].position.z +
			3.0f * uu * t * controlPoints[1].position.z +
			3.0f * u * tt * controlPoints[2].position.z +
			ttt * controlPoints[3].position.z;

		return point;
	}

	// 5点以上の場合はDe Casteljauのアルゴリズム
	std::vector<Vector3> temp;
	for (const auto& cp : controlPoints) {
		temp.push_back(cp.position);
	}

	while (temp.size() > 1) {
		std::vector<Vector3> newTemp;
		for (size_t i = 0; i < temp.size() - 1; ++i) {
			newTemp.push_back(Math::Lerp(temp[i], temp[i + 1], t));
		}
		temp = newTemp;
	}

	return temp[0];
}

///-------------------------------------------/// 
/// ベジェ曲線上の回転を計算
///-------------------------------------------///
Quaternion PlayerHand::CalculateBezierRotation(const std::vector<MiiEngine::BezierControlPointData>& controlPoints, float t) {
	size_t n = controlPoints.size();
	if (n == 0) {
		// デフォルト quaternion（必要に応じて変更）
		return Quaternion{ 0, 0, 0, 1 };
	}
	if (n == 1) {
		return controlPoints[0].rotation;
	}

	// 2点の場合→SLerp
	if (n == 2) {
		return Math::SLerp(controlPoints[0].rotation, controlPoints[1].rotation, t);
	}

	// 3点以上→De Casteljau法   (SLerpで再帰的に作る)
	std::vector<Quaternion> temp;
	for (const auto& cp : controlPoints) {
		temp.push_back(cp.rotation);
	}

	// 再帰的補間
	while (temp.size() > 1) {
		std::vector<Quaternion> nextLayer;
		for (size_t i = 0; i < temp.size() - 1; ++i) {
			nextLayer.push_back(Math::SLerp(temp[i], temp[i + 1], t));
		}
		temp = nextLayer;
	}
	return temp[0];
}

///-------------------------------------------/// 
/// ベジェ曲線に沿った軌道更新
///-------------------------------------------///
void PlayerHand::UpdateBezierTrajectory() {
	// イージング関数を適用（滑らかな加速・減速）
	float t = attackInfo_.progress;
	float easedT = Easing::EaseInOutQuad(t);

	// ベジェ曲線上の位置を計算
	transform_.translate = CalculateBezierPoint(attackInfo_.trajectoryPoints, easedT);
	// ベジェ曲線上の回転を計算
	transform_.rotate = CalculateBezierRotation(attackInfo_.trajectoryPoints, easedT);
}