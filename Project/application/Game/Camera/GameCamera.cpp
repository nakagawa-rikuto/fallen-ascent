#include "GameCamera.h"
#include <cassert>

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GameCamera::~GameCamera() {
	camera_.reset();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// WorldMatrix
const Matrix4x4& GameCamera::GetWorldMatrix() const {
	return camera_->GetWorldMatrix();
}
// ViewMatrix
const Matrix4x4& GameCamera::GetViewMatrix() const {
	return camera_->GetViewMatrix();
}
// ProjectionMatrix
const Matrix4x4& GameCamera::GetProjectionMatrix() const {
	return camera_->GetProjectionMatrix();
}
// ViewProjectionMatrix
const Matrix4x4& GameCamera::GetViewProjectionMatrix() const {
	return camera_->GetViewProjectionMatrix();
}
// Translate
const Vector3& GameCamera::GetTranslate() const {
	return camera_->GetTranslate();
}
// Rotate
const Quaternion& GameCamera::GetRotate() const {
	return camera_->GetRotate();
}
// Offset
const Vector3& GameCamera::GetOffset() const {
	auto follow = std::dynamic_pointer_cast<MiiEngine::FollowCamera>(camera_);
	assert(!follow);
	return follow->GetOffset();
}
// OrbitingOffset
const Vector3& GameCamera::GetOrbitingOffset() const {
	auto follow = std::dynamic_pointer_cast<MiiEngine::FollowCamera>(camera_);
	assert(!follow);
	return follow->GetOrbitingOffset();
}


///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// Translate
void GameCamera::SetTranslate(const Vector3& translate) {
	camera_->SetTranslate(translate);
}
// Rotate
void GameCamera::SetRotate(const Quaternion& rotate) {
	camera_->SetRotate(rotate);
}
// ForY
void GameCamera::SetForY(const float& forY) {
	camera_->SetForY(forY);
}
// AspectRatio
void GameCamera::SetAspectRatio(const float& aspect) {
	camera_->SetAspectRatio(aspect);
}
// NearClip
void GameCamera::SetNearClip(const float& nearClip) {
	camera_->SetNearClip(nearClip);
}
// FarClip
void GameCamera::SetFarClip(const float& farClip) {
	camera_->SetFarClip(farClip);
}
// FollowCameraの設定
void GameCamera::SetFollowCamera(FollowCameraType type) {
	if (auto follow = std::dynamic_pointer_cast<MiiEngine::FollowCamera>(camera_)) {
		follow->SetFollowCamera(type);
	}
	return;
}
// 追従対象の座標を設定
void GameCamera::SetTarget(Vector3* position, Quaternion* rotation) {
	if (auto follow = std::dynamic_pointer_cast<MiiEngine::FollowCamera>(camera_)) {
		follow->SetTarget(position, rotation);
	}
	return;
}
// 追従のオフセット
void GameCamera::SetOffset(const Vector3& offset) {
	if (auto follow = std::dynamic_pointer_cast<MiiEngine::FollowCamera>(camera_)) {
		follow->SetOffset(offset);
	}
	return;
}
void GameCamera::SetOrbitingOffset(const Vector3& offset) {
	if (auto follow = std::dynamic_pointer_cast<MiiEngine::FollowCamera>(camera_)) {
		follow->SetOrbitingOffset(offset);
	}
	return;
}
// 追従速度を設定
void GameCamera::SetFollowSpeed(float speed) {
	if (auto follow = std::dynamic_pointer_cast<MiiEngine::FollowCamera>(camera_)) {
		follow->SetFollowSpeed(speed);
	}
	return;
}
// 回転補間速度
void GameCamera::SetLerpSpeed(float speed) {
	if (auto follow = std::dynamic_pointer_cast<MiiEngine::FollowCamera>(camera_)) {
		follow->SetLerpSpeed(speed);
	}
	return;
}
// 回転の重み
void GameCamera::SetStick(const Vector2& stickValue) {
	if (auto follow = std::dynamic_pointer_cast<MiiEngine::FollowCamera>(camera_)) {
		follow->SetStick(stickValue);
	}
	return;
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GameCamera::Init(CameraType type) {
	type_ = type;

	if (type_ == CameraType::Normal) {
		camera_ = std::make_shared<MiiEngine::NormalCamera>();
		camera_->Initialize();
	} else {
		camera_ = std::make_shared<MiiEngine::FollowCamera>();
		camera_->Initialize();
	}
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GameCamera::Update() {
	camera_->Update();
}

///-------------------------------------------/// 
/// 情報
///-------------------------------------------///
void GameCamera::ImGuiUpdate() {
#ifdef USE_IMGUI
	camera_->ImGuiUpdate();
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// デバッグ用の更新
///-------------------------------------------///
void GameCamera::DebugUpdate() {
#ifdef _DEBUG
	camera_->DebugUpdate();
#endif // _DEBUG
}

///-------------------------------------------/// 
/// シェイク開始処理
///-------------------------------------------///
void GameCamera::StartShake(float intensity, float duration, float frequency) {
	camera_->StartShake(intensity, duration, frequency);
}

///-------------------------------------------/// 
/// シェイク処理終了
///-------------------------------------------///
void GameCamera::StopShake() {
	camera_->StopShake();
}