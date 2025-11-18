#include "Ground.h"
// Service
#include "Engine/System/Service/ColliderService.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Ground::~Ground() {
	object3d_.reset();
}

///-------------------------------------------/// 
/// 初期化処理（呼び出し用）
///-------------------------------------------///
void Ground::GameInit(const std::string& modelName) {
	// Object3dの生成
	object3d_ = std::make_unique<Object3d>();
	// 初期化
	object3d_->Init(ObjectType::Model, modelName);
	// 初期化処理
	AABBCollider::Initialize();
	name_ = ColliderName::Ground; // コライダー名を設定
	// コライダーサービスに登録
	ColliderService::AddCollider(this);
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Ground::Initialize() {
	// OBBColliderの初期化
	AABBCollider::Initialize();
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