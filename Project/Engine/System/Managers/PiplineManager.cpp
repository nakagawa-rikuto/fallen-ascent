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
		/// ===GSPipeline=== ///
		for (PipelineType type : AllPipelineTypes()) {
			// Compilerの作成と初期化
			auto compiler = std::make_unique<Compiler>();
			compiler->Initialize(dxCommon, type);
			compiler_[type] = std::move(compiler);

			// BlendMode毎にパイプラインを作成
			for (BlendMode mode : AllBlendModes()) {

				// ペアのキーをｓ作成
				auto key = std::make_pair(type, mode);

				// パイプラインの作成
				auto gsPipeline = std::make_unique<GSPSOCommon>();
				gsPipeline->Create(dxCommon, compiler_[type].get(), type, mode);

				// パイプラインの追加
				graphicsPipelines_[key] = std::move(gsPipeline);
			}
		}

		/// ===CSPipeline=== ///
		for (CSPipelineType type : AllCSPipelineTypes()) {
			// CSCompilerの作成と初期化
			auto csCompiler = std::make_unique<CSCompiler>();
			csCompiler->Initialize(dxCommon, type);
			// ムーブ前にカーネル名リストを取得する
			const auto kernelNames = csCompiler->GetKernelNames();
			csCompiler_[type] = std::move(csCompiler); // ← ここでムーブ

			// CSFFTOcean の場合、カーネルごとに CSPSOCommon を生成
			for (const auto& kernelName : kernelNames) {

				// ペアのキーを作成
				auto key = std::make_pair(type, kernelName);

				// パイプラインの作成
				auto csPipeline = std::make_unique<CSPSOCommon>();
				csPipeline->Create(dxCommon, csCompiler_[type].get(), type, kernelName);

				// パイプラインの追加
				computePipelines_[key] = std::move(csPipeline);
			}
		}
	}

	///-------------------------------------------/// 
	/// パイプラインの登録
	///-------------------------------------------///
	void PipelineManager::SetPipeline(ID3D12GraphicsCommandList* commandList, PipelineType type, BlendMode mode, D3D12_PRIMITIVE_TOPOLOGY topology) {

		// Graphicsパイプラインの取得
		GSPSOCommon* GSPipeline = GetGSPipeline(type, mode);
		assert(GSPipeline != nullptr);

		// PSO を設定
		GSPipeline->SetPSO(commandList);

		//プリミティブトポロジー設定
		commandList->IASetPrimitiveTopology(topology);
	}

	///-------------------------------------------/// 
	/// CSパイプラインの登録 
	///-------------------------------------------///
	void PipelineManager::SetCSPipeline(ID3D12GraphicsCommandList* commandList, CSPipelineType type, const std::wstring& kernelName) {

		// Computeパイプラインの取得
		CSPSOCommon* CSPipeline = GetCSPipeline(type, kernelName);
		assert(CSPipeline != nullptr);

		// PSO を設定
		CSPipeline->SetPSO(commandList);
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
	CSPSOCommon* PipelineManager::GetCSPipeline(CSPipelineType type, const std::wstring& kernelName) {
		auto key = std::make_pair(type, kernelName);
		auto it = computePipelines_.find(key);
		if (it != computePipelines_.end()) {
			return it->second.get();
		}
		return nullptr;
	}
}