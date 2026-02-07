#include "IScene.h"
// Service
#include "Engine/System/Service/ServiceLocator.h"
#include "Engine/System/Service/CameraService.h"
// Line
#include "Engine/Graphics/3d/Line/LineObject3D.h"

namespace MiiEngine {
	///-------------------------------------------/// 
	/// デストラクタ
	///-------------------------------------------///
	IScene::~IScene() {
		defaultCamera_.reset();
	}

	///-------------------------------------------/// 
	/// Setter
	///-------------------------------------------///
	void IScene::SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

	///-------------------------------------------/// 
	/// 初期化
	///-------------------------------------------///
	void IScene::Initialize() {

		/// ===デフォルトカメラの生成=== ///
		defaultCamera_ = std::make_shared<GameCamera>();
		defaultCamera_->Init(CameraType::Normal);
		defaultCamera_->SetTranslate({ 0.0f, 0.0f, -50.0f });
		defaultCamera_->SetRotate({ 0.0f, 0.0f, 0.0f });

		// デフォルトカメラの設定
		Service::CameraService::AddCamera("Default", defaultCamera_);
		Service::CameraService::SetActiveCamera("Default");
	}

	///-------------------------------------------/// 
	/// 更新
	///-------------------------------------------///
	void IScene::Update() {

		// Line更新
		Service::ServiceLocator::GetLineObject3D()->Update();
	}

	///-------------------------------------------/// 
	/// 描画
	///-------------------------------------------///
	void IScene::Draw() {

		// Lineの描画
		Service::ServiceLocator::GetLineObject3D()->Draw();
	}
}


