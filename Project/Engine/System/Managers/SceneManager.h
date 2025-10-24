#pragma once
/// ===Include=== ///
// c++
#include <memory>
#include <string>
// Game
#include "Engine/Scene/AbstractSceneFactory.h"
// Imgui
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI

///=====================================================/// 
/// シーンマネージャー
///=====================================================///
class SceneManager {
public:/// ===基本的な関数=== ///

	SceneManager() = default;
	~SceneManager();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 現在のシーンの更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 現在のシーンの描画処理
	/// </summary>
	void Draw();

public:/// ===シーン処理=== ///

	/// <summary>
	/// シーンの切り替え処理
	/// </summary>
	/// <param name="type">切り替えるシーンの種類を指定します（通常は SceneType 列挙型の値）。</param>
	void ChangeScene(SceneType type);

	/// <summary>
	/// シーンの観測を行う関数
	/// </summary>
	void SceneObservation();

public: /// ===Getter=== ///

	// 選択されたレベル番号の取得
	int GetSelectedLevel() const;

public: /// ===Setter=== ///

	// レベルの設定
	void SetSelectedLevel(int level);

	// シーンファクトリーのSetter
	void SetSceneFactory(AbstractSceneFactory* sceneFactory);

private:
	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;

	// 現在のシーン
	SceneType currentSceneType_ = SceneType::Title; // 初期のシーン
	std::unique_ptr<IScene> currentScene_; // 現在のシーン

	// 選択されたレベル番号を保持する
	int selectLevel_ = 1;
};

