#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Core/ComPtr.h"
#include "Engine/Graphics/Base/BufferBase.h"
#include "Engine/Graphics/Base/UAV.h"
// Data
#include "Engine/DataInfo/OceanData.h"
// Math
#include "Math/Matrix4x4.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"
// c++
#include <d3d12.h>
#include <memory>
#include <wrl.h>

namespace MiiEngine {
    /// ===前方宣言=== ///
    class SRVManager;

    ///=====================================================/// 
    /// FFTOceanCompute
    ///=====================================================///
    class FFTOceanCompute {
    public:
        FFTOceanCompute() = default;
        ~FFTOceanCompute();

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
        /// FFTパラメータを更新します。
        /// </summary>
        /// <param name="windDirection">風の方向（XZ平面）。</param>
        /// <param name="windSpeed">風の速度。</param>
        /// <param name="amplitude">波の振幅。</param>
        /// <param name="choppiness">チョッピネス（波の尖り具合）。</param>
        /// <param name="time">現在時刻。</param>
        void UpdateFFTParams(const Vector2& windDirection, float windSpeed, float amplitude, float choppiness, float time);

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
        /// ===FFTパラメータ（GPU送信用）=== ///
        // TODO: FFTOcean.CS.hlsl のレジスタ構成に合わせて調整すること
        struct FFTOceanParamsForGPU {
            Vector2 windDirection;  // 風の方向（XZ平面）
            float windSpeed;        // 風の速度
            float amplitude;        // 波の振幅
            float choppiness;       // チョッピネス
            float time;             // 現在時刻
            Vector2 padding;        // アライメント用
        };

        /// ===設定情報=== ///
        struct FFTWaveSettings {
            int gridSize;
            float gridWidth;
            float gridDepth;
            float normalEpsilon;
            Matrix4x4 worldMatrix;
            Vector3 worldOffset;
            float padding1;
        };

        /// ===バッファリソース=== ///
        std::unique_ptr<BufferBase> fftParamsBuffer_;   // FFTパラメータバッファ（ConstantBuffer）
        std::unique_ptr<BufferBase> settingsBuffer_;    // 設定バッファ（ConstantBuffer）
        std::unique_ptr<UAV> outputUAV_;                // 出力バッファ用UAVラッパー
        ComPtr<ID3D12Resource> outputBuffer_;           // 出力バッファリソース
        SRVManager* srvManager_ = nullptr;              // SRV管理クラス

        /// ===バッファデータポインタ=== ///
        FFTOceanParamsForGPU* fftParamsData_ = nullptr;
        FFTWaveSettings* settingsData_ = nullptr;

        /// ===Index=== ///
        uint32_t uavIndex_ = 0;
        uint32_t srvIndex_ = 0;

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
        /// <param name="srvManager">SRVManagerへのポインタ。</param>
        void CreateViews(ID3D12Device* device, SRVManager* srvManager);
    };
}
