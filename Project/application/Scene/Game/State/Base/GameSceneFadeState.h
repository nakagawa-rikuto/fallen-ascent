#pragma once
/// ===前方宣言=== ///
class GameScene;
class SceneManager;

///=====================================================/// 
/// ゲームシーンフェード状態
///=====================================================///
class GameSceneFadeState {
public:
	virtual ~GameSceneFadeState() = default;

	/// <summary>
	/// ステートに入る際に呼び出される仮想関数。
	/// </summary>
	/// <param name="gameScene">入るゲームシーンへのポインタ。</param>
	virtual void Enter(GameScene* gameScene) = 0;

	/// <summary>
	/// ステート時の更新処理、純粋仮想関数
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// ステート時の描画処理、純粋仮想関数
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// ステート終了処理
	/// </summary>
	virtual void Finalize();

public: /// ===Setter=== ///
	// SceneManagerの設定
	void SetSceneManager(SceneManager* sceneManager) { sceneManager_ = sceneManager; }

protected:
	// ゲームシーン
	GameScene* gameScene_ = nullptr; 
	// SceneManager
	SceneManager* sceneManager_ = nullptr;
};

