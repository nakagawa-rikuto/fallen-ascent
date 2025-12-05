#pragma once
/// ===Include=== ///
#include "Math/Vector2.h"
#include "Math/Vector3.h"
#include "Math/Vector4.h"
#include "Math/Matrix4x4.h"

// FFT海洋シミュレーション用の定数
constexpr int kOceanResolution = 256; // 解像度(2のべき乗)
constexpr float kOceanSize = 640.0f; // 海のサイズ(メートル)
constexpr float kGravity = 9.81f; // 重力加速度

// Phillips スペクトルのパラメータ
struct PhillipsSpectrumParams {
    float windSpeed = 30.0f;        // 風速 (m/s)
    Vector2 windDirection = { 1.0f, 0.0f }; // 風の方向
    float amplitude = 0.0002f;      // 波の振幅スケール
    float suppression = 0.001f;     // 小さい波の抑制
    float padding[3];
};

// 海洋シミュレーション設定
struct OceanSimulationSettings {
    int resolution = kOceanResolution;
    float oceanSize = kOceanSize;
    float time = 0.0f;
    float choppiness = 1.5f;        // 波の尖り具合
    float tileScale = 1.0f;         // タイリングスケール
    int cascadeCount = 3;           // カスケード数
    float padding[2];
};

// 複素数構造体
struct Complex {
    float real;
    float imag;

    float padding[2]; // GPU alignment
};

// FFT計算用のパラメータ
struct FFTParams {
    int stage;          // FFTのステージ
    int pingPong;       // Ping-pong バッファの選択
    int direction;      // 0: horizontal, 1: vertical
    int resolution;
    float padding[4];
};

// 海洋の色情報(既存のものを拡張)
struct OceanFFTColorInfo {
    Vector3 seaBase = { 0.1f, 0.4f, 0.6f };
    float padding1 = 0.0f;
    Vector3 seaShallow = { 0.2f, 0.7f, 0.8f };
    float padding2 = 0.0f;
    Vector3 sky = { 0.4f, 0.6f, 0.9f };
    float padding3 = 0.0f;
    Vector3 deepWater = { 0.0f, 0.05f, 0.2f };
    float baseStrength = 1.2f;
    float waterClarity = 0.3f;
    float foamThreshold = 0.5f;
    float foamScale = 1.0f;
    float padding4 = 0.0f;
};

// カスケード情報
struct CascadeInfo {
    float scale;        // このカスケードのスケール
    float windSpeed;    // このカスケードの風速
    float padding[2];
};

// 頂点データ(FFT版)
struct OceanVertexData {
    Vector4 position;   // xyz: position, w: unused
    Vector2 texcoord;
    Vector3 normal;
    float padding;
};