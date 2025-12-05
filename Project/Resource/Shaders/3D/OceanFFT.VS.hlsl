/// ===================================================
/// Ocean FFT Vertex Shader
/// ===================================================

// 変換行列
struct TransformationMatrix
{
    float4x4 WVP;
    float4x4 World;
    float4x4 WorldInverseTranspose;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b0);

// 変位マップ
Texture2D<float4> gDisplacementMap : register(t0);

// 法線マップ
Texture2D<float4> gNormalMap : register(t1);

// サンプラー
SamplerState gSampler : register(s0);

// 頂点入力
struct VertexShaderInput
{
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

// 頂点出力
struct VertexShaderOutput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : WORLD_POSITION;
    float jacobian : JACOBIAN; // 泡の生成に使用
};

VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    // テクスチャ座標でマップをサンプリング
    float4 displacement = gDisplacementMap.SampleLevel(gSampler, input.texcoord, 0);
    float4 normalData = gNormalMap.SampleLevel(gSampler, input.texcoord, 0);
    
    // 変位をスケーリング
    float scale = 20.0;
    
    // 変位を適用
    float3 displacedPosition = input.position.xyz + displacement.xyz * scale;
    
    // ワールド座標に変換
    float4 worldPos = mul(float4(displacedPosition, 1.0), gTransformationMatrix.World);
    output.worldPosition = worldPos.xyz;
    
    // クリップ空間に変換
    output.position = mul(float4(displacedPosition, 1.0), gTransformationMatrix.WVP);
    
    // 法線をワールド空間に変換
    output.normal = normalize(mul(normalData.xyz, (float3x3) gTransformationMatrix.WorldInverseTranspose));
    
    // テクスチャ座標
    output.texcoord = input.texcoord;
    
    // Jacobian(泡の生成用)
    output.jacobian = normalData.w;
    
    return output;
}