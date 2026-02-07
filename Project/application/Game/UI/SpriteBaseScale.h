#pragma once
/// ===Include=== ///
// Vector2
#include "Math/Vector2.h"
// Service
#include "Service/GraphicsResourceGetter.h"

///-------------------------------------------/// 
/// 基準スケールの取得
///-------------------------------------------///
inline Vector2 GetBaseScale() {
	// 基準解像度
	constexpr float BASE_WIDTH = 1920.0f;
	constexpr float BASE_HEIGHT = 1080.0f;
	// ウィンドウサイズの取得
	Vector2 windowSize = {
		static_cast<float>(Service::GraphicsResourceGetter::GetWindowWidth()),
		static_cast<float>(Service::GraphicsResourceGetter::GetWindowHeight())
	};
	// スケール計算
	Vector2 scale = { windowSize.x / BASE_WIDTH, windowSize.y / BASE_HEIGHT };
	return scale;
}


