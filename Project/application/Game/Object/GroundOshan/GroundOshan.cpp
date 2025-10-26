#include "GroundOshan.h"
// Service
#include "Engine/System/Service/InputService.h"


///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GroundOshan::~GroundOshan() {
	oshan_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GroundOshan::Initialize() {

	AABBCollider::Initialize();
	aabb_.min = { -500.0f, -2.0f, -500.0f };
	aabb_.max = { 500.0f, 0.0f, 500.0f };


	// オーシャンの初期化
	oshan_ = std::make_unique<Oshan>();
	oshan_->Initialize(150);
	oshan_->SetTranslate({ 0.0f, 0.0f, 0.0f });

	oshan_->Update();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GroundOshan::Update() {

#ifdef _DEBUG

	if (InputService::TriggerKey(DIK_SPACE)) {
		// 波紋を追加
		Vector3 ripplePos = { 0.0f, 0.0f, 0.0f };
		oshan_->AddRipple(ripplePos, 1.0f);
	}

#endif // _DEBUG

	// オーシャンの更新
	oshan_->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GroundOshan::Draw(BlendMode mode) {

#ifdef _DEBUG
#endif // _DEBUG

	// オーシャンの描画
	oshan_->Draw(mode);
}

///-------------------------------------------/// 
/// ImGui情報の表示
///-------------------------------------------///
void GroundOshan::ShowImGui() {
#ifdef USE_IMGUI
	oshan_->ShowImGui();
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 衝突判定
///-------------------------------------------///
void GroundOshan::OnCollision(Collider* collider) {
	// オーシャンに波紋を追加
	Vector3 collisionPos = collider->GetTransform().translate;
	oshan_->OnCollision(collisionPos, 0.0f, 1.0f);
}
