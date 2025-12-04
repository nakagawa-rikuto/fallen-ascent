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
    float4 position : POSITION0;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
};

/// 頂点シェーダーのmain
VertexShaderOutput main(VertexShaderInput input)
{
    VertexShaderOutput output;
    
    // 頂点位置と法線は既にCompute Shaderで波の影響を受けた状態
    // ここでは単純に変換行列を適用するのみ
    
    // クリップ空間への変換
    output.position = mul(input.position, gTransformationMatrix.WVP);
    
    // テクスチャ座標
    output.texcood = input.texcoord;
    
    // 法線の変換（ワールド空間）
    output.normal = normalize(mul(input.normal, (float3x3) gTransformationMatrix.World));
    
    // ワールド座標
    output.worldPosition = mul(input.position, gTransformationMatrix.World).xyz;
    
    // ワールド座標のXZ（使用しない場合もあるが互換性のため）
    output.worldXZ = output.worldPosition.xz;
    
    return output;
}