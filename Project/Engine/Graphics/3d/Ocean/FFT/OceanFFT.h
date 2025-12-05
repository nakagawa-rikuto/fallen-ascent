#pragma once
/// ===Include=== ///
// Buffer
#include "Engine/Graphics/Base/BufferBase.h"
// Common
#include "Engine/Graphics/3d/Ocean/geltner/OceanCommon.h"
// FFT Compute
#include "OceanFFTCompute.h"
// Pipeline
#include "Engine/DataInfo/PipelineStateObjectType.h"
// c++
#include <memory>
#include <array>

///=====================================================/// 
/// OceanFFT - FFTベースの海洋レンダリング
///=====================================================///
class OceanFFT : public OceanCommon {
public:
    OceanFFT() = default;
    ~OceanFFT();

    /// <summary>
    /// 初期化処理
    /// </summary>
    /// <param name="gridSize">グリッドのサイズ（縦横の分割数）</param>
    /// <param name="fftResolution">FFTテクスチャの解像度（デフォルト256）</param>
    void Initialize(int gridSize, int fftResolution = 256);

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update();

    /// <summary>
    /// 描画処理
    /// </summary>
    /// <param name="mode">ブレンドモード</param>
    void Draw(BlendMode mode = BlendMode::KBlendModeNormal);

    /// <summary>
    /// ImGui情報の表示
    /// </summary>
    void ShowImGui();

public: /// ===Setter=== ///
    // FFT Ocean Parameters
    void SetWindSpeed(float speed);
    void SetWindDirection(const Vector2& dir);
    void SetChoppiness(float value);
    void SetAmplitude(float value);
    void SetSuppression(float value);

    // Ocean Color Parameters
    void SetColorInfo(const OceanFFTColorInfo& colorInfo);

public: /// ===Getter=== ///
    float GetWindSpeed() const { return windSpeed_; }
    Vector2 GetWindDirection() const { return windDirection_; }
    float GetChoppiness() const { return choppiness_; }

private:
    /// ===バッファリソース=== ///
    std::unique_ptr<BufferBase> vertex_;
    std::unique_ptr<BufferBase> index_;
    std::unique_ptr<BufferBase> oceanColorBuffer_;

    /// ===FFT Compute=== ///
    std::unique_ptr<OceanFFTCompute> fftCompute_;

    /// ===バッファリソース内のデータを指すポインタ=== ///
    VertexData3D* vertexData_ = nullptr;
    uint32_t* indexData_ = nullptr;
    OceanFFTColorInfo* oceanColorData_ = nullptr;

    /// ===バッファビュー=== ///
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
    D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

    /// ===グリッド情報=== ///
    int gridSize_ = 64;
    int fftResolution_ = 256;
    int vertexCount_ = 0;
    int indexCount_ = 0;

    /// ===FFTパラメータ=== ///
    float windSpeed_ = 30.0f;
    Vector2 windDirection_ = { 1.0f, 0.0f };
    float amplitude_ = 0.2f;
    float suppression_ = 0.001f;
    float choppiness_ = 1.5f;
    float currentTime_ = 0.0f;

    /// ===Ocean Color情報=== ///
    OceanFFTColorInfo colorInfo_ = {
        { 0.1f, 0.4f, 0.6f }, 0.0f,
        { 0.2f, 0.7f, 0.8f }, 0.0f,
        { 0.4f, 0.6f, 0.9f }, 0.0f,
        { 0.0f, 0.05f, 0.2f },
        1.2f,
        0.3f,
        0.5f,
        1.0f,
        0.0f
    };

private: /// ===Functions=== ///
    /// <summary>
    /// グリッドメッシュの生成
    /// </summary>
    void CreateGridMesh();

    /// <summary>
    /// ColorData書き込み処理
    /// </summary>
    void ColorDataWrite();

    /// <summary>
    /// Compute Shader実行（FFT計算）
    /// </summary>
    void ExecuteFFTCompute(ID3D12GraphicsCommandList* commandList);
};