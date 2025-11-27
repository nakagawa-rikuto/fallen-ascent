#pragma once
/// ===Include=== ///
// Base
#include "Engine/Graphics/Pipeline/Base/PipelineStateObjectBase.h"
// Pipeline Components
#include "Engine/Graphics/Pipeline/Setting/Compute/CSRootSignature.h"

/// ===前方宣言=== ///
class DXCommon;
class Compiler;

///=====================================================/// 
/// ComputePipelineStateObject
///=====================================================///
class ComputePipelineStateObject : public PipelineStateObjectBase {
	public:
	ComputePipelineStateObject();
	~ComputePipelineStateObject();

	/// <summary>
	/// PSOの生成処理
	/// </summary>
	/// <param name="dxcommon">DirectX に関連する共通リソース、デバイス、コンテキストを保持するオブジェクトへのポインタ。パイプライン作成に必要な環境情報を提供します。</param>
	/// <param name="compiler">シェーダやパイプラインステージをコンパイルするためのコンパイラオブジェクトへのポインタ。</param>
	/// <param name="Type">作成するパイプラインの種類を指定する列挙型（PipelineType）の値。</param>
	void Create(DXCommon* dxcommon, Compiler* compiler, PipelineType Type);

	/// <summary>
	/// PSOの設定処理
	/// </summary>
	/// <param name="commandList">PSO を設定する対象の ID3D12GraphicsCommandList へのポインタ。有効なコマンドリストを渡してください。</param>
	void SetPSO(ID3D12GraphicsCommandList* commandList) override;

private:/// ===変数=== ///

	/// ===PSOに必要な情報=== ///
	std::unique_ptr<CSRootSignature> rootSignature_;     // RootSignature
	Compiler* compiler_;                               // Compiler

	// ComputePipelineState
	ComPtr<ID3D12PipelineState> computePipelineState_;
	D3D12_COMPUTE_PIPELINE_STATE_DESC computePipelineStateDesc_{};

private:/// ===関数=== ///

	/// <summary>
	/// PipelineStateの生成処理
	/// </summary>
	/// <param name="dxCommon">DirectX のデバイスやリソース情報を保持するコンテキストへのポインタ。パイプラインステート作成時に必要な情報を提供します。</param>
	/// <param name="type">作成するパイプラインの種類を示す値（例: グラフィックス、コンピュートなど）。</param>
	void CreatePipelineState(DXCommon* dxCommon, PipelineType Type);
};

