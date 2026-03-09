#pragma once
/// ===Include=== ///
#include "CSRootSignature.h"
// PipelineType
#include "Engine/DataInfo/PipelineStateObjectType.h"
// c++
#include <memory>
#include <array>
#include <cassert>
#include <string>
// directX
#include <d3d12.h>
#include <dxgidebug.h>
#include <dxgi1_6.h>
#include <dxcapi.h>

namespace MiiEngine {
	/// ===前方宣言=== ///
	class DXCommon;
	class CSCompiler;

	///=====================================================/// 
	/// ComputePipelineStateObjectCommon
	///=====================================================///
	class CSPSOCommon {
	public:

		CSPSOCommon() = default;
		~CSPSOCommon();

		/// <summary>
		/// PSOの生成処理
		/// </summary>
		/// <param name="DXCommon">DirectX に関連する共通リソース、デバイス、コンテキストを保持するオブジェクトへのポインタ。パイプライン作成に必要な環境情報を提供します。</param>
		/// <param name="compiler">シェーダやパイプラインステージをコンパイルするためのコンパイラオブジェクトへのポインタ。</param>
		/// <param name="Type">作成するパイプラインの種類を指定する列挙型（PipelineType）の値。</param>
		/// <param name="kernelName">カーネルの名前。</param>
		void Create(DXCommon* dxcommon, CSCompiler* compiler, CSPipelineType Type, const std::wstring& kernelName = L"main");

		/// <summary>
		/// PSOの設定処理
		/// </summary>
		/// <param name="commandList">PSO を設定する対象の ID3D12GraphicsCommandList へのポインタ。有効なコマンドリストを渡してください。</param>
		void SetPSO(ID3D12GraphicsCommandList* commandList);

	private: /// ===変数=== ///

		/// ===PSOに必要な情報=== ///
		std::unique_ptr<CSRootSignature> rootSignature_;    // RootSignature
		CSCompiler* compiler_;                              // Compiler

		ComPtr<ID3D12PipelineState> computePipelineState_;
		D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineStateDesc_{};

	private: /// ===関数=== ///

		/// <summary>
		/// パイプラインステートを作成します。
		/// </summary>
		/// <param name="dxCommon">DirectX の共通リソースやコンテキストを保持する DXCommon オブジェクトへのポインタ。</param>
		/// <param name="kernelName">使用するシェーダーカーネルの名前。通常はエントリーポイント関数名（例: L"main"）を指定します。</param>
		void CreatePipelineState(DXCommon* dxCommon, const std::wstring& kernelName);
	};
}

