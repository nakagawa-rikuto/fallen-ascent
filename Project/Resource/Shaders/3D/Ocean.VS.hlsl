#include "Ocean.hlsli"

/// ===================================================
/// VertexShader 
/// ===================================================

// 変換行列の構造体
struct TransformationMatrix
{
    float4x4 WVP;
    float4x4 World;
};

ConstantBuffer<TransformationMatrix> gTransformationMatrix : register(b1);

// 頂点シェーダーへの入力
struct VertexShaderInput
{
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    float2 worldXZ : TEXCOORD0;
};

/// 頂点シェーダーのmain
VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    // クリップ空間への変換
    output.position = mul(float4(input.position, 1.0), gTransformationMatrix.WVP);
    
    // テクスチャ座標
    output.texcood = input.worldXZ;
    
    // 法線の変換（ワールド空間）
    output.normal = normalize(mul(input.normal, (float3x3) gTransformationMatrix.World));
    
    // ワールド座標
    output.worldPosition = mul(float4(input.position, 1.0), gTransformationMatrix.World).xyz;
    
    // ワールド座標のXZ（使用しない場合もあるが互換性のため）
    output.worldXZ = output.worldXZ;
    
    return output;
}