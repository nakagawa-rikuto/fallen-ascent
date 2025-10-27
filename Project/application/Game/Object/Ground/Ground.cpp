#include "Ground.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Ground::~Ground() {
	object3d_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Ground::Initialize() {
	AABBCollider::Initialize();
	//aabb_.min = {};
	//aabb_.max = {};

	// object3dの初期化
	object3d_ = std::make_unique<Object3d>();
	object3d_->Init(ObjectType::Model, "terrain");
	transform_.translate = { 0.0f, -5.0f, 0.0f };
	transform_.scale = { 5.0f, 5.0f, 5.0f };
	object3d_->Update();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Ground::Update() {
	// 更新
	AABBCollider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Ground::Draw(BlendMode mode) {
	// 描画
	AABBCollider::Draw(mode);
}

///-------------------------------------------/// 
/// 衝突判定
///-------------------------------------------///
void Ground::OnCollision(Collider* collider) {
	collider;
}