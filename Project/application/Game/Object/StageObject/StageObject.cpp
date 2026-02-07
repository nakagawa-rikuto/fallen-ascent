#include "StageObject.h"
// Service
#include "Engine/System/Service/ColliderService.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
StageObject::~StageObject() {
	object3d_.reset();
}

///-------------------------------------------/// 
/// 初期化処理（呼び出し用）
///-------------------------------------------///
void StageObject::GameInit(const std::string& modelName) {
	// Object3dの生成
	object3d_ = std::make_unique<Object3d>();
	// 初期化
	object3d_->Init(ObjectType::Model, modelName);
	// 初期化処理
	GameObject::Initialize();
	name_ = MiiEngine::ColliderName::Object; // コライダー名を設定
	// コライダーサービスに登録
	Service::ColliderService::AddCollider(this);
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void StageObject::Initialize() {
	// GameObjectの初期化
	GameObject::Initialize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void StageObject::Update() {
	// GameObjectの更新
	GameObject::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void StageObject::Draw(MiiEngine::BlendMode mode) {
	// GameObjectの描画
	GameObject::Draw(mode);
}

///-------------------------------------------/// 
/// 衝突処理
///-------------------------------------------///
void StageObject::OnCollision(MiiEngine::Collider* collider) {
	collider;
}

