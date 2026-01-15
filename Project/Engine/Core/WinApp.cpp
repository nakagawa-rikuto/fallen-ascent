#include "WinApp.h"
#include <string>

#ifdef USE_IMGUI
#include <imGui.h>
#endif // USE_IMGUI


#ifdef _DEBUG
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
#endif // _DEBUG

// ウィンドウクラスの名前
const wchar_t WinApp::kWindowClassName[] = L"DeathsDoor";

///-------------------------------------------/// 
/// ウィンドウの横幅の取得
///-------------------------------------------///
const int WinApp::GetWindowWidth() const { return windowWidth_; }

///-------------------------------------------/// 
/// ウィンドウの縦幅の取得
///-------------------------------------------///
const int WinApp::GetWindowHeight() const { return windowHeight_; }

///-------------------------------------------/// 
/// ウィンドウプロージャ
///-------------------------------------------///
LRESULT WinApp::WindowProc(HWND hwnd, UINT msg, WPARAM WPARAM, LPARAM LPARAM) {
#ifdef USE_IMGUI
	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, WPARAM, LPARAM)) {
		return true;
	}
#endif // USE_IMGUI

	// メッセージに応じたゲーム固有の処理
	switch (msg) {

		// ウィンドウが破棄された
	case WM_DESTROY:

		// OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	// 標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, WPARAM, LPARAM);
}

///-------------------------------------------/// 
/// ゲームウィンドウの作成
///-------------------------------------------///
void WinApp::CreateGameWindow(const wchar_t* title, int32_t kClientWidth, int32_t kClientHeight) {

	// ウィンドウサイズの設定
	windowWidth_ = kClientWidth;
	windowHeight_ = kClientHeight;

	/* /////////////////////////////////////
					COMの初期化
	*/ /////////////////////////////////////
	// COMの初期化
	HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
	if (FAILED(hr)) {
		// 初期化失敗時のエラーハンドリング
		MessageBox(nullptr, L"COMの初期化に失敗しました。", L"エラー", MB_OK | MB_ICONERROR);
		return;
	}

	/* /////////////////////////////////////
				ウィンドウクラスの登録
	*/ /////////////////////////////////////
	// ウィンドウプロージャ
	wndClass_.lpfnWndProc = WindowProc;

	// ウィンドウクラス名
	wndClass_.lpszClassName = kWindowClassName;

	// インスタンスハンドル
	wndClass_.hInstance = GetModuleHandle(nullptr);

	// カーソル
	wndClass_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	// ウィンドウクラスを登録
	RegisterClass(&wndClass_);

	/* /////////////////////////////////////
			　　　　ウィンドの作成
	*/ /////////////////////////////////////
	// ウィンドウサイズを表す構造体にクライアント領域を入れる
	RECT wrc = { 0, 0, kClientWidth, kClientHeight };

	// クライアント領域を元に実際のサイズにwrcを変更してもらう
	AdjustWindowRect(&wrc, WS_OVERLAPPEDWINDOW, false);

	// ウィンドウの生成
	hwnd_ = CreateWindow(
		wndClass_.lpszClassName,    // 利用するクラス名
		title,                      // タイトルバーの文字
		WS_OVERLAPPEDWINDOW,        // ウィンドウスタイル
		CW_USEDEFAULT,              // 表示X座標
		CW_USEDEFAULT,              // 表示Y座標
		wrc.right - wrc.left,       // ウィンドウの横幅
		wrc.bottom - wrc.top,       // ウィンドウの縦幅
		nullptr,                    // 親ウィンドウハンドル
		nullptr,                    // メニューハンドル
		wndClass_.hInstance,        // インスタンスハンドル
		nullptr);                   // オプション

	// ウィンドウを表示する
	ShowWindow(hwnd_, SW_SHOW);
}

///-------------------------------------------/// 
/// ゲームウィンドウの破棄
///-------------------------------------------///
void WinApp::TerminateGameWindow() {
	// ウィンドウクラスを登録解除
	UnregisterClass(wndClass_.lpszClassName, wndClass_.hInstance);
}

///-------------------------------------------/// 
/// メッセージの処理
///-------------------------------------------///
bool WinApp::ProcessMessage() {
	MSG msg{}; // メッセージ

	// Windowにメッセージが来てたら最優先で処理させる
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg); // キー入力メッセージの処理
		DispatchMessage(&msg); // ウィンドウプロシージャにメッセージを送る
	}

	if (msg.message == WM_QUIT) { //　終了メッセージが来たらループを抜ける
		return true;
	}

	return false;
}

/// <summary>
/// ウィンドウハンドルの取得
/// </summary>
HWND WinApp::GetHwnd() const { return hwnd_; }

/// <summary>
/// ウィンドウクラスの取得
/// </summary>
/// <returns></returns>
WNDCLASS WinApp::GetWNDClass() const { return wndClass_; }
