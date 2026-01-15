#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Scene/Framework.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"

///=====================================================/// 
/// ゲーム全体
///=====================================================///
class MyGame : public Framework {
public:

	MyGame();
	~MyGame();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="title">タイトルの名前</param>
	void Initialize(const wchar_t* title) override;

	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレームの更新処理
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:/// ====== ///

	/// ===シーン=== ///
	// シーンマネージャ
	std::unique_ptr<SceneManager> sceneManager_;
private:
	/// ===Load=== ///
	
	/// <summary>
	/// Audioの読み込み処理
	/// </summary>
	void LoadAudio();

	/// <summary>
	/// テクスチャの読み込み処理
	/// </summary>
	void LoadTexture();

	/// <summary>
	/// モデルの読み込み処理
	/// </summary>
	void LoadModel();

	/// <summary>
	/// アニメーションモデルの読み込み処理
	/// </summary>
	void LoadAnimation();

	/// <summary>
	/// Jsonデータの読み込み処理
	/// </summary>
	void LoadJson();
};

