/// ===================================================
/// ComputeShader 
/// ===================================================
// 波情報の構造体
struct OceanShaderInfo
{
    float3 distance; // 波の方向ベクトル
    float amplitude; // 波の振幅
    float length; // 波の波長
    float speed; // 波の速度
    float time; // 時間
    float padding; // アライメント用
};

// 波紋情報の構造体
struct RippleInfo
{
    float2 position; // 波紋の中心位置
    float startTime; // 波紋の開始時間
    float intensity; // 波紋の強度
    float duration; // 波紋の持続時間
    float maxRadius; // 波紋の最大半径
    float speed; // 波紋の拡散速度
    float padding; // アライメント用
};

// 波紋バッファ
struct RippleBuffer
{
    RippleInfo ripples[8];
    int activeCount;
    float currentTime;
    float rippleSpeed;
    float rippleDecay;
};

// 頂点データ(出力用)
struct WaveVertexData
{
    float3 position; // 波の影響を受けた位置
    float3 normal; // 計算された法線
    float2 worldXZ; // ワールド座標のXZ
    float padding; // アライメント用
};

// 定数バッファ
cbuffer WaveSettings : register(b0)
{
    int gridSize; // グリッドサイズ
    float gridWidth; // グリッドの幅
    float gridDepth; // グリッドの奥行き
    float normalEpsilon; // 法線計算の微小変位
    float4x4 worldMatrix; // ワールド行列
    float3 worldOffset; // ワールド座標のオフセット
    float padding1;
};

// 波情報バッファ(3個の波)
StructuredBuffer<OceanShaderInfo> gWaveInfos : register(t0);
ConstantBuffer<RippleBuffer> gRippleBuffer : register(b1);

// 出力バッファ
RWStructuredBuffer<WaveVertexData> gOutputVertices : register(u0);

// 波の数
const int Waceount = 3;

// ゲルストナー波を計算する関数
float3 GerstnerWave(float2 position, OceanShaderInfo info)
{
    const float Pi = 3.1415926535;
    
    // 方向ベクトルの長さをチェック
    float dirLength = length(info.distance.xy);
    if (dirLength < 0.001)
    {
        return float3(0.0, 0.0, 0.0);
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
    
    if (elapsed < 0.0 || ripple.intensity <= 0.0)
        return 0.0;
    
    if (elapsed > ripple.duration)
        return 0.0;
    
    float2 toRipple = position - ripple.position;
    float distance = length(toRipple);
    
    float rippleRadius = elapsed * ripple.speed;
    
    if (rippleRadius > ripple.maxRadius)
        return 0.0;
    
    float rippleWidth = 2.0 + rippleRadius * 0.05;
    float distanceFromEdge = abs(distance - rippleRadius);
    float edgeFactor = exp(-distanceFromEdge * distanceFromEdge / (rippleWidth * rippleWidth));
    
    float lifetimeRatio = elapsed / ripple.duration;
    float timeFactor = 1.0 - smoothstep(0.7, 1.0, lifetimeRatio);
    
    float waveFrequency1 = 3.14159 * 3.0;
    float waveFrequency2 = 3.14159 * 5.0;
    
    float wave1 = sin((distance - rippleRadius) * waveFrequency1 / rippleWidth) * 0.5 + 0.5;
    float wave2 = sin((distance - rippleRadius) * waveFrequency2 / rippleWidth) * 0.3;
    
    float wave = wave1 + wave2 * 0.5;
    
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

// すべての波を合成してオフセットを計算(VS版と同じロジック)
float3 CalculateWaveOffset(float2 worldXZ)
{
    float3 offset = float3(0.0, 0.0, 0.0);

    // Waceountの波を合成
    for (int i = 0; i < Waceount; i++)
    {
        offset += GerstnerWave(worldXZ, gWaveInfos[i]);
    }

    // 波紋の影響を加算(VS版と同じ係数)
    float rippleHeight = CalculateAllRipples(worldXZ);
    offset.y += rippleHeight * 0.5;

    return offset;
}

// 法線を計算(VS版と同じロジック)
float3 CalculateNormal(float2 worldXZ)
{
    float epsilon = 0.1; // VS版と同じ値
    
    float3 posCenter = CalculateWaveOffset(worldXZ);
    float3 posRight = CalculateWaveOffset(worldXZ + float2(epsilon, 0.0));
    float3 posForward = CalculateWaveOffset(worldXZ + float2(0.0, epsilon));
    
    float3 tangent = posRight - posCenter;
    float3 bitangent = posForward - posCenter;
    
    float tangentLen = length(tangent);
    float bitangentLen = length(bitangent);
    
    if (tangentLen < 0.001 || bitangentLen < 0.001)
    {
        return float3(0.0, 1.0, 0.0);
    }
    
    return normalize(cross(tangent, bitangent));
}

// メインのCompute Shader
[numthreads(8, 8, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    int x = DTid.x;
    int z = DTid.y;
    
    // グリッドサイズを超える場合は処理しない
    if (x >= gridSize + 1 || z >= gridSize + 1)
        return;
    
    // 頂点インデックス
    int vertexIndex = z * (gridSize + 1) + x;
    // グリッドの各セルのサイズ
    float step = gridWidth / gridSize;
    
    // ローカル座標
    float localX = -gridWidth * 0.5 + x * step;
    float localZ = -gridDepth * 0.5 + z * step;
    
    // ワールド座標のXZを計算
    float4 localPos = float4(localX, 0.0f, localZ, 1.0f);
    float4 worldPos = mul(localPos, worldMatrix);
    float2 worldXZ = worldPos.xz;
    
    // 波のオフセットを計算
    float3 waveOffset = CalculateWaveOffset(worldXZ);
    
    // 元の頂点位置に波のオフセットを加算
    float3 finalPosition = float3(localX, 0.0, localZ) + waveOffset;
    
    // 法線を計算
    float3 normal = CalculateNormal(worldXZ);
    // ワールド空間に変換
    float3 worldNormal = normalize(mul(normal, (float3x3) worldMatrix));
    
    // 結果を出力バッファに書き込み
    gOutputVertices[vertexIndex].position = finalPosition;
    gOutputVertices[vertexIndex].normal = worldNormal;
    gOutputVertices[vertexIndex].worldXZ = worldXZ;
    gOutputVertices[vertexIndex].padding = 0.0;
}