#include "CSPSOCommon.h"
// Compiler
#include "Engine/Graphics/Pipeline/CS/CSCompiler.h"
// Engine
#include "Engine/Core/DXCommon.h"
// c++
#include <cassert>

namespace MiiEngine {
	///-------------------------------------------/// 
	/// デストラクタ
	///-------------------------------------------///
	CSPSOCommon::~CSPSOCommon() {
		rootSignature_.reset();
		computePipelineState_.Reset();
	}

	///-------------------------------------------/// 
	/// PSOの作成
	///-------------------------------------------///
	void CSPSOCommon::Create(DXCommon* dxCommon, CSCompiler* compiler, CSPipelineType Type, const std::wstring& kernelName) {
		// RootSignatureの生成
		rootSignature_ = std::make_unique<CSRootSignature>();
		rootSignature_->Create(dxCommon, Type);
		compiler_ = compiler;
		// PipelineState
		CreatePipelineState(dxCommon, kernelName);
	}

	///-------------------------------------------/// 
	/// パイプラインの設定
	///-------------------------------------------///
	void CSPSOCommon::SetPSO(ID3D12GraphicsCommandList* commandList) {
		// assertでチェック
		assert(rootSignature_);
		assert(computePipelineState_);

		// RootSignatureの設定
		commandList->SetComputeRootSignature(rootSignature_->GetRootSignature());
		// PSOの設定
		commandList->SetPipelineState(computePipelineState_.Get());
	}

	///-------------------------------------------/// 
	/// パイプラインステートの作成
	///-------------------------------------------///
	void CSPSOCommon::CreatePipelineState(DXCommon* dxCommon, const std::wstring& kernelName) {
		// 指定カーネルのBlobを取得
		IDxcBlob* csBlob = compiler_->GetKernel(kernelName);
		assert(csBlob && "CS Kernel Blob is null");

		// ComputePipelineStateDescの設定
		computePipelineStateDesc_ = {};
		computePipelineStateDesc_.pRootSignature = rootSignature_->GetRootSignature();
		computePipelineStateDesc_.CS = {
			.pShaderBytecode = csBlob->GetBufferPointer(),
			.BytecodeLength = csBlob->GetBufferSize(),
		};

		// PipelineStateの生成
		HRESULT hr = dxCommon->GetDevice()->CreateComputePipelineState(
			&computePipelineStateDesc_, IID_PPV_ARGS(&computePipelineState_));
		assert(SUCCEEDED(hr));
	}
}