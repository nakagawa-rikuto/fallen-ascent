#pragma once
/// ===Include=== ///
// c++
#include <memory>
#include <string>
// Game
#include "Engine/Scene/AbstractSceneFactory.h"
// SceneTransitionManager
#include "Engine/Scene/Transition/SceneTransitionManager.h"
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
	void Initialize(AbstractSceneFactory* sceneFactor);

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
	/// <param name="type">切り替えるシーンを示す値（SceneType 型）。</param>
	/// <param name="transition">使用する遷移効果（TransitionType）。省略時は TransitionType::None。</param>
	/// <param name="duraiton">遷移の継続時間（秒）。既定値は 1.0f。</param>
	void ChangeScene(SceneType type);

	/// <summary>
	/// シーンの観測を行う関数
	/// </summary>
	void SceneObservation();

	/// <summary>
	/// 指定したトランジションでフェードインを開始
	/// </summary>
	/// <param name="type">フェードインに使用するトランジションの種類。TransitionType 型で指定します。</param>
	/// <param name="duration">フェードインにかける時間（秒単位）。</param>
	void StartFadeIn(TransitionType type, const float duration);

	/// <summary>
	/// 指定した継続時間でフェードアウトを開始
	/// </summary>
	/// <param name="type">フェードインに使用するトランジションの種類。TransitionType 型で指定します。</param>
	/// <param name="duration">フェードアウトの継続時間（秒単位）。</param>
	void StartFadeOut(TransitionType type, const float duration);

public: /// ===Getter=== ///

	// 選択されたレベル番号の取得
	int GetSelectedLevel() const;

	// トランジションが完了したか
	bool GetTransitionFinished() const;
	// フェードの状態を取得
	FadeState GetFadeState() const;


public: /// ===Setter=== ///

	// レベルの設定
	void SetSelectedLevel(int level);

	// シーンファクトリーのSetter
	void SetSceneFactory(AbstractSceneFactory* sceneFactory);

private:
	// シーンファクトリー
	AbstractSceneFactory* sceneFactory_ = nullptr;

	// シーントランジションマネージャ
	std::unique_ptr<SceneTransitionManager> sceneTransitionManager_;

	// 現在のシーン
	SceneType currentSceneType_ = SceneType::Title; // 初期のシーン
	std::unique_ptr<IScene> currentScene_; // 現在のシーン

	// 選択されたレベル番号を保持する
	int selectLevel_ = 1;

private: 

	/// <summary>
	/// 指定した持続時間でシーンを初期化
	/// </summary>
	/// <param name="duration">初期化処理にかける時間（秒）。シーンの設定や遷移の持続時間を指定します。</param>
	void SceneInit();
};

