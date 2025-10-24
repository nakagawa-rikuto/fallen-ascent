#pragma once
/// ===Include=== ///
#include "Engine/Core/ComPtr.h"
// ImGui
#ifdef USE_IMGUI
#include <imgui.h>
#include <imgui_impl_dx12.h>
#include <imgui_impl_win32.h>
#endif // USE_IMGUI

/// ===前方宣言=== ///
class WinApp;
class DXCommon;
class SRVManager;
class SceneView;

///=====================================================/// 
/// ImGuiManager
///=====================================================///
class ImGuiManager {
public:

	ImGuiManager();
	~ImGuiManager();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="winApp">ウィンドウやアプリケーション固有の設定・状態を管理する WinApp オブジェクトへのポインタ。</param>
	/// <param name="dxCommon">DirectX に関連する共通リソースや初期化処理を管理する DXCommon オブジェクトへのポインタ。</param>
	/// <param name="srvManager">シェーダーリソースビュー（SRV）などのリソース管理を行う SRVManager オブジェクトへのポインタ。</param>
	void Initialize(WinApp* winApp, DXCommon* dxCommon, SRVManager* srvManager);
	
	/// <summary>
	/// 終了処理
	/// </summary>
	void Finalize();

	/// <summary>
	/// 受付開始処理
	/// </summary>
	void Begin();

	/// <summary>
	/// 受付終了処理
	/// </summary>
	void End();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
	
public: /// ===Setter=== ///
	// SceneViewの設定
	void SetSceneView(SceneView* sceneView);

private:
	
	WinApp* winApp_ = nullptr;
	DXCommon* dxCommon_ = nullptr;
	SRVManager* srvManager_ = nullptr;
	SceneView* sceneView_ = nullptr;

private:
	/// <summary>
	/// Dock付きUI描画処理
	/// </summary>
	void DrawMainDockWindow(); 

	/// <summary>
	/// メニューバーの処理
	/// </summary>
	void MenuBar();
};

