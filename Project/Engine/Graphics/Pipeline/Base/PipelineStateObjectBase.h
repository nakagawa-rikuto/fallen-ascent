#pragma once
/// ===Include=== ///
// PipelineType
#include "Engine/DataInfo/PipelineStateObjectType.h"
// C++
#include <memory>
#include <array>
// directX
#include <d3d12.h>
#include <dxgidebug.h>
#include <dxgi1_6.h>
#include <dxcapi.h>

///=====================================================/// 
/// PipelineStateObjectBase
///=====================================================///
class PipelineStateObjectBase {
public:

	virtual ~PipelineStateObjectBase() = default;

	/// <summary>
	/// 指定されたコマンドリストにパイプラインステートオブジェクト (PSO) を設定する純粋仮想関数。
	/// </summary>
	/// <param name="commandList">設定対象の ID3D12GraphicsCommandList へのポインタ。PSO を適用するコマンドリストを指定します。</param>
	virtual void SetPSO(ID3D12GraphicsCommandList* commandList) = 0;
};

