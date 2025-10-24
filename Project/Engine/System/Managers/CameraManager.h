#pragma once
/// ===Include=== ///
// GameCamera
#include "application/Game/Camera/GameCamera.h"
// C++
#include <unordered_map>
#include <string>
#include <memory>

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
	/// <param name="camera">追加する GameCamera オブジェクトへの std::shared_ptr。</param>
	void AddCamera(const std::string& name, std::shared_ptr<GameCamera> camera);

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

	// 指定されたカメラのGetter
	std::shared_ptr<GameCamera> GetCamera(const std::string& name) const;
	// アクティブカメラのGetter
	std::shared_ptr<GameCamera> GetActiveCamera() const;
	// アクティブカメラのSetter
	void SetActiveCamera(const std::string& name);

private:
	// カメラを管理するマップ
	std::unordered_map<std::string, std::shared_ptr<GameCamera>> cameras_;

	// 現在アクティブなカメラ
	std::shared_ptr<GameCamera> activeCamera_;
};