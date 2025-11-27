#include "PiplineManager.h"

#include <cassert>

///-------------------------------------------/// 
/// コンストラクタ、デストラクタ
///-------------------------------------------///
PipelineManager::PipelineManager() = default;
PipelineManager::~PipelineManager() { 
	graphicsPipelines_.clear();
	computePipelines_.clear();
	compiler_.clear();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void PipelineManager::Initialize(DXCommon* dxCommon) {
	for (PipelineType type : AllPipelineTypes()) {

		// もしかしたらブレンドモード以外も作るべきかも
		auto compiler = std::make_unique<Compiler>();
		compiler->Initialize(dxCommon, type);
		compiler_[type] = std::move(compiler);

		// CSを使用するときはコンピュートパイプラインも作成
		if (IsComputePipeline(type)) {
			auto CSpipeline = std::make_unique<ComputePipelineStateObject>();
			CSpipeline->Create(dxCommon, compiler_[type].get(), type);
			computePipelines_[type] = std::move(CSpipeline);
		}
		// グラフィックスパイプライン
		for (BlendMode mode : AllBlendModes()) {
			auto key = std::make_pair(type, mode);
			auto pipeline = std::make_unique<GraphicsPipelineStateObject>();
			pipeline->Create(dxCommon, compiler_[type].get(), type, mode);
			graphicsPipelines_[key] = std::move(pipeline);
		}
	}
}

///-------------------------------------------/// 
/// パイプラインの登録
///-------------------------------------------///
void PipelineManager::SetPipeline(
	ID3D12GraphicsCommandList * commandList, PipelineType type, BlendMode mode, D3D12_PRIMITIVE_TOPOLOGY topology) {
	
	if (IsComputePipeline(type)) {
		// コンピュートパイプラインの取得
		ComputePipelineStateObject* CSpipeline = GetComputePipeline(type);
		assert(CSpipeline != nullptr && "Compute Pipeline not found");
		CSpipeline->SetPSO(commandList);
	} 
	// グラフィックスパイプラインの取得
	GraphicsPipelineStateObject* pipeline = GetGraphicsPipeline(type, mode);
	assert(pipeline != nullptr && "Graphics Pipeline not found");
	pipeline->SetPSO(commandList);

	// プリミティブトポロジー設定
	commandList->IASetPrimitiveTopology(topology);
}

///-------------------------------------------/// 
/// タイプとモードを取得
///-------------------------------------------///
GraphicsPipelineStateObject* PipelineManager::GetGraphicsPipeline(PipelineType type, BlendMode mode) {
	auto key = std::make_pair(type, mode);
	auto it = graphicsPipelines_.find(key);
	if (it != graphicsPipelines_.end()) {
		return it->second.get();
	}
	return nullptr;
}

///-------------------------------------------/// 
/// タイプを取得
///-------------------------------------------///
ComputePipelineStateObject* PipelineManager::GetComputePipeline(PipelineType type) {
	auto it = computePipelines_.find(type);
	if (it != computePipelines_.end()) {
		return it->second.get();
	}
	return nullptr;
}

