#include "Fullscreen.hlsli"

Texture2D<float4> gBackgroundTexture : register(t0); // 背景テクスチャ（割れる前）
Texture2D<float4> gGlassTexture : register(t1); // ガラステクスチャ（割れる）
SamplerState gSampler : register(s0);

cbuffer ShatterParams : register(b0)
{
    float progress; // 割れる進行度 (0.0 ~ 1.0)
    float impactX; // 衝撃点のX座標
    float impactY; // 衝撃点のY座標
    float crackDensity; // ひび割れの密度
    float dispersion; // 破片の飛散度
    float rotation; // 破片の回転量
    float fadeOut; // フェードアウト効果
    float randomSeed; // ランダムシード値
};

// ハッシュ関数
float hash(float2 p)
{
    float3 p3 = frac(float3(p.xyx) * 0.1031);
    p3 += dot(p3, p3.yzx + 33.33);
    return frac((p3.x + p3.y) * p3.z + randomSeed * 0.01);
}

// Voronoiノイズ（セルIDと最近接点の距離を返す）
float3 voronoi(float2 p)
{
    float2 n = floor(p);
    float2 f = frac(p);
    
    float minDist = 8.0;
    float2 minPoint = float2(0.0, 0.0);
    float2 minCell = float2(0.0, 0.0);
    
    // 3x3の近傍セルを探索
    for (int j = -1; j <= 1; j++)
    {
        for (int i = -1; i <= 1; i++)
        {
            float2 neighbor = float2(float(i), float(j));
            float2 cellPos = n + neighbor;
            
            // セル内のランダムな点
            float2 pos = float2(
                hash(cellPos),
                hash(cellPos + float2(10.5, 20.3))
            );
            
            float2 diff = neighbor + pos
            -f;
            float dist = length(diff);
            
            if (dist < minDist)
            {
                minDist = dist;
                minPoint = pos;
                minCell = cellPos;
            }
        }
    }
    
    // セルID（ユニークな値）
    float cellId = hash(minCell);
    
    return float3(minDist, cellId, hash(minCell + float2(5.5, 15.3)));
}

// 2D回転
float2 rotate(float2 p, float angle)
{
    float c = cos(angle);
    float s = sin(angle);
    return float2(p.x * c - p.y * s, p.x * s + p.y * c);
}

float4 main(VertexShaderOutPut input) : SV_TARGET
{
    float2 uv = input.texcoord;
    
    // 背景テクスチャをサンプリング（常に必要）
    float4 bgColor = gBackgroundTexture.Sample(gSampler, uv);
    
    // progressが0の時は背景テクスチャをそのまま返す
    if (progress <= 0.0)
    {
        return bgColor;
    }
    
    float2 impactPoint = float2(impactX, impactY);
    
    // 衝撃点からの距離と方向
    float2 toCenter = uv - impactPoint;
    float dist = length(toCenter);
    float2 dir = normalize(toCenter + float2(0.0001, 0.0001));
    
    // Voronoiでセル情報を取得
    float3 voronoiResult = voronoi(uv * crackDensity);
    float cellDist = voronoiResult.x;
    float cellId = voronoiResult.y;
    float cellId2 = voronoiResult.z;
    
    // 衝撃点からの距離による進行度の遅延（波紋効果）
    float distanceDelay = dist * 0.5;
    float localProgress = saturate((progress - distanceDelay) * 2.0);
    
    // 破片ごとの遅延（バラバラに割れる）
    float shardDelay = cellId * 0.3;
    float shardProgress = saturate((localProgress - shardDelay) * 3.0);
    shardProgress = smoothstep(0.0, 1.0, shardProgress);
    
    // 破片が飛散し始めるタイミング
    float flyProgress = smoothstep(0.2, 1.0, shardProgress);
    
    // 破片ごとのランダムな方向
    float2 randomDir = float2(
        hash(float2(cellId, 0.0)) - 0.5,
        hash(float2(cellId, 1.0)) - 0.5
    );
    randomDir = normalize(randomDir + dir * 0.5);
    
    // 重力効果
    float gravity = flyProgress * flyProgress * 0.3;
    
    // 破片の移動
    float2 displacement = randomDir * flyProgress * dispersion * 0.5;
    displacement.y += gravity;
    
    // 破片の回転（各破片が独立して回転）
    float rotationAmount = (cellId2 - 0.5) * rotation * flyProgress * 6.28;
    
    // セルの中心を計算（回転の中心点）
    float2 cellCenter = floor(uv * crackDensity) / crackDensity + 0.5 / crackDensity;
    
    // セル中心を基準に回転
    float2 centeredUV = uv - cellCenter;
    centeredUV = rotate(centeredUV, rotationAmount);
    float2 rotatedUV = centeredUV + cellCenter;
    
    // 最終的なUV座標
    float2 finalUV = rotatedUV + displacement;
    
    // UV範囲外チェック（先に行う）
    bool outOfBounds = finalUV.x < 0.0 || finalUV.x > 1.0 || finalUV.y < 0.0 || finalUV.y > 1.0;
    
    // UV範囲外なら背景を返す
    if (outOfBounds)
    {
        return bgColor;
    }
    
    // ひび割れのエッジ検出
    float crackThreshold = 0.02 / crackDensity;
    float crackEdge = smoothstep(0.0, crackThreshold, cellDist / crackDensity);
    
    // ガラステクスチャをサンプリング（範囲内のみ）
    float4 glassColor = gGlassTexture.Sample(gSampler, finalUV);
    
    // ひび割れの線（黒い線）
    float crackLine = 1.0 - smoothstep(0.0, crackThreshold * 0.5, cellDist / crackDensity);
    float crackVisibility = saturate(1.0 - flyProgress * 2.0) * smoothstep(0.0, 0.3, localProgress);
    glassColor.rgb = lerp(glassColor.rgb, glassColor.rgb * 0.1, crackLine * crackVisibility);
    
    // 破片のエッジを暗くする
    float edgeDarkness = lerp(1.0, 0.7, (1.0 - crackEdge) * flyProgress);
    glassColor.rgb *= edgeDarkness;
    
    // ガラスの反射ハイライト
    float highlight = pow(1.0 - cellDist / crackDensity, 4.0) * 0.2 * (1.0 - flyProgress * 0.7);
    glassColor.rgb += highlight * smoothstep(0.1, 0.3, localProgress);
    
    // フェードアウト効果
    float fadeAmount = fadeOut * smoothstep(0.3, 1.0, flyProgress);
    glassColor.a *= 1.0 - fadeAmount;
    
    // 破片が飛散した後は透明に
    glassColor.a *= 1.0 - smoothstep(0.7, 1.0, flyProgress);
    
    // アルファが非常に小さい場合は背景を返す
    if (glassColor.a < 0.01)
    {
        return bgColor;
    }
    
    // 背景とガラスをアルファブレンド
    float4 finalColor = lerp(bgColor, glassColor, glassColor.a);
    
    return finalColor;
}