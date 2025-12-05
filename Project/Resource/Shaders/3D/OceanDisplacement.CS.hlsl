/// ===================================================
/// Ocean Displacement and Normal Calculation
/// ===================================================

#define PI 3.14159265359

// シミュレーション設定
cbuffer SimulationSettings : register(b0)
{
    int resolution;
    float oceanSize;
    float time;
    float choppiness;
    float tileScale;
    int cascadeCount;
    float2 padding;
};

// FFT結果 h(k,t) 
Texture2D<float4> hktTexture : register(t0);

// 出力: 変位マップ (dx, dy, dz)
RWTexture2D<float4> displacementMap : register(u0);

// 出力: 法線マップ
RWTexture2D<float4> normalMap : register(u1);

/// 変位ベクトル計算
[numthreads(8, 8, 1)]
void CalculateDisplacement(uint3 DTid : SV_DispatchThreadID)
{
    uint2 id = DTid.xy;
    if (id.x >= resolution || id.y >= resolution)
        return;
    
    // FFT結果を読み込み(実部 = 高さフィールド)
    float4 hkt = hktTexture[id];
    float height = hkt.x; // 実部 = 高さ
    float2 k = hkt.zw; // 波数ベクトル
    
    float k_length = length(k);
    
    // 変位ベクトルの計算
    float3 displacement = float3(0, 0, 0);
    
    if (k_length > 0.0001)
    {
        // 水平変位 (Choppiness を考慮)
        float2 k_normalized = k / k_length;
        displacement.x = -k_normalized.x * height * choppiness;
        displacement.z = -k_normalized.y * height * choppiness;
    }
    
    // 垂直変位
    displacement.y = height;
    
    // 変位マップに格納
    displacementMap[id] = float4(displacement, 1.0);
}

/// 法線とJacobian計算
[numthreads(8, 8, 1)]
void CalculateNormals(uint3 DTid : SV_DispatchThreadID)
{
    uint2 id = DTid.xy;
    if (id.x >= resolution || id.y >= resolution)
        return;
    
    // 隣接ピクセルのインデックス
    uint2 idLeft = uint2((id.x - 1 + resolution) % resolution, id.y);
    uint2 idRight = uint2((id.x + 1) % resolution, id.y);
    uint2 idDown = uint2(id.x, (id.y - 1 + resolution) % resolution);
    uint2 idUp = uint2(id.x, (id.y + 1) % resolution);
    
    // 変位を読み込み
    float3 center = displacementMap[id].xyz;
    float3 left = displacementMap[idLeft].xyz;
    float3 right = displacementMap[idRight].xyz;
    float3 down = displacementMap[idDown].xyz;
    float3 up = displacementMap[idUp].xyz;
    
    // グリッド間隔
    float dx = oceanSize / float(resolution);
    
    // 接線ベクトル
    float3 tangentX = float3(2.0 * dx, right.y - left.y, 0.0);
    float3 tangentZ = float3(0.0, up.y - down.y, 2.0 * dx);
    
    // 法線(外積)
    float3 normal = normalize(cross(tangentZ, tangentX));
    
    // Jacobian計算(泡の生成に使用)
    float Jxx = 1.0 + (right.x - left.x) / (2.0 * dx);
    float Jzz = 1.0 + (up.z - down.z) / (2.0 * dx);
    float Jxz = (right.z - left.z) / (2.0 * dx);
    float Jzx = (up.x - down.x) / (2.0 * dx);
    
    float jacobian = Jxx * Jzz - Jxz * Jzx;
    
    // 法線マップに格納 (xyz: 法線, w: Jacobian)
    normalMap[id] = float4(normal, jacobian);
}

/// メインエントリーポイント：統合処理(変位と法線を一度に計算)
[numthreads(8, 8, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint2 id = DTid.xy;
    if (id.x >= resolution || id.y >= resolution)
        return;
    
    // 変位を計算
    float4 hkt = hktTexture[id];
    float height = hkt.x;
    float2 k = hkt.zw;
    float k_length = length(k);
    
    float3 displacement = float3(0, height, 0);
    if (k_length > 0.0001)
    {
        float2 k_normalized = k / k_length;
        displacement.x = -k_normalized.x * height * choppiness;
        displacement.z = -k_normalized.y * height * choppiness;
    }
    
    displacementMap[id] = float4(displacement, 1.0);
    
    // 法線を計算
    uint2 idLeft = uint2((id.x - 1 + resolution) % resolution, id.y);
    uint2 idRight = uint2((id.x + 1) % resolution, id.y);
    uint2 idDown = uint2(id.x, (id.y - 1 + resolution) % resolution);
    uint2 idUp = uint2(id.x, (id.y + 1) % resolution);
    
    float3 left = displacementMap[idLeft].xyz;
    float3 right = displacementMap[idRight].xyz;
    float3 down = displacementMap[idDown].xyz;
    float3 up = displacementMap[idUp].xyz;
    
    float dx = oceanSize / float(resolution);
    float3 tangentX = float3(2.0 * dx, right.y - left.y, 0.0);
    float3 tangentZ = float3(0.0, up.y - down.y, 2.0 * dx);
    float3 normal = normalize(cross(tangentZ, tangentX));
    
    // Jacobian
    float Jxx = 1.0 + (right.x - left.x) / (2.0 * dx);
    float Jzz = 1.0 + (up.z - down.z) / (2.0 * dx);
    float Jxz = (right.z - left.z) / (2.0 * dx);
    float Jzx = (up.x - down.x) / (2.0 * dx);
    float jacobian = Jxx * Jzz - Jxz * Jzx;
    
    normalMap[id] = float4(normal, jacobian);
}