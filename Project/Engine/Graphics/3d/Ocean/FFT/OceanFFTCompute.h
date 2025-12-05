#pragma once
/// ===Include=== ///
#include "Engine/Core/ComPtr.h"
// Buffer
#include "Engine/Graphics/Base/BufferBase.h"
#include "Engine/Graphics/Base/UAV.h"
// Data
#include "Engine/DataInfo/OceanFFTData.h"
// C++
#include <d3d12.h>
#include <memory>
#include <array>

/// ===前方宣言=== ///
class SRVManager;

///=====================================================
/// OceanFFTCompute
///=====================================================
class OceanFFTCompute {
public:
    OceanFFTCompute() = default;
    ~OceanFFTCompute();

    /// 初期化
    void Initialize(ID3D12Device* device, int resolution = kOceanResolution);

    /// フレーム更新
    void Update();

    /// Compute Shader実行
    void Dispatch(ID3D12GraphicsCommandList* commandList);

    /// パラメータ設定
    void SetWindSpeed(float speed) { phillipsParams_.windSpeed = speed; }
    void SetWindDirection(const Vector2& dir) { phillipsParams_.windDirection = dir; }
    void SetChoppiness(float value) { settings_.choppiness = value; }
    void SetAmplitude(float value) { phillipsParams_.amplitude = value; }

    /// 取得
    uint32_t GetDisplacementSRV() const { return displacementSRV_; }
    uint32_t GetNormalSRV() const { return normalSRV_; }
    ID3D12Resource* GetDisplacementTexture() const { return displacementTexture_.Get(); }
    ID3D12Resource* GetNormalTexture() const { return normalTexture_.Get(); }

private:
    /// <summary>
    /// テクスチャ作成
    /// </summary>
    /// <param name="device">テクスチャを作成するために使用する Direct3D 12 デバイスへのポインタ。有効なデバイスを渡す必要があります。</param>
    void CreateTextures(ID3D12Device* device);

    /// <summary>
    /// 初期スペクトル生成
    /// </summary>
    /// <param name="commandList">初期スペクトル生成用のGPUコマンドを記録する ID3D12GraphicsCommandList のポインタ。nullptr であってはなりません。</param>
    void GenerateInitialSpectrum(ID3D12GraphicsCommandList* commandList);

    /// <summary>
    /// 時間依存スペクトル更新
    /// </summary>
    /// <param name="commandList">スペクトラムの更新処理を記録するための Direct3D 12 コマンドリストへのポインタ。</param>
    void UpdateSpectrum(ID3D12GraphicsCommandList* commandList);

    /// <summary>
    /// FFT実行
    /// </summary>
    /// <param name="commandList">コマンドを記録するための有効な ID3D12GraphicsCommandList へのポインタ。FFT の GPU 実行に使用されます。nullptr であってはなりません。</param>
    void ExecuteFFT(ID3D12GraphicsCommandList* commandList);

    /// <summary>
    /// 法線とJacobian計算
    /// </summary>
    /// <param name="commandList">計算および関連するGPU操作を記録するための ID3D12GraphicsCommandList へのポインタ。</param>
    void CalculateNormalsAndDerivatives(ID3D12GraphicsCommandList* commandList);

    /// <summary>
    /// バッファとビュー作成
    /// </summary>
    /// <param name="device">バッファやビューの作成に使用するID3D12Deviceへのポインタ。</param>
    /// <param name="srvManager">作成したビューを管理・登録するSRVManagerへのポインタ。</param>
    void CreateBuffersAndViews(ID3D12Device* device, SRVManager* srvManager);

private:
    // パラメータ
    PhillipsSpectrumParams phillipsParams_;
    OceanSimulationSettings settings_;

    // バッファ
    std::unique_ptr<BufferBase> phillipsBuffer_;
    std::unique_ptr<BufferBase> settingsBuffer_;
    std::unique_ptr<BufferBase> fftParamsBuffer_;

    // テクスチャ(UAV/SRV)
    ComPtr<ID3D12Resource> h0kTexture_;          // 初期スペクトル h0(k)
    ComPtr<ID3D12Resource> hktTexture_;          // 時間依存スペクトル h(k,t)
    ComPtr<ID3D12Resource> dxTexture_;           // X方向変位
    ComPtr<ID3D12Resource> dyTexture_;           // Y方向変位(高さ)
    ComPtr<ID3D12Resource> dzTexture_;           // Z方向変位
    ComPtr<ID3D12Resource> displacementTexture_; // 統合変位マップ
    ComPtr<ID3D12Resource> normalTexture_;       // 法線マップ

    // Ping-pong バッファ(FFT用)
    std::array<ComPtr<ID3D12Resource>, 2> fftPingPong_;

    // UAV/SRVインデックス
    uint32_t h0kUAV_ = 0;
    uint32_t hktUAV_ = 0;
    uint32_t hktSRV_ = 0;
    uint32_t dxUAV_ = 0;
    uint32_t dyUAV_ = 0;
    uint32_t dzUAV_ = 0;
    uint32_t displacementUAV_ = 0;
    uint32_t displacementSRV_ = 0;
    uint32_t normalUAV_ = 0;
    uint32_t normalSRV_ = 0;
    std::array<uint32_t, 2> pingPongUAV_;
    std::array<uint32_t, 2> pingPongSRV_;

    // 管理
    SRVManager* srvManager_ = nullptr;
    int resolution_ = kOceanResolution;
    float currentTime_ = 0.0f;

};