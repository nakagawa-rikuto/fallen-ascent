#include "Ground.h"
// Service
#include "Service/Collision.h"

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
	GameObject::Initialize();
	name_ = MiiEngine::ColliderName::Ground; // コライダー名を設定
	// コライダーサービスに登録
	Service::Collision::AddCollider(this);
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Ground::Initialize() {
	// GameObjectの初期化
	GameObject::Initialize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Ground::Update() {
	// GameObjectの更新
	GameObject::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Ground::Draw(MiiEngine::BlendMode mode) {
	// GameObjectの描画
	GameObject::Draw(mode);
}

///-------------------------------------------/// 
/// 衝突判定
///-------------------------------------------///
void Ground::OnCollision(MiiEngine::Collider* collider) {
	collider;
}