#include "NormalCamera.h"

namespace MiiEngine {
	///-------------------------------------------/// 
	/// 初期化
	///-------------------------------------------///
	void NormalCamera::Initialize() {
		// 基底クラスの初期化を呼ぶ
		CameraCommon::Initialize();
		// CameraType
		cameraType_ = CameraType::Normal;
	}

	///-------------------------------------------/// 
	/// 更新
	///-------------------------------------------///
	void NormalCamera::Update() {
		// 基底クラスの更新を呼ぶ
		CameraCommon::Update();
	}

	///-------------------------------------------/// 
	/// 情報
	///-------------------------------------------///
	void NormalCamera::ImGuiUpdate() {
		// 基底クラスのImGui更新を呼ぶ
		CameraCommon::ImGuiUpdate();
	}

	///-------------------------------------------/// 
	/// デバッグ用の更新
	///-------------------------------------------///
	void NormalCamera::DebugUpdate() {
		// 基底クラスのデバッグ更新を呼ぶ
		CameraCommon::DebugUpdate();
	}
}
