#include "StageObject.h"

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
StageObject::~StageObject() {
	object3d_.reset();
}

///-------------------------------------------/// 
/// 初期化処理（呼び出し用）
///-------------------------------------------///
void StageObject::GameInit(const std::string& modelName, const Vector3& halfSize) {
	// Object3dの生成
	object3d_ = std::make_unique<Object3d>();
	// 初期化
	object3d_->Init(ObjectType::Model, modelName);
	// 初期化処理
	Initialize();
	// OBBの半分のサイズを設定
	obb_.halfSize = halfSize;
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void StageObject::Initialize() {
	// OBBColliderの初期化
	OBBCollider::Initialize();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void StageObject::Update() {
	// OBBColliderの更新
	OBBCollider::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void StageObject::Draw(BlendMode mode) {
	// OBBColliderの描画
	OBBCollider::Draw(mode);
}

///-------------------------------------------/// 
/// 衝突処理
///-------------------------------------------///
void StageObject::OnCollision(Collider* collider) {
	collider;
}

