#pragma once
/// ===Include=== ///
// CameraCommon
#include "Engine/Camera/Base/CameraCommon.h"
// C++
#include <unordered_map>
#include <string>
#include <memory>

namespace MiiEngine {
	///=====================================================/// 
	/// カメラマネージャ
	///=====================================================///
	class CameraManager {
	public:/// ===基本的な関数=== ///

		CameraManager();
		~CameraManager();

		/// <summary>
		/// 全てのカメラの更新処理
		/// </summary>
		void UpdateAllCameras();

	public:/// ===関数=== ///

		/// <summary>
		/// カメラの追加処理
		/// </summary>
		/// <param name="name">追加するカメラの識別名（const std::string&）。</param>
		/// <param name="camera">追加する CameraCommon オブジェクトへのポインタ。</param>
		void AddCamera(const std::string& name, CameraCommon* camera);

		/// <summary>
		/// カメラの削除処理
		/// </summary>
		/// <param name="name">削除対象のカメラの名前。</param>
		void RemoveCamera(const std::string& name);

		/// <summary>
		/// 指定した名前のカメラが存在するかの確認処理
		/// </summary>
		/// <param name="name">確認対象のカメラ名。存在を識別するための文字列参照。</param>
		/// <returns>指定した名前のカメラが存在する場合は true、存在しない場合は false。</returns>
		bool HasCamera(const std::string& name) const;

	public:/// ===Setter・Getter=== ///

		// 指定された名前のカメラのGetter
		CameraCommon* GetCamera(const std::string& name) const;
		// アクティブカメラのGetter
		CameraCommon* GetActiveCamera() const;
		// アクティブカメラのSetter
		void SetActiveCamera(const std::string& name);

	private:
		// カメラを管理するマップ
		std::unordered_map<std::string, CameraCommon*> cameras_;

		// 現在アクティブなカメラ
		CameraCommon* activeCamera_ = nullptr;
	};
}