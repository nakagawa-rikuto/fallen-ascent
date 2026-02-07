#pragma once
#include "Math/Vector2.h"
#include "Math/Vector3.h"

namespace MiiEngine {
    ///=====================================================/// 
    /// Ocean Shader用データ構造
    ///=====================================================///

    const int kWaveCount_ = 3; // 使用する波の数

    /// <summary>
    /// 波情報構造体（ゲルストナー波用）
    /// </summary>
    struct OceanShaderInfo {
        Vector3 distance;   // 波の方向ベクトル
        float amplitude;    // 波の振幅
        float length;       // 波の波長
        float speed;        // 波の速度
        float time;         // 時間
        float padding;      // アライメント用（16バイト境界）
    };

    /// <summary>
    /// 波紋情報構造体
    /// </summary>
    struct RippleInfo {
        Vector2 position;   // 波紋の中心位置(XZ平面)
        float startTime;    // 波紋の開始時間
        float intensity;    // 波紋の強度
        float duration;     // 波紋の持続時間
        float maxRadius;    // 波紋の最大半径
        float speed;        // 波紋の拡散速度
        float padding;      // アライメント用
    };

    /// <summary>
    /// 波紋データ（CPU側管理用）
    /// </summary>
    struct RippleData {
        Vector2 position;
        float startTime;
        float intensity;
        float duration;
        float maxRadius;
        float speed;
        int priority;       // 優先度（新しいほど高い）
        bool isActive;
    };

    /// <summary>
    /// 波紋バッファ（GPU送信用）
    /// </summary>
    struct RippleBufferForGPU {
        RippleInfo ripples[8];  // 最大8つの波紋
        int activeCount;        // アクティブな波紋の数
        float currentTime;      // 現在時刻
        float rippleSpeed;      // デフォルトの拡散速度
        float rippleDecay;      // デフォルトの減衰率
    };

    /// <summary>
    /// 海の色情報
    /// </summary>
    struct OceanColorInfo {
        Vector3 seaBase;        // 基本の海の色
        float padding1;
        Vector3 seaShallow;     // 浅瀬の色
        float padding2;
        Vector3 sky;            // 空の色
        float padding3;
        Vector3 deepWater;      // 深海の色
        float baseStrength;     // 色の強度
        float heightOffset;     // 波の高さに基づいた色のオフセット
        float waterClarity;     // 水の透明度
        float foamThreshold;    // 泡の閾値
    };

    /// <summary>
    /// Compute Shader出力用頂点データ
    /// </summary>
    struct WaveVertexData {
        Vector3 position;   // 波の影響を受けた位置
        Vector3 normal;     // 計算された法線
        Vector2 worldXZ;    // ワールド座標のXZ
        float padding;      // アライメント用
    };
}