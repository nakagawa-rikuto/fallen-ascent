#pragma once
/// ===Include=== ///
// MiiEngine
#include "Engine/Core/Mii.h"
// CameraManager
#include "Engine/System/Managers/CameraManager.h"
// ParticleManager
#include "Engine/System/Managers/ParticleManager.h"
// ColliderManager
#include "Engine/System/Managers/ColliderManager.h"
// SceneFactory
#include "Engine/Scene/AbstractSceneFactory.h"
// ｃ++
#include <memory>

///=====================================================/// 
/// フレームワーク
///=====================================================///
class Framework {
public:/// ====== ///

	Framework() = default;
	virtual ~Framework() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="title">タイトルの名前</param>
	virtual void Initialize(const wchar_t* title);
	
	/// <summary>
	/// 終了処理
	/// </summary>
	virtual void Finalize();

	/// <summary>
	/// 毎フレーム更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理、純粋仮想関数
	/// </summary>
	virtual void Draw() = 0;
	// 終了フラグのチェック
	virtual bool IsEndRequst();

public:/// ===メンバ関数=== ///
	/// <summary>
	/// 実行処理
	/// </summary>
	/// <param name="title">タイトルの名前</param>
	void Run(const wchar_t* title);
	
	/// <summary>
	/// 描画前処理
	/// </summary>
	void PreDraw();
	
	/// <summary>
	/// 描画後処理
	/// </summary>
	void PostDraw();

private:
	// MiiEngine
	std::unique_ptr<Mii> MiiEngine_;
	// CameraManager
	std::unique_ptr<CameraManager> cameraManager_;
	// ParticleManager
	std::unique_ptr<ParticleManager> particleManager_;
	// ColliderManager
	std::unique_ptr<ColliderManager> colliderManager_;
protected:
	// ゲーム終了フラグ
	bool endRequst_ = false;
	// シーンファクトリー
	std::unique_ptr<AbstractSceneFactory> sceneFactory_;
};

