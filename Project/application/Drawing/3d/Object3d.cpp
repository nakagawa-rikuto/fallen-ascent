#include "Object3d.h"
// C++
#include <cassert>

using namespace MiiEngine;
///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Object3d::~Object3d() {
	if (type_ == ObjectType::Model) {
		model_.reset();
	} else {
		animationModel_.reset();
	}
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Object3d::Init(ObjectType type, const std::string& modelName, MiiEngine::LightType light) {
	// タイプの確認
	type_ = type;
	// それぞれ対応した方を生成
	if (type_ == ObjectType::Model) {
		model_ = std::make_unique<Model>();
		model_->Initialize(modelName, light);
	} else {
		animationModel_ = std::make_unique<AnimationModel>();
		animationModel_->Initialize(modelName, light);
	}
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Object3d::Update() {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		model_->Update();
	} else {
		animationModel_->Update();
	}
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Object3d::Draw(MiiEngine::BlendMode mode) {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		model_->Draw(mode);
	} else {
		animationModel_->Draw(mode);
	}
}

///-------------------------------------------/// 
/// 親子関係
///-------------------------------------------///
// SetParent
void Object3d::SetParent(MiiEngine::ModelCommon* parent) {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		model_->SetParent(parent);
	} else {
		animationModel_->SetParent(parent);
	}
}
// ClearParent
void Object3d::ClearParent() {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		model_->ClearParent();
	} else {
		animationModel_->ClearParent();
	}
}
// Offset
void Object3d::SetParentOffset(const Vector3& offset) {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		model_->SetParentOffset(offset);
	} else {
		animationModel_->SetParentOffset(offset);
	}
} 
const Vector3& Object3d::GetParentOffset() const {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		return model_->GetParentOffset();
	} else {
		return animationModel_->GetParentOffset();
	}
}


///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// Translate
const Vector3& Object3d::GetWorldTranslate() const {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		return model_->GetWorldTranslate();
	} else {
		return animationModel_->GetWorldTranslate();
	}
}
// Rotate
const Quaternion& Object3d::GetWorldRotate() const {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		return model_->GetWorldRotate();
	} else {
		return animationModel_->GetWorldRotate();
	}
}
// Scale
const Vector3& Object3d::GetWorldScale() const {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		return model_->GetWorldScale();
	} else {
		return animationModel_->GetWorldScale();
	}
}
// Transform
const MiiEngine::QuaternionTransform& Object3d::GetWorldTransform() const {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		return model_->GetWorldTransform();
	} else {
		return animationModel_->GetWorldTransform();
	}
}
// Color
const Vector4& Object3d::GetColor() const {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		return model_->GetColor();
	} else {
		return animationModel_->GetColor();
	}
}
// ModelCommon
MiiEngine::ModelCommon* Object3d::GetModelCommon() {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		return model_.get();
	} else {
		return animationModel_.get();
	}
}


///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// Translate
void Object3d::SetTranslate(const Vector3& translate) {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		model_->SetTranslate(translate);
	} else {
		animationModel_->SetTranslate(translate);
	}
}
// Rotate
void Object3d::SetRotate(const Quaternion& rotate) {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		model_->SetRotate(rotate);
	} else {
		animationModel_->SetRotate(rotate);
	}
}
// Scale
void Object3d::SetScale(const Vector3& scale) {
	//	タイプで分岐
	if (type_ == ObjectType::Model) {
		model_->SetScale(scale);
	} else {
		animationModel_->SetScale(scale);
	}
}
// Color
void Object3d::SetColor(const Vector4& color) {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		model_->SetColor(color);
	} else {
		animationModel_->SetColor(color);
	}
}
// LightInfo
void Object3d::SetLightData(MiiEngine::LightInfo light) {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		model_->SetLightData(light);
	} else {
		animationModel_->SetLightData(light);
	}
}
// Light
void Object3d::SetLight(MiiEngine::LightType type) {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		model_->SetLightType(type);
	} else {
		animationModel_->SetLightType(type);
	}
}
// 環境マップ
void Object3d::SetEnvironmentMapData(bool flag, float string) {
	// タイプで分岐
	if (type_ == ObjectType::Model) {
		model_->SetEnvironmentMapData(flag, string);
	} else {
		animationModel_->SetEnvironmentMapData(flag, string);
	}
}
// Animation
void Object3d::SetAnimation(const std::string& animationName, bool isLoop) {
	// タイプで分岐
	if (type_ == ObjectType::AnimationModel) {
		animationModel_->SetAnimation(animationName, isLoop);
	} else {
		// タイプがアニメーションモデル出なければ強制的に止める
		assert(0);
	}
}
