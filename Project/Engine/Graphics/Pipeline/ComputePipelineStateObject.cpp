#include "ComputePipelineStateObject.h"
// Compiler
#include "Engine/Graphics/Pipeline/Setting/Compiler.h"
// Engine
#include "Engine/Core/DXCommon.h"


///-------------------------------------------/// 
/// コンストラクタ
///-------------------------------------------///
ComputePipelineStateObject::ComputePipelineStateObject() = default;

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
ComputePipelineStateObject::~ComputePipelineStateObject() {
	rootSignature_.reset();
	computePipelineState_.Reset();
}

///-------------------------------------------/// 
/// PSOの作成
///-------------------------------------------///
void ComputePipelineStateObject::Create(DXCommon* dxcommon, Compiler* compiler, PipelineType Type) {

	// RootSignatureの生成
	rootSignature_ = std::make_unique<CSRootSignature>();
	rootSignature_->Create(dxcommon, Type);

	// Compilerの初期化
	compiler_ = compiler;

	// PipelineState
	CreatePipelineState(dxcommon, Type);
}

///-------------------------------------------/// 
/// PSOの設定
///-------------------------------------------///
void ComputePipelineStateObject::SetPSO(ID3D12GraphicsCommandList * commandList) {

	// assertでチェック
	assert(rootSignature_);
	assert(computePipelineState_);

	// RootSignatureの設定
	commandList->SetComputeRootSignature(rootSignature_->GetRootSignature());
	// PSOの設定
	commandList->SetPipelineState(computePipelineState_.Get());
}

///-------------------------------------------/// 
/// PipelineStateの生成処理
///-------------------------------------------///
void ComputePipelineStateObject::CreatePipelineState(DXCommon* dxCommon, PipelineType type) {
	HRESULT hr;

	// ComputePipelineStateDescの設定
	computePipelineStateDesc_.CS = {
		.pShaderBytecode = compiler_->GetObjCS()->GetBufferPointer(),
		.BytecodeLength = compiler_->GetObjCS()->GetBufferSize()
	};
	// RootSignatureの設定
	computePipelineStateDesc_.pRootSignature = rootSignature_->GetRootSignature();
	// 実際に生成
	hr = dxCommon->GetDevice()->CreateComputePipelineState(&computePipelineStateDesc_, IID_PPV_ARGS(&computePipelineState_));
	assert(SUCCEEDED(hr));
}