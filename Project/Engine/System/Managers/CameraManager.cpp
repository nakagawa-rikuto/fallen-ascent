#include "CameraManager.h"
// Camera
#include "Engine/Camera/NormalCamera.h"
#include "Engine/Camera/FollowCamera.h"
// C++
#include <cassert>

namespace MiiEngine {
	///-------------------------------------------/// 
	/// コンストラクタ、デストラクタ
	///-------------------------------------------///
	CameraManager::CameraManager() = default;
	CameraManager::~CameraManager() {
		activeCamera_ = nullptr;
		cameras_.clear();
	}

	///-------------------------------------------/// 
	/// 全てのカメラの更新
	///-------------------------------------------///
	void CameraManager::UpdateAllCameras() {
		// 更新をする
		for (auto& [name, camera] : cameras_) {
			camera->Update();
		}
	}

	///-------------------------------------------/// 
	/// カメラを追加
	///-------------------------------------------///
	void CameraManager::AddCamera(const std::string& name, CameraCommon* camera) {
		cameras_[name] = camera;

		if (!activeCamera_) {
			activeCamera_ = cameras_[name]; // 最初に追加されたカメラをデフォルトでアクティブにする
		}
	}

	///-------------------------------------------/// 
	/// カメラを削除
	///-------------------------------------------///
	void CameraManager::RemoveCamera(const std::string& name) {
		auto it = cameras_.find(name);
		if (it != cameras_.end()) {
			// アクティブカメラが削除されるカメラの場合、activeCamera_をnullptrにする
			if (activeCamera_ == it->second) {
				activeCamera_ = nullptr;
			}
			cameras_.erase(it);
		}
	}

	///-------------------------------------------/// 
	/// カメラが存在するかチェック
	///-------------------------------------------///
	bool CameraManager::HasCamera(const std::string& name) const {
		return cameras_.find(name) != cameras_.end();
	}

	///-------------------------------------------/// 
	/// Setter
	///-------------------------------------------///
	// アクティブカメラのSetter
	void CameraManager::SetActiveCamera(const std::string& name) {
		auto it = cameras_.find(name);
		if (it != cameras_.end()) {
			activeCamera_ = it->second;
		} else {
			assert(false && "指定されたカメラが存在しません");
		}
	}

	///-------------------------------------------/// 
	/// Getter
	///-------------------------------------------///
	// 指定されたカメラのGetter
	CameraCommon* CameraManager::GetCamera(const std::string& name) const {
		auto it = cameras_.find(name);
		if (it != cameras_.end()) {
			return it->second;
		}
		return nullptr;
	}
	
	// アクティブカメラのGetter
	CameraCommon* CameraManager::GetActiveCamera() const {
		return activeCamera_;
	}
}