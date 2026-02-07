#include "PiplineManager.h"
// C++
#include <cassert>

namespace MiiEngine {
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

			// Compilerの作成と初期化
			auto compiler = std::make_unique<Compiler>();
			compiler->Initialize(dxCommon, type);
			compiler_[type] = std::move(compiler);

			// Computeパイプラインの作成
			if (IsComputePipeline(type)) {
				auto CSPipeline = std::make_unique<CSPSOCommon>();
				CSPipeline->Create(dxCommon, compiler_[type].get(), type);
				// パイプラインの追加
				computePipelines_[type] = std::move(CSPipeline);
			} else {
				// BlendMode毎にパイプラインを作成
				for (BlendMode mode : AllBlendModes()) {

					// ペアのキーをｓ作成
					auto key = std::make_pair(type, mode);

					// パイプラインの作成
					auto GSPipeline = std::make_unique<GSPSOCommon>();
					GSPipeline->Create(dxCommon, compiler_[type].get(), type, mode);

					// パイプラインの追加
					graphicsPipelines_[key] = std::move(GSPipeline);
				}
			}
		}
	}

	///-------------------------------------------/// 
	/// パイプラインの登録
	///-------------------------------------------///
	void PipelineManager::SetPipeline(ID3D12GraphicsCommandList* commandList, PipelineType type, BlendMode mode, D3D12_PRIMITIVE_TOPOLOGY topology) {

		// Computeパイプラインの場合
		if (IsComputePipeline(type)) {
			CSPSOCommon* CSPipeline = GetCSPipeline(type);
			assert(CSPipeline != nullptr);
			// PSO を設定
			CSPipeline->SetPSO(commandList);
		} else {
			// Graphicsパイプラインの取得
			GSPSOCommon* GSPipeline = GetGSPipeline(type, mode);
			assert(GSPipeline != nullptr);

			// PSO を設定
			GSPipeline->SetPSO(commandList);

			//プリミティブトポロジー設定
			commandList->IASetPrimitiveTopology(topology);
		}
	}


	///-------------------------------------------/// 
	/// タイプとモードを取得
	///-------------------------------------------///
	GSPSOCommon* PipelineManager::GetGSPipeline(PipelineType type, BlendMode mode) {
		auto key = std::make_pair(type, mode);
		auto it = graphicsPipelines_.find(key);
		if (it != graphicsPipelines_.end()) {
			return it->second.get();
		}
		return nullptr;
	}

	///-------------------------------------------/// 
	/// CSパイプラインの取得
	///-------------------------------------------///
	CSPSOCommon* PipelineManager::GetCSPipeline(PipelineType type) {
		auto it = computePipelines_.find(type);
		if (it != computePipelines_.end()) {
			return it->second.get();
		}
		return nullptr;
	}
}