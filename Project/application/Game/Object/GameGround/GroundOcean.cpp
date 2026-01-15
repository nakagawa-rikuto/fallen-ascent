#include "GroundOcean.h"
// Service
#include "Engine/System/Service/InputService.h"


///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GroundOcean::~GroundOcean() {
	ocean_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GroundOcean::Initialize() {

	// AABBコライダーの初期化と設定
	AABBCollider::Initialize();
	AABBCollider::SetAABB({ { -500.0f, -2.0f, -500.0f }, { 500.0f, 0.0f, 500.0f } });

	// オーシャンの初期化
	ocean_ = std::make_unique<Ocean>();
	ocean_->Initialize(500);
	ocean_->SetTranslate({ 0.0f, -1.0f, 0.0f });
	ocean_->SetScale({ 0.5f, 0.5f, 0.5f });

	ocean_->Update();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GroundOcean::Update() {

#ifdef _DEBUG

	if (InputService::TriggerKey(DIK_SPACE)) {
		// 波紋を追加
		Vector3 ripplePos = { 0.0f, 0.0f, 0.0f };
		ocean_->AddCircularRipple(ripplePos, 1.0f, 1.0f, 4.0f);
	}

#endif // _DEBUG

	// オーシャンの更新
	ocean_->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GroundOcean::Draw(BlendMode mode) {

#ifdef _DEBUG
#endif // _DEBUG

	// オーシャンの描画
	ocean_->Draw(mode);
}

///-------------------------------------------/// 
/// ImGui情報の表示
///-------------------------------------------///
void GroundOcean::ShowImGui() {
#ifdef USE_IMGUI
	ocean_->ShowImGui();
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 衝突判定
///-------------------------------------------///
void GroundOcean::OnCollision(Collider* collider) {
	// オーシャンに波紋を追加
	Vector3 collisionPos = collider->GetTransform().translate;
	ocean_->AddCircularRipple(collisionPos, 2.0f, 1.0f, 4.0f);
}
