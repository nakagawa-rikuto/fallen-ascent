#pragma once
/// ===Include=== ///
// PipelineType
#include "Engine/DataInfo/PipelineStateObjectType.h"
// directX
#include <d3d12.h>
#include <unordered_map>

///=====================================================/// 
/// DepthStencil
///=====================================================///
class DepthStencil {
public:

	DepthStencil() = default;
	~DepthStencil() = default;

	/// <summary>
	/// デプスステンシルの生成処理
	/// </summary>
	/// <param name="Type">作成するパイプラインの種類を指定する PipelineType 型の値。</param>
	void Create(PipelineType Type);

	/// <summary>
	/// デプスステンシルの取得
	/// </summary>
	/// <returns></returns>
	D3D12_DEPTH_STENCIL_DESC GetDepthStencilDesc() const;

private:

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc_{}; // DepthStencil
};