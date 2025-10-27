#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"

/// ===波紋情報=== ///
struct RippleData {
    Vector2 position;      // 波紋の中心位置
    float startTime;       // 波紋の開始時間
    float intensity;       // 波紋の強度
    bool isActive;         // アクティブかどうか
    float duration;        // 波紋の持続時間
    float maxRadius;       // 波紋の最大半径
    float speed;           // 波紋の拡散速度
    int priority;          // 優先度
};

// GPU用の波紋情報
struct RippleInfoForGPU {
    Vector2 position;
    float startTime;
    float intensity;
    float duration;
    float maxRadius;
    float speed;
    float padding;       // アライメント用
};

struct RippleBufferForGPU {
    RippleInfoForGPU ripples[8];
    int activeCount;
    float currentTime;
    float rippleSpeed;   // デフォルトの速度として残す
    float rippleDecay;
};

/// ===オーシャンシェーダーの情報=== ///
struct OceanShaderInfo {
    Vector3 distance;    // 波の方向ベクトル
    float amplitude;     // 波の振幅
    float length;        // 波の波長
    float speed;         // 波の速度
    float time;          // 時間
    float padding[3];    // パディング
};

/// ===オーシャンシェーダーの色の構造体=== ///
struct OceanColorInfo {
    Vector3 seaBase;      // 基本の海の色
    float padding1;
    Vector3 seaShallow;   // 浅瀬の色
    float padding2;
    Vector3 sky;          // 空の色
    float padding3;
    Vector3 deepWater;    // 深い水の色
    float baseStrength;   // 色の強度
    float heightOffset;   // 波の高さに基づいた色のオフセット
    float waterClarity;   // 水の透明度
    float foamThreshold;  // 泡の閾値
};