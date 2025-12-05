#include "GroundOshan.h"
// Service
#include "Engine/System/Service/InputService.h"


///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
GroundOshan::~GroundOshan() {
	ocean_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void GroundOshan::Initialize() {

	AABBCollider::Initialize();
	aabb_.min = { -500.0f, -2.0f, -500.0f };
	aabb_.max = { 500.0f, 0.0f, 500.0f };

	// オーシャンの初期化
	ocean_ = std::make_unique<OceanFFT>();
	ocean_->Initialize(128, 256);
	ocean_->SetTranslate({ 0.0f, -1.0f, 0.0f });
	ocean_->SetScale({ 0.5f, 0.5f, 0.5f });

	ocean_->Update();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void GroundOshan::Update() {

#ifdef _DEBUG

	//if (InputService::TriggerKey(DIK_SPACE)) {
	//	// 波紋を追加
	//	Vector3 ripplePos = { 0.0f, 0.0f, 0.0f };
	//	ocean_->AddCircularRipple(ripplePos, 1.0f, 1.0f, 4.0f);
	//}

#endif // _DEBUG

	// オーシャンの更新
	ocean_->Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void GroundOshan::Draw(BlendMode mode) {

#ifdef _DEBUG
#endif // _DEBUG

	// オーシャンの描画
	ocean_->Draw(mode);
}

///-------------------------------------------/// 
/// ImGui情報の表示
///-------------------------------------------///
void GroundOshan::ShowImGui() {
#ifdef USE_IMGUI
	ocean_->ShowImGui();
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 衝突判定
///-------------------------------------------///
void GroundOshan::OnCollision(Collider* collider) {
	// オーシャンに波紋を追加
	/*Vector3 collisionPos = collider->GetTransform().translate;
	ocean_->AddCircularRipple(collisionPos, 2.0f, 1.0f, 4.0f);*/

	collider;
}
