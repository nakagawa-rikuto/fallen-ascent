
#pragma once
/// ===Include=== ///
// PipelineType
#include "Engine/DataInfo/PipelineStateObjectType.h"

// c++
#include <d3d12.h>

///=====================================================/// 
/// BlendState
///=====================================================///
class BlendState {
public:

	BlendState() = default;
	~BlendState() = default;

	/// <summary>
	///  ブレンドステートの生成
	/// </summary>
	/// <param name="mode">使用するブレンド（合成）モード。BlendMode 列挙型の値を指定します。</param>
	void Create(BlendMode mode);

	/// <summary>
	/// ブレンドステートの取得
	/// </summary>
	/// <returns></returns>
	D3D12_BLEND_DESC GetBlendDesc();

private:

	D3D12_BLEND_DESC blendDesc_{}; // BlendState
};
