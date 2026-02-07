#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Core/ComPtr.h"
#include "Engine/Graphics/Base/BufferBase.h"
#include "Engine/Graphics/Base/UAV.h"
// Data
#include "Engine/DataInfo/OceanData.h"
// c++
#include <d3d12.h>
#include <memory>
#include <array>
#include <wrl.h>

namespace MiiEngine {
    /// ===前方宣言=== ///
    class SRVManager;

    ///=====================================================/// 
    /// OceanWaveCompute
    ///=====================================================///
    class OceanWaveCompute {
    public:
        OceanWaveCompute() = default;
        ~OceanWaveCompute();

        /// <summary>
        /// 指定したデバイスとグリッドサイズを用いて初期化を行います。
        /// </summary>
        /// <param name="device">初期化に使用する ID3D12Device へのポインタ。nullptr であってはなりません。</param>
        /// <param name="gridSize">初期化するグリッドのサイズを表す整数（要素数や解像度などを指定）。</param>
        void Initialize(ID3D12Device* device, int gridSize);

        /// <summary>
        /// 指定したコマンドリスト上でディスパッチ操作を実行します。
        /// </summary>
        /// <param name="commandList">ディスパッチを実行する対象の ID3D12GraphicsCommandList へのポインタ。</param>
        void Dispatch(ID3D12GraphicsCommandList* commandList);

        /// <summary>
        /// 海面シェーダーで使用する波情報を更新します。
        /// </summary>
        /// <param name="waveInfos">12要素の OceanShaderInfo を格納した std::array への const 参照。各要素が個別の波のパラメータを表し、これらを内部状態に適用します。</param>
        void UpdateWaveInfos(const std::array<OceanShaderInfo, kWaveCount_>& waveInfos);

        /// <summary>
        /// GPU 用のリップルバッファの内容を更新します。
        /// </summary>
        /// <param name="rippleBuffer">更新対象の RippleBufferForGPU 型のオブジェクトへの const 参照。更新に必要なデータを提供します。</param>
        void UpdateRippleBuffer(const RippleBufferForGPU& rippleBuffer);

        /// <summary>
        /// 指定されたワールド行列で内部のワールド変換を更新します。
        /// </summary>
        /// <param name="worldMatrix">更新に使用する読み取り専用のワールド行列への参照。</param>
        void UpdateWorldMatrix(const Matrix4x4& worldMatrix);

        /// <summary>
        /// コマンドリストに対して結果を頂点バッファにコピーするコマンドを記録します。
        /// </summary>
        /// <param name="commandList">コピーコマンドを記録するID3D12GraphicsCommandListへのポインタ。</param>
        /// <param name="vertexBuffer">コピー先の頂点バッファを表すID3D12Resourceへのポインタ。</param>
        void CopyResultsToVertexBuffer(ID3D12GraphicsCommandList* commandList, ID3D12Resource* vertexBuffer);

    public: /// ===取得=== ///
        // UAVのIndex取得
        uint32_t GetUAVIndex() const;
        // SRVのIndex取得
        uint32_t GetSRVIndex() const;
        // 出力バッファリソースの取得
        ID3D12Resource* GetOutputBuffer() const;

    private:
        /// ===バッファリソース=== ///
        std::unique_ptr<BufferBase> waveInfoBuffer_;    // 12個の波情報（StructuredBuffer）
        std::unique_ptr<BufferBase> rippleBuffer_;      // 波紋バッファ（ConstantBuffer）
        std::unique_ptr<BufferBase> settingsBuffer_;    // 設定バッファ（ConstantBuffer）
        std::unique_ptr<UAV> outputUAV_;                // 出力バッファ用UAVラッパー
        ComPtr<ID3D12Resource> outputBuffer_;           // 出力バッファリソース
        SRVManager* srvManager_ = nullptr;              // SRV管理クラス

        /// ===設定情報=== ///
        struct WaveSettings {
            int gridSize;
            float gridWidth;
            float gridDepth;
            float normalEpsilon;
            Matrix4x4 worldMatrix;
            Vector3 worldOffset;
            float padding1;
        };
        WaveSettings* settingsData_ = nullptr;

        /// ===Index=== ///
        uint32_t uavIndex_ = 0;
        uint32_t srvIndex_ = 0;

        /// ===波情報=== ///
        OceanShaderInfo* waveInfoData_ = nullptr;
        RippleBufferForGPU* rippleBufferData_ = nullptr;

        /// ===グリッド情報=== ///
        int gridSize_ = 128;
        int vertexCount_ = 0;

    private:

        /// <summary>
        /// UAV（Unordered Access View）用のバッファを作成します。
        /// </summary>
        /// <param name="device">バッファ作成に使用する ID3D12Device インターフェイスへのポインタ。</param>
        void CreateUAVBuffer(ID3D12Device* device);

        /// <summary>
        /// UAVとSRVのビューを作成します。
        /// </summary>
        /// <param name="device">ビューを作成するために使用する ID3D12Device へのポインタ。</param>
        void CreateViews(ID3D12Device* device, SRVManager* srvMangaer);
    };
}