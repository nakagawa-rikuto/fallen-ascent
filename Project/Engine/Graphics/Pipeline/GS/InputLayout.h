#pragma once
/// ===Include=== ///
// PipelineType
#include "Engine/DataInfo/PipelineStateObjectType.h"
// c++
#include <d3d12.h>
#include <unordered_map>

namespace MiiEngine {
	///=====================================================/// 
	/// InputLayout
	///=====================================================///
	class InputLayout {
	public:

		InputLayout() = default;
		~InputLayout() = default;

		/// <summary>
		/// インプットレイアウトの生成処理
		/// </summary>
		/// <param name="Type">作成するパイプラインの種類を指定します。</param>
		void Create(PipelineType Type);

		/// <summary>
		/// インプットレイアウトの取得
		/// </summary>
		/// <returns></returns>
		D3D12_INPUT_LAYOUT_DESC GetInputLayout()const;

	private:

		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc_{}; // InputLayout
	};
}
