#pragma once
// Windows
#include <Windows.h>

// int32_tを使うためのもの
#include <cstdint>

class WinApp {
public:
	// ウィンドウクラス名
	static const wchar_t kWindowClassName[];

public:

	/// <summary>
	/// ウィンドウの横幅の取得
	/// </summary>
	/// <returns></returns>
	const int GetWindowWidth() const;

	/// <summary>
	/// ウィンドウの縦幅の取得
	/// </summary>
	/// <returns></returns>
	const int GetWindowHeight() const;

	/// <summary>
	/// ウィンドウプロージャ
	/// </summary>
	/// <param name="hwnd"></param>
	/// <param name="msg"></param>
	/// <param name="WPARAM"></param>
	/// <param name="LPARAM"></param>
	/// <returns></returns>
	static LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);

	/// <summary>
	/// ゲームウィンドウの作成関数
	/// </summary>
	/// <param name="kWindowClassName"></param>
	/// <param name="kClientWidth"></param>
	/// <param name="kClientHeight"></param>
	void CreateGameWindow(const wchar_t* title, int32_t kClientWidth, int32_t kClientHeight);

	/// <summary>
	/// ゲームウィンドウの破棄
	/// </summary>
	void TerminateGameWindow();

	/// <summary>
	/// メッセージの処理
	/// </summary>
	/// <returns></returns>
	bool ProcessMessage();

	/// <summary>
	/// ウィンドウハンドルの取得
	/// </summary>
	/// <returns></returns>
	HWND GetHwnd() const;

	/// <summary>
	/// ウィンドウclassの取得
	/// </summary>
	/// <returns></returns>
	WNDCLASS GetWNDClass() const;

private:// メンバ変数

	// ウィンドウサイズ
	int32_t windowWidth_ = 1280;  // 横幅
	int32_t windowHeight_ = 720;  // 縦幅

	WNDCLASS wndClass_{};		// ウィンドウクラス
	HWND hwnd_ = nullptr;		// ウィンドウハンドル
	UINT windowStyle_;			// 
	bool isFullScreen_ = false; // 
	RECT windowRect_;			// 
};