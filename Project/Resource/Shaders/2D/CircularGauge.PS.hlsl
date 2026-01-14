#include "Obj2D.hlsli"

// ゲージパラメータ
struct GaugeParams
{
    float progress; // 0.0 ~ 1.0 の進行度
    float innerRadius; // 内側の半径（ドーナツ型にする）
    float smoothness; // エッジのスムーズさ
    int gaugeType; // 0: クールダウン（時計回り）, 1: ゲージ（反時計回り）
    float4 fillColor; // 塗りつぶし色
    float4 backgroundColor; // 背景色
    float4 edgeColor; // エッジ色
    float edgeThickness; // エッジの太さ
    float padding[3]; // パディング
};

ConstantBuffer<GaugeParams> gGaugeParams : register(b0); // b0はマテリアル用なのでb1に

Texture2D<float4> gTexture : register(t0);
SamplerState gSampler : register(s0);

// ピクセルシェーダー出力
struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

// 円周率
static const float PI = 3.14159265359;

// メインピクセルシェーダー関数
PixelShaderOutput main(VertexShaderOutput input)
{
    PixelShaderOutput output;
    
    // UV座標を中心基準の座標に変換 (-1 ~ 1)
    float2 centered = input.texcoord * 2.0 - 1.0;
    
    // 距離を計算
    float dist = length(centered);
    
    // 角度を計算（上から時計回り:  0°が上）
    float angle = atan2(centered.x, -centered.y);
    angle = (angle + PI) / (2.0 * PI); // 0~1に正規化
    
    // 円の外側をカット（アンチエイリアス付き）
    float outerCircle = 1.0 - smoothstep(1.0 - gGaugeParams.smoothness, 1.0, dist);
    
    // 円の内側をカット（ドーナツ型）
    float innerCircle = smoothstep(gGaugeParams.innerRadius - gGaugeParams.smoothness,
                                    gGaugeParams.innerRadius, dist);
    
    // ドーナツマスク
    float donutMask = outerCircle * innerCircle;
    
    // プログレスに基づいて色を決定
    float isFilled = 0.0;
    if (gGaugeParams.gaugeType == 0)
    {
        // クールダウンタイプ（時計回りに塗りつぶし）
        isFilled = step(angle, gGaugeParams.progress);
    }
    else
    {
        // ゲージタイプ（反時計回りに塗りつぶし）
        isFilled = step(1.0 - angle, gGaugeParams.progress);
    }
    
    // 色の決定
    float4 gaugeColor = lerp(gGaugeParams.backgroundColor, gGaugeParams.fillColor, isFilled);
    
    // エッジの計算（プログレス境界部分）
    float edgeDist = abs(angle - gGaugeParams.progress);
    if (gGaugeParams.gaugeType == 1)
    {
        edgeDist = abs((1.0 - angle) - gGaugeParams.progress);
    }
    
    // エッジを滑らかに表示
    float edgeMask = 1.0 - smoothstep(0.0, gGaugeParams.edgeThickness, edgeDist);
    gaugeColor = lerp(gaugeColor, gGaugeParams.edgeColor, edgeMask * isFilled);
    
    // アイコンテクスチャをサンプリング
    float4 iconColor = gTexture.Sample(gSampler, input.texcoord);
    
    // 中央部分はアイコンを表示（innerRadius内）
    float centerMask = 1.0 - innerCircle;
    output.color = lerp(gaugeColor * donutMask, iconColor, centerMask);
    
    // 最終的なアルファ値
    output.color.a *= (donutMask + centerMask * iconColor.a);
    
    // アルファが0なら破棄
    if (output.color.a < 0.01)
    {
        discard;
    }
    
    return output;
}