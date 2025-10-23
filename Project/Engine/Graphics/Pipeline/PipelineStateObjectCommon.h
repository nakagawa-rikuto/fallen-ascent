#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Graphics/Pipeline/RootSignature.h"
#include "Engine/Graphics/Pipeline/InputLayout.h"
#include "Engine/Graphics/Pipeline/BlendState.h"
#include "Engine/Graphics/Pipeline/DepthStencil.h"
#include "Engine/Graphics/Pipeline/RasterizerState.h"
// PipelineType
#include "Engine/DataInfo/PipelineStateObjectType.h"
// c++
#include <memory>
#include <array>
#include <cassert>
// directX
#include <d3d12.h>
#include <dxgidebug.h>
#include <dxgi1_6.h>
#include <dxcapi.h>


/// ===前方宣言=== ///
class DXCommon;
class Compiler;

class PipelineStateObjectCommon {
public:

	PipelineStateObjectCommon();
	~PipelineStateObjectCommon();

	/// <summary>
	/// PSOの生成処理
	/// </summary>
	/// <param name="dxcommon">DirectX に関連する共通リソース、デバイス、コンテキストを保持するオブジェクトへのポインタ。パイプライン作成に必要な環境情報を提供します。</param>
	/// <param name="compiler">シェーダやパイプラインステージをコンパイルするためのコンパイラオブジェクトへのポインタ。</param>
	/// <param name="Type">作成するパイプラインの種類を指定する列挙型（PipelineType）の値。</param>
	/// <param name="Mode">使用するブレンド動作を指定する列挙型（BlendMode）の値。</param>
	void Create(DXCommon* dxcommon, Compiler* compiler, PipelineType Type, BlendMode Mode);

	/// <summary>
	/// PSOの設定処理
	/// </summary>
	/// <param name="commandList">PSO を設定する対象の ID3D12GraphicsCommandList へのポインタ。有効なコマンドリストを渡してください。</param>
	void SetPSO(ID3D12GraphicsCommandList* commandList);

private:/// ===変数=== ///

	/// ===PSOに必要な情報=== ///
	std::unique_ptr<RootSignature> rootSignature_;     // RootSignature
	std::unique_ptr<InputLayout> inputLayout_;         // InputLayout
	std::unique_ptr<BlendState> blendState_;           // BlendState
	std::unique_ptr<RasterizerState> rasterizerState_; // RasterizerState
	std::unique_ptr<DepthStencil> depthStencil_;       // DepthStencil
	Compiler* compiler_;                               // Compiler

	ComPtr<ID3D12PipelineState> graphicsPipelineState_;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc_{};

private:/// ===関数=== ///

	/// <summary>
	/// PipelineStateの生成処理
	/// </summary>
	/// <param name="dxCommon">DirectX のデバイスやリソース情報を保持するコンテキストへのポインタ。パイプラインステート作成時に必要な情報を提供します。</param>
	/// <param name="type">作成するパイプラインの種類を示す値（例: グラフィックス、コンピュートなど）。</param>
	void CreatePipelineState(DXCommon* dxCommon, PipelineType type);
};