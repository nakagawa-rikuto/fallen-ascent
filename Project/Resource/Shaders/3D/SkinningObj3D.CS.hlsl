// スキニング用構造体
struct Vertex
{
    float4 position;
    float2 texcoord;
    float3 normal;
};
// 頂点の影響情報
struct VertexInfluence
{
    float4 weight;
    int4 index;
};
// スキニング情報
struct SkinningInfomation
{
    uint numVertices;
};
// SkinningObj3D.VS.hlslで作った物ト同じPalette
struct Well
{
    float4x4 skeletonSpaceMatrix;
    float4x4 skeletonSpaceInverseTransposeMatrix;
};

// SkinningObj3D.VS.hlslで作った物ト同じPalette
StructuredBuffer<Well> gMatrixPalette : register(t0);
// VertexBufferViewのstream1として利用していた入力インフルエンス
StructuredBuffer<Vertex> gInputVertices : register(t1);
// VertexBufferViewのstream2として利用していた入力インフルエンス
StructuredBuffer<VertexInfluence> gInfluences : register(t2);
// Skinning計算後の頂点データ。SkinnedVertex
RWStructuredBuffer<Vertex> gOutputVertices : register(u0);
// Skinningに関するちょっとした情報
ConstantBuffer<SkinningInfomation> gSkinningeInformation : register(b0);

// 実行スレッド数
[numthreads(1024, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    uint vertexIndex = DTid.x;
    if (vertexIndex < gSkinningeInformation.numVertices)
    {
        /// === Skinnign計算 === ///
        
        // 必要なデータをStructuredBufferから取得
        // SkinningObj3D.VS.hlslでは入力頂点として受け取っていた
        Vertex input = gInputVertices[vertexIndex];
        VertexInfluence influence = gInfluences[vertexIndex];
        
        // Skinning後の頂点を計算
        Vertex skinned;
        skinned.texcoord = input.texcoord;
        
        // 計算の方法はSkinningObj3D.VS.hlslと同じ
        // データの取得方法が変わるだけで、原理が変わるわけではない
        // 位置の変換
        skinned.position = mul(input.position, gMatrixPalette[influence.index.x].skeletonSpaceMatrix) * influence.weight.x;
        skinned.position += mul(input.position, gMatrixPalette[influence.index.y].skeletonSpaceMatrix) * influence.weight.y;
        skinned.position += mul(input.position, gMatrixPalette[influence.index.z].skeletonSpaceMatrix) * influence.weight.z;
        skinned.position += mul(input.position, gMatrixPalette[influence.index.w].skeletonSpaceMatrix) * influence.weight.w;
        skinned.position.w = 1.0f; // 確実に1を入れる
    
        // 法線の変換
        skinned.normal = mul(input.normal, (float3x3) gMatrixPalette[influence.index.x].skeletonSpaceInverseTransposeMatrix) * influence.weight.x;
        skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[influence.index.y].skeletonSpaceInverseTransposeMatrix) * influence.weight.y;
        skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[influence.index.z].skeletonSpaceInverseTransposeMatrix) * influence.weight.z;
        skinned.normal += mul(input.normal, (float3x3) gMatrixPalette[influence.index.w].skeletonSpaceInverseTransposeMatrix) * influence.weight.w;
        skinned.normal = normalize(skinned.normal); // 正規化して戻してあげる
        
        // Skinning後の頂点データを格納、書き込む
        gOutputVertices[vertexIndex] = skinned;
    }
}