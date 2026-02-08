#pragma once
/// ===Include=== ///
// Engine
#include "Engine/DataInfo/PipelineStateObjectType.h"
#include "Engine/Graphics/Pipeline/GS/GSPSOCommon.h"
#include "Engine/Graphics/Pipeline/CS/CSPSOCommon.h"
#include "Engine/Graphics/Pipeline/Compiler.h"
// c++
#include <unordered_map>
#include <memory>
#include <utility> // std::pair
#include <functional> // std::hash

///-------------------------------------------/// 
    /// std::pair 用のハッシュ関数を特殊化
    ///-------------------------------------------///
namespace std {
    template <>
    struct hash<std::pair<MiiEngine::PipelineType, MiiEngine::BlendMode>> {
        size_t operator()(const std::pair<MiiEngine::PipelineType, MiiEngine::BlendMode>& p) const noexcept {
            size_t h1 = std::hash<int>()(static_cast<int>(p.first));   // PipelineType のハッシュ値
            size_t h2 = std::hash<int>()(static_cast<int>(p.second)); // BlendMode のハッシュ値
            return h1 ^ (h2 << 1); // ハッシュ値を組み合わせる（XOR とシフト）
        }
    };
}

namespace MiiEngine {
    /// ===前方宣言=== ///
    class DXCommon;

    ///=====================================================/// 
    /// PipeLineManager
    ///=====================================================///
    class PipelineManager {
    public:

        PipelineManager();
        ~PipelineManager();

        /// <summary>
        /// 初期化処理
        /// </summary>
        /// <param name="dxCommon">初期化する DXCommon オブジェクトへのポインタ。</param>
        void Initialize(DXCommon* dxCommon);

        /// <summary>
        /// PSO（PipelineStateObject）の設定処理
        /// </summary>
        /// <param name="commandList">コマンドを記録するID3D12GraphicsCommandListへのポインター。設定はこのコマンドリストに適用されます。</param>
        /// <param name="type">使用するパイプラインの種類を示すPipelineType（例: グラフィックス、コンピュートなど）。</param>
        /// <param name="mode">レンダリング時のブレンド動作を指定するBlendMode。</param>
        /// <param name="topology">描画に使用するプリミティブのトポロジーを示すD3D12_PRIMITIVE_TOPOLOGY値。</param>
        void SetPipeline(ID3D12GraphicsCommandList* commandList, PipelineType type, BlendMode mode, D3D12_PRIMITIVE_TOPOLOGY topology);

    private:/// ===変数=== ///
        /// パイプラインの管理
        // Graphicsパイプライン
        std::unordered_map<std::pair<PipelineType, BlendMode>, std::unique_ptr<GSPSOCommon>> graphicsPipelines_;
        // Computeパイプライン
        std::unordered_map<PipelineType, std::unique_ptr<CSPSOCommon>> computePipelines_;
        // Compilerの管理
        std::unordered_map<PipelineType, std::unique_ptr<Compiler>> compiler_;

    private:/// ===関数=== ///
        /// <summary>
        /// 指定された PipelineType と BlendMode に基づいてパイプライン状態オブジェクトを取得
        /// </summary>
        /// <param name="type">取得するパイプラインの種類を示す値。</param>
        /// <param name="mode">適用するブレンドモードを示す値。</param>
        /// <returns>要求したパイプライン状態を表す GSPSOCommon へのポインタ。</returns>
        GSPSOCommon* GetGSPipeline(PipelineType type, BlendMode mode);

        /// <summary>
        /// 指定された種類の CSPSOCommon パイプラインへのポインタを取得します。
        /// </summary>
        /// <param name="type">取得するパイプラインの種類を示す PipelineType の値。</param>
        /// <returns>指定された種類に対応する CSPSOCommon へのポインタ。該当するパイプラインが存在しない場合は nullptr を返すことがあります。</returns>
        CSPSOCommon* GetCSPipeline(PipelineType type);
    };
}