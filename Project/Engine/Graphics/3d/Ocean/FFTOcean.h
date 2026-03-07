#pragma once
/// ===Include=== ///
// buffer
#include "Engine/Graphics/Base/BufferBase.h"
// Common
#include "OceanCommon.h"
// FFT Wave Compute
#include "FFTOceanCompute.h"
// Pipeline
#include "Engine/DataInfo/PipelineStateObjectType.h"
// Data
#include "Engine/DataInfo/OceanData.h"
// Math
#include "Math/Vector2.h"
// c++
#include <memory>

namespace MiiEngine {
    ///=====================================================/// 
    /// FFTOcean
    ///=====================================================///
    class FFTOcean : public OceanCommon {
    public:
        FFTOcean() = default;
        ~FFTOcean();

        /// <summary>
        /// 初期化処理
        /// </summary>
        /// <param name="gridSize">グリッドのサイズ（縦横の分割数）</param>
        void Initialize(int gridSize);

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
        /// <summary>
        /// FFTパラメータを設定します。
        /// </summary>
        /// <param name="windDirection">風の方向（XZ平面）。</param>
        /// <param name="windSpeed">風の速度。</param>
        /// <param name="amplitude">波の振幅。</param>
        /// <param name="choppiness">チョッピネス（波の尖り具合）。</param>
        void SetFFTParams(const Vector2& windDirection, float windSpeed, float amplitude, float choppiness);

        /// <summary>
        /// 色情報を設定します。
        /// </summary>
        /// <param name="colorInfo">設定するOceanColorInfo。</param>
        void SetColorInfo(const OceanColorInfo& colorInfo);

    private:
        /// ===FFT用パラメータ構造体=== ///
        struct FFTOceanParams {
            Vector2 windDirection;  // 風の方向（XZ平面）
            float windSpeed;        // 風の速度
            float amplitude;        // 波の振幅
            float choppiness;       // チョッピネス
            float time;             // 現在時刻
            Vector2 padding;        // アライメント用
        };

        /// ===バッファリソース=== ///
        std::unique_ptr<BufferBase> vertex_;
        std::unique_ptr<BufferBase> index_;
        std::unique_ptr<BufferBase> oceanColorBuffer_;    // 色情報バッファ

        /// ===FFT Wave Compute=== ///
        std::unique_ptr<FFTOceanCompute> fftCompute_;

        /// ===バッファリソース内のデータを指すポインタ=== ///
        uint32_t* indexData_ = nullptr;
        OceanColorInfo* oceanColorData_ = nullptr;

        /// ===バッファビュー=== ///
        D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
        D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

        /// ===グリッド情報=== ///
        int gridSize_ = 64;
        int vertexCount_ = 0;
        int indexCount_ = 0;

        /// ===FFTパラメータ=== ///
        FFTOceanParams fftParams_ = {
            { 1.0f, 0.0f },    // windDirection
            10.0f,             // windSpeed
            1.0f,              // amplitude
            1.5f,              // choppiness
            0.0f,              // time
            { 0.0f, 0.0f }     // padding
        };

        /// ===時間管理=== ///
        float currentTime_ = 0.0f;

        /// ===Ocean Color情報=== ///
        OceanColorInfo colorInfo_ = {
            { 0.1f, 0.4f, 0.6f }, 0.0f,
            { 0.2f, 0.7f, 0.8f }, 0.0f,
            { 0.4f, 0.6f, 0.9f }, 0.0f,
            { 0.0f, 0.05f, 0.2f },
            1.2f,
            0.3f,
            0.4f,
            0.5f
        };

    private: /// ===Functions=== ///
        /// <summary>
        /// ColorData書き込み処理
        /// </summary>
        void ColorDataWrite();
    };
}
