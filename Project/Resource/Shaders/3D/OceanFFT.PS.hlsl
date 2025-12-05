/// ===================================================
/// Ocean FFT Pixel Shader
/// ===================================================

struct Material
{
    float4 color;
    int enableLighting;
    float shininess;
    float transparency;
    float refractionIndex;
};

struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};

struct Camera
{
    float3 worldPosition;
    float padding;
};

struct OceanColor
{
    float3 seaBase;
    float padding1;
    float3 seaShallow;
    float padding2;
    float3 sky;
    float padding3;
    float3 deepWater;
    float baseStrength;
    float waterClarity;
    float foamThreshold;
    float foamScale;
    float padding4;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b1);
ConstantBuffer<Camera> gCamera : register(b2);
ConstantBuffer<OceanColor> gOceanColor : register(b3);

struct PixelShaderInput
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 worldPosition : WORLD_POSITION;
    float jacobian : JACOBIAN;
};

struct PixelShaderOutput
{
    float4 color : SV_TARGET0;
};

// フレネル項(Schlick近似)
float FresnelSchlick(float cosTheta, float F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - saturate(cosTheta), 5.0);
}

// 空の色
float3 GetSkyColor(float3 reflectDir)
{
    float t = 0.5 * (reflectDir.y + 1.0);
    return lerp(float3(1.0, 1.0, 1.0), gOceanColor.sky, t);
}

// 泡の計算(Jacobianベース)
float CalculateFoam(float jacobian, float waveHeight)
{
    // Jacobianが負の領域で泡が発生
    float foam = saturate(-jacobian * gOceanColor.foamThreshold);
    
    // 波の高さによる調整
    foam *= saturate(waveHeight * 0.5 + 0.5);
    
    return pow(foam, 0.7) * gOceanColor.foamScale;
}

// サブサーフェス・スキャタリング(簡易版)
float3 CalculateSSS(float3 lightDir, float3 viewDir, float3 normal)
{
    float3 H = normalize(lightDir + normal * 0.5);
    float VdotH = saturate(dot(viewDir, -H));
    float sss = pow(VdotH, 4.0);
    return gOceanColor.seaBase * sss * 0.3;
}

PixelShaderOutput main(PixelShaderInput input)
{
    PixelShaderOutput output;
    
    // 法線の正規化
    float3 N = normalize(input.normal);
    float3 V = normalize(gCamera.worldPosition - input.worldPosition);
    float3 L = normalize(-gDirectionalLight.direction);
    float3 R = reflect(-V, N);
    
    // Lambertian拡散反射
    float NdotL = saturate(dot(N, L));
    float3 diffuse = gOceanColor.seaBase * NdotL;
    
    // アンビエント
    float3 ambient = gOceanColor.seaBase * 0.2;
    
    // Blinn-Phong鏡面反射
    float3 H = normalize(L + V);
    float NdotH = saturate(dot(N, H));
    float spec = pow(NdotH, gMaterial.shininess);
    float3 specular = gDirectionalLight.color.rgb * spec * 0.8;
    
    // フレネル効果
    float NdotV = saturate(dot(N, V));
    float fresnel = FresnelSchlick(NdotV, 0.02);
    
    // 空の反射
    float3 skyReflection = GetSkyColor(R);
    
    // 深度による色の変化
    float depth = saturate(input.worldPosition.y / 10.0);
    float3 waterColor = lerp(gOceanColor.deepWater, gOceanColor.seaBase, depth);
    waterColor = lerp(waterColor, gOceanColor.seaShallow, saturate(depth * 2.0));
    
    // 基本色の合成
    float3 baseColor = waterColor * (diffuse + ambient) * gOceanColor.baseStrength;
    
    // 反射色
    float3 reflectionColor = skyReflection * 0.4 + specular;
    
    // フレネルで反射と屈折をブレンド
    float3 finalColor = lerp(baseColor, reflectionColor, fresnel * 0.6);
    
    // サブサーフェス・スキャタリング
    float3 sss = CalculateSSS(L, V, N);
    finalColor += sss * gDirectionalLight.intensity;
    
    // 泡の追加
    float foam = CalculateFoam(input.jacobian, input.worldPosition.y);
    finalColor = lerp(finalColor, float3(1.0, 1.0, 1.0), foam);
    
    // トーンマッピング
    finalColor = finalColor / (finalColor + 1.0);
    
    // ガンマ補正
    finalColor = pow(finalColor, 1.0 / 2.2);
    
    // 透明度(フレネルベース)
    float alpha = lerp(0.5, 0.95, fresnel);
    alpha = lerp(alpha, 1.0, 1.0 - gOceanColor.waterClarity);
    
    output.color = float4(finalColor, alpha);
    
    return output;
}