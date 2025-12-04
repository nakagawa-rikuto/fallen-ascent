struct VertexShaderOutput
{
    float4 position : SV_Position;
    float2 texcood : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : POSITION0;
    float2 worldXZ : TEXCOORD1; // 波紋計算用のワールド座標
};