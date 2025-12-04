#pragma once
/// ===Include=== ///
// buffer
#include "Engine/Graphics/Base/BufferBase.h"
// Common
#include "OceanCommon.h"
// Wave Compute
#include "OceanWaveCompute.h"
// Pipeline
#include "Engine/DataInfo/PipelineStateObjectType.h"
// Data
#include "Engine/DataInfo/OshanData.h"
// c++
#include <vector>
#include <array>
#include <algorithm>

///=====================================================/// 
/// Oshan
///=====================================================///
class Ocean : public OceanCommon {
public:
    Ocean() = default;
    ~Ocean();

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

    /// <summary>
    /// 円状に広がる波紋を追加
    /// </summary>
    void AddCircularRipple(const Vector3& center, float duration = 3.0f, float intensity = 1.0f, float maxRadius = 20.0f);

    /// <summary>
    /// すべての波紋をクリア
    /// </summary>
    void ClearRipples();

public: /// ===Setter=== ///
    // Ocean Wave Parameters（12個の波対応）
    void SetWaveInfo(int waveIndex, const Vector3& direction, float amplitude, float length, float speed);
    // Ocean Color Parameters
    void SetColorInfo(const OceanColorInfo& colorInfo);
    // Ripple Parameters
    void SetRippleSpeed(float speed);
    void SetRippleDecay(float decay);

private:
    /// ===バッファリソース=== ///
    std::unique_ptr<BufferBase> vertex_;
    std::unique_ptr<BufferBase> index_;
    std::unique_ptr<BufferBase> oceanColorBuffer_;    // 色情報バッファ

    /// ===Wave Compute=== ///
    std::unique_ptr<OceanWaveCompute> waveCompute_;

    /// ===バッファリソース内のデータを指すポインタ=== ///
    VertexData3D* vertexData_ = nullptr;
    uint32_t* indexData_ = nullptr;
    OceanColorInfo* oceanColorData_ = nullptr;

    /// ===バッファビュー=== ///
    D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
    D3D12_INDEX_BUFFER_VIEW indexBufferView_{};

    /// ===グリッド情報=== ///
    int gridSize_ = 64;
    int vertexCount_ = 0;
    int indexCount_ = 0;

    /// ===波情報（12個）=== ///
    std::array<OceanShaderInfo, kWaveCount_> waveInfos_;

    /// ===波紋管理=== ///
    std::array<RippleData, 8> ripples_;
    RippleBufferForGPU rippleBuffer_;
    float currentTime_ = 0.0f;
    float rippleSpeed_ = 4.0f;
    float rippleDecay_ = 1.0f;
    int nextPriority_ = 0;

    // 衝突検知用
    struct HitRecord {
        int objectID;
        Vector3 lastPosition;
        bool wasHitting;
    };
    std::vector<HitRecord> hitRecords_;

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
    /// グリッドメッシュの生成（初期状態・平面）
    /// </summary>
    void CreateGridMesh();

    /// <summary>
    /// ColorData書き込み処理
    /// </summary>
    void ColorDataWrite();

    /// <summary>
    /// 波紋の更新処理
    /// </summary>
    void UpdateRipples();

    /// <summary>
    /// 波情報の初期化（12個）
    /// </summary>
    void InitializeWaveInfos();
};