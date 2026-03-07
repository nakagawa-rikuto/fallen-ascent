#pragma once
/// ===Include=== ///
#include "Base/CameraCommon.h"

namespace MiiEngine {
	///=====================================================/// 
	/// カメラ基底クラス
	///=====================================================///
	class NormalCamera : public CameraCommon {
	public:
		NormalCamera() = default;
		~NormalCamera() = default;

		/// <summary>
		/// 初期化処理
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// 更新処理
		/// </summary>
		void Update() override;

		/// <summary>
		/// ImGui情報の更新
		/// </summary>
		void ImGuiUpdate() override;

		/// <summary>
		/// デバッグ用の更新
		/// </summary>
		void DebugUpdate() override;
	};
}