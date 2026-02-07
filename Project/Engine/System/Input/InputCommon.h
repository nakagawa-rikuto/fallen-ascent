#pragma once
// Engine
#include "Engine/Core/ComPtr.h"
// c++
#define DIRECTINPUT_VERSION   0x0800   // DirectInputのバージョン指定
#include <dinput.h>
#include <Windows.h>
#include <array>
#include <vector>
#include <wrl.h>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

namespace MiiEngine {
	/// ===前方宣言=== ///
	class WinApp;

	///=====================================================/// 
	/// 共通入力処理
	///=====================================================///
	class InputCommon {
	public:

		InputCommon() = default;
		~InputCommon();

		/// <summary>
		/// 初期化処理
		/// </summary>
		/// <param name="winApp">Class WinApp</param>
		void Initialize(WinApp* winApp);

	public: /// ===Getter=== ///
		// DirectInput
		ComPtr<IDirectInput8> GetDirectInput();

	private:
		// DirectInputインスタンス
		ComPtr<IDirectInput8> directInput_;
	};
}

