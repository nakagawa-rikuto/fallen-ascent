#pragma once
/// ===Include=== ///
// PipelineType
#include "Engine/DataInfo/PipelineStateObjectType.h"
// directX
#include <d3d12.h>
#include <unordered_map>

namespace MiiEngine {
	///=====================================================/// 
	/// RasterizerState
	///=====================================================///
	class RasterizerState {
	public:

		RasterizerState() = default;
		~RasterizerState() = default;

		/// <summary>
		/// ラスタライザステートの生成処理
		/// </summary>
		/// <param name="Type">作成するパイプラインの種類を指定します。</param>
		void Create(PipelineType Type);

		/// <summary>
		/// ラスタライザステートの取得
		/// </summary>
		/// <returns></returns>
		D3D12_RASTERIZER_DESC GetRasterizerState()const;

	private:
		// RasterizerState
		D3D12_RASTERIZER_DESC rasterizerDesc_{};
	};
}
