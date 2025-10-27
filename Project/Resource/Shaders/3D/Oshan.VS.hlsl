#include "Oshan.hlsli"

/// ===================================================
/// VertexShader
/// ===================================================
// 変換行列の構造体
struct TransformationMatrix
{
    float4x4 WVP;
    float4x4 World;
};

// 構造体定義
struct OceanShaderInfo
{
    float3 distance; // 波の方向ベクトル
    float amplitude; // 波の振幅
    float length; // 波の波長
    float speed; // 波の速度
    float time; // 時間
};

// 波紋情報の構造体（最大8つの波紋）- 拡張版
struct RippleInfo
{
    float2 position; // 波紋の中心位置
    float startTime; // 波紋の開始時間
    float intensity; // 波紋の強度
    float duration; // 波紋の持続時間
    float maxRadius; // 波紋の最大半径
    float speed; // 波紋の拡散速度
    float padding; // アライメント用（16バイト境界）
};

struct RippleBuffer
{
    RippleInfo ripples[8]; // 最大8つの波紋
    int activeCount; // アクティブな波紋の数
    float currentTime; // 現在の時間
    float rippleSpeed; // デフォルトの拡散速度
    float rippleDecay; // デフォルトの減衰率
};

// ObjectCommonが使用するレジスタ（変更不可）
ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b1);

// Ocean専用のレジスタ
ConstantBuffer<OceanShaderInfo> gOceanShaderInfo0 : register(b4); // 波情報1
ConstantBuffer<OceanShaderInfo> gOceanShaderInfo1 : register(b5); // 波情報2
ConstantBuffer<OceanShaderInfo> gOceanShaderInfo2 : register(b6); // 波情報3
ConstantBuffer<RippleBuffer> gRippleBuffer : register(b9);

// 頂点シェーダーへの情報の受け渡し
struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

// ゲルストナー波を計算する関数
float3 GerstnerWave(float2 position, OceanShaderInfo info)
{
    const float Pi = 3.1415;
    
    // 方向ベクトルの長さをチェック
    float dirLength = length(info.distance.xy);
    if (dirLength < 0.001)  // ほぼゼロの場合
    {
        return float3(0.0, 0.0, 0.0); // 波を発生させない
    }
    
    float k = (2.0 * Pi) / info.length;
    float a = info.amplitude / k;
    float theta = dot(info.distance.xy, position * k);
    float sinTheta = sin(theta + info.speed * info.time);
    float cosTheta = cos(theta + info.speed * info.time);

    return float3(
        a * info.distance.x * cosTheta,
        a * sinTheta,
        a * info.distance.z * cosTheta
    );
}

// 波紋の計算関数
float CalculateRipple(float2 position, RippleInfo ripple, float currentTime)
{
    float elapsed = currentTime - ripple.startTime;
    
    // 波紋がまだ始まっていない、または強度が0の場合
    if (elapsed < 0.0 || ripple.intensity <= 0.0)
        return 0.0;
    
    // 持続時間を超えた場合
    if (elapsed > ripple.duration)
        return 0.0;
    
    // 波紋の中心からの距離
    float2 toRipple = position - ripple.position;
    float distance = length(toRipple);
    
    // 現在の波紋の半径（個別のspeedを使用）
    float rippleRadius = elapsed * ripple.speed;
    
    // 最大半径を超えたら表示しない
    if (rippleRadius > ripple.maxRadius)
        return 0.0;
    
    // 波紋の幅（半径に応じて動的に調整）
    float rippleWidth = 2.0 + rippleRadius * 0.05;
    
    // 距離と半径の差
    float distanceFromEdge = abs(distance - rippleRadius);
    
    // 波紋のエッジに近いほど強い（ガウス分布風）
    float edgeFactor = exp(-distanceFromEdge * distanceFromEdge / (rippleWidth * rippleWidth));
    
    // 時間による減衰（durationに基づく滑らかな減衰）
    float lifetimeRatio = elapsed / ripple.duration;
    float timeFactor = 1.0 - smoothstep(0.7, 1.0, lifetimeRatio); // 後半30%で減衰
    
    // 波の形状（サイン波）- 複数の波を重ねる
    float waveFrequency1 = 3.14159 * 3.0;
    float waveFrequency2 = 3.14159 * 5.0;
    
    float wave1 = sin((distance - rippleRadius) * waveFrequency1 / rippleWidth) * 0.5 + 0.5;
    float wave2 = sin((distance - rippleRadius) * waveFrequency2 / rippleWidth) * 0.3;
    
    float wave = wave1 + wave2 * 0.5;
    
    // 最終的な波紋の高さ
    return wave * edgeFactor * timeFactor * ripple.intensity;
}

// すべての波紋の影響を計算
float CalculateAllRipples(float2 position)
{
    float totalRipple = 0.0;
    
    for (int i = 0; i < gRippleBuffer.activeCount && i < 8; i++)
    {
        totalRipple += CalculateRipple(
            position,
            gRippleBuffer.ripples[i],
            gRippleBuffer.currentTime
        );
    }
    
    return totalRipple;
}

// 波の合成(関数) - オフセットのみを返す
float3 P(float2 worldXZ, OceanShaderInfo info0, OceanShaderInfo info1, OceanShaderInfo info2)
{
    // 初期値はゼロ（オフセットなし）
    float3 offset = float3(0.0, 0.0, 0.0);

    // ゲルストナー波の計算
    float3 wave0 = GerstnerWave(worldXZ, info0);
    float3 wave1 = GerstnerWave(worldXZ, info1);
    float3 wave2 = GerstnerWave(worldXZ, info2);

    // 波紋の影響を計算
    float rippleHeight = CalculateAllRipples(worldXZ);

    // 合成（波紋は主にY方向に影響）
    offset = wave0 + wave1 + wave2;
    offset.y += rippleHeight * 0.5; // 波紋の高さを調整（0.3 → 0.5に変更してより目立つように）

    return offset;
}

// 法線計算用の微小変位
float3 CalculateNormal(float2 position, OceanShaderInfo info0, OceanShaderInfo info1, OceanShaderInfo info2)
{
    float epsilon = 0.1;
    
    float3 posCenter = P(position, info0, info1, info2);
    float3 posRight = P(position + float2(epsilon, 0.0), info0, info1, info2);
    float3 posForward = P(position + float2(0.0, epsilon), info0, info1, info2);
    
    float3 tangent = posRight - posCenter;
    float3 bitangent = posForward - posCenter;
    
    // tangentとbitangentの長さをチェック
    float tangentLen = length(tangent);
    float bitangentLen = length(bitangent);
    
    // 波がほぼない場合はデフォルトの上向き法線を返す
    if (tangentLen < 0.001 || bitangentLen < 0.001)
    {
        return float3(0.0, 1.0, 0.0); // 上向き
    }
    
    return normalize(cross(tangent, bitangent));
}

/// 頂点シェーダーのmain
VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    OceanShaderInfo info0 = gOceanShaderInfo0;
    OceanShaderInfo info1 = gOceanShaderInfo1;
    OceanShaderInfo info2 = gOceanShaderInfo2;
    
    // まずワールド座標に変換
    float4 worldPos = mul(input.position, gTransformationMatrix.World);
    
    // ワールド座標のXZ平面で波を計算
    float2 worldXZ = worldPos.xz;
    
    // 波の影響を受けた頂点位置を計算（ワールド空間で）
    float3 waveOffset = P(worldXZ, info0, info1, info2);
    
    // 元の頂点位置に波のオフセットを加算
    float3 newPosition = input.position.xyz + waveOffset;
    
    // より正確な法線計算（ワールド空間で）
    float3 normal = CalculateNormal(worldXZ, info0, info1, info2);
    
    // 変換行列を使って位置を変換
    output.position = mul(float4(newPosition, 1.0f), gTransformationMatrix.WVP);
    output.texcood = input.texcoord;
    
    // 法線の変換
    output.normal = normalize(mul(normal, (float3x3) gTransformationMatrix.World));
    
    // ワールド座標
    output.worldPosition = mul(float4(newPosition, 1.0f), gTransformationMatrix.World).xyz;
    output.worldXZ = worldXZ;
    
    return output;
}