#include "CSPSOCommon.h"
// Compiler
#include "Engine/Graphics/Pipeline/Compiler.h"
// Engine
#include "Engine/Core/DXCommon.h"
// c++
#include <cassert>

///-------------------------------------------/// 
/// コンストラクタ
///-------------------------------------------///
CSPSOCommon::CSPSOCommon() = default;

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
CSPSOCommon::~CSPSOCommon() {
	rootSignature_.reset();
	uav_.reset();
	computePipelineState_.Reset();
}

///-------------------------------------------/// 
/// PSOの作成
///-------------------------------------------///
void CSPSOCommon::Create(DXCommon* dxCommon, Compiler* compiler, PipelineType Type) {
	// RootSignatureの生成
	rootSignature_ = std::make_unique<CSRootSignature>();
	rootSignature_->Create(dxCommon, Type);
	// UAVの生成
	uav_ = std::make_unique<UAV>();
	// uav_->Create(dxCommon, Type, ); numElementsとstructureByteStrideの設定を考え中
	compiler_ = compiler;
	// PipelineState
	CreatePipelineState(dxCommon, Type);
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
void CSPSOCommon::CreatePipelineState(DXCommon* dxCommon, PipelineType type) {
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