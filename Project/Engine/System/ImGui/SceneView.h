#pragma once
/// ===Include=== ///
#include <d3dx12.h>

///-------------------------------------------/// 
/// SceneView
///-------------------------------------------///
class SceneView {
public:
	/// <summary>
	/// テクスチャの設定
	/// </summary>
	/// <param name="handle">ハンドル</param>
	void SetTextureHandle(D3D12_GPU_DESCRIPTOR_HANDLE handle);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	D3D12_GPU_DESCRIPTOR_HANDLE textureHandle_{};
};

