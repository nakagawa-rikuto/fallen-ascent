#include "Ocean.hlsli"

/// ===================================================
/// PixelShader
/// ===================================================
struct Material
{
    float4 color;
    int enableLighting;
    float shininess; // 光沢度
    float transparency; // 透明度
    float refractionIndex; // 屈折率
};

struct DirectionalLight
{
    float4 color;
    float3 direction;
    float intensity;
};

struct Color
{
    float3 seaBease; // 基本の海の色
    float padding1;
    float3 seaShallow; // 浅瀬の色
    float padding2;
    float3 sky; // 空の色
    float padding3;
    float3 deepWater; // 深い水の色
    float beseStrength; // 色の強度
    float hightOffset; // 波の高さに基づいた色のオフセット
    float waterClarity; // 水の透明度
    float foamThreshold; // 泡の閾値
};

struct Camera
{
    float3 worldPosition;
    float padding;
};

ConstantBuffer<Material> gMaterial : register(b0);
ConstantBuffer<DirectionalLight> gDirectionalLight : register(b2);
ConstantBuffer<Camera> gCamera : register(b3);
ConstantBuffer<Color> gColor : register(b4);

struct PixlShaderOutput
{
    float4 color : SV_TARGET0;
};

// 空の色を計算する関数
float3 GetSkyColor(float3 dir, float3 c)
{
    dir.y = max(0.0, dir.y);
    float et = 1.0 - dir.y;
    return (1.0 - c) * et + c;
}

// シュリックのフレネル近似（より正確な計算）
float FresnelSchlick(float cosTheta, float F0)
{
    return F0 + (1.0 - F0) * pow(1.0 - cosTheta, 5.0);
}

// 水面の泡を計算
float CalculateFoam(float3 normal, float waveHeight)
{
    // 法線のY成分が小さい（波が急な）ところで泡が発生
    float steepness = 1.0 - abs(normal.y);
    float foam = pow(steepness, 3.0) * saturate(waveHeight * 2.0);
    return foam;
}

/// ピクセルシェーダーのmain
PixlShaderOutput main(VertexShaderOutput input)
{
    PixlShaderOutput output;
    
    if (gMaterial.enableLighting != 0)
    {
        float3 normal = normalize(input.normal);
        
        // 法線が不正な場合のフォールバック
        if (length(normal) < 0.1)
        {
            normal = float3(0.0, 1.0, 0.0);
        }
        
        float3 viewDir = normalize(gCamera.worldPosition - input.worldPosition);
        float3 reflectDir = reflect(-viewDir, normal);
        
        // ライトの反射ベクトル
        float3 halfVector = normalize(-gDirectionalLight.direction + viewDir);
        
        // 拡散反射の計算
        float NdotL = saturate(dot(normal, -gDirectionalLight.direction));
        float diffuseStrength = pow(NdotL * 0.5 + 0.5, 2.0);
        
        // 拡散反射
        float3 diffuse = gColor.seaBease * gDirectionalLight.color.rgb *
                        diffuseStrength * gDirectionalLight.intensity;
        
        // アンビエント光
        float3 ambient = gColor.seaBease * 0.3;
        diffuse += ambient;
        
        // 鏡面反射
        float NdotH = saturate(dot(normal, halfVector));
        float specularStrength = pow(NdotH, gMaterial.shininess);
        float3 specular = gDirectionalLight.color.rgb * specularStrength *
                         gDirectionalLight.intensity * 1.5;
        
        // フレネル効果
        float NdotV = saturate(dot(normal, viewDir));
        float fresnel = FresnelSchlick(NdotV, 0.02);
        
        // 空の反射色
        float3 skyReflection = GetSkyColor(reflectDir, gColor.sky);
        
        // 波の高さ
        float waveHeight = input.worldPosition.y;
        
        // 深度による色の変化
        float depth = saturate((10.0 - input.worldPosition.y) / 20.0);
        
        // 3段階の色補間
        float3 waterColor;
        if (depth < 0.3)
        {
            waterColor = lerp(gColor.seaShallow, gColor.seaBease, depth / 0.3);
        }
        else if (depth < 0.7)
        {
            waterColor = gColor.seaBease;
        }
        else
        {
            waterColor = lerp(gColor.seaBease, gColor.deepWater, (depth - 0.7) / 0.3);
        }
        
        // 透明感を考慮した基本色
        float3 baseColor = waterColor * diffuse * gColor.beseStrength;
        
        // 浅瀬の効果
        float shallowEffect = saturate(waveHeight * 0.5 + 0.5);
        float3 shallowColor = gColor.seaShallow * shallowEffect * 0.2;
        
        // 泡の計算
        float foam = CalculateFoam(normal, waveHeight);
        float3 foamColor = float3(1.0, 1.0, 1.0) * foam * gColor.foamThreshold;
        
        // 屈折による色の歪み効果
        float refractionEffect = (1.0 - fresnel) * gColor.waterClarity;
        
        // 水の色の最終合成
        float3 underwaterColor = baseColor + shallowColor;
        float3 reflectionColor = skyReflection * 0.3 + specular;
        
        // フレネル効果で反射と屈折をブレンド
        float3 waterSurface = lerp(underwaterColor, reflectionColor, fresnel * 0.5);
        
        // 透明感の追加
        waterSurface = lerp(waterSurface, underwaterColor * 1.1, refractionEffect);
        
        // 泡を加算
        float3 finalColor = waterSurface + foamColor;
        
        // サブサーフェス・スキャタリング
        float subsurface = pow(saturate(dot(-viewDir, gDirectionalLight.direction)), 3.0);
        float3 subsurfaceColor = gColor.seaBease * subsurface * 0.2 * gDirectionalLight.intensity;
        finalColor += subsurfaceColor;
        
        // トーンマッピングとガンマ補正
        finalColor = finalColor / (finalColor + 1.0);
        finalColor = pow(finalColor, 1.0 / 2.2);
        
        output.color.rgb = finalColor;
        
        // アルファ値の計算（透明度）
        // 角度が浅い（真上から見る）ほど透明に、斜めから見るほど不透明に
        float transparency = lerp(0.4, 0.9, fresnel); // 0.4～0.9の範囲
        
        // 水の透明度パラメータを適用
        transparency = lerp(transparency, 1.0, 1.0 - gColor.waterClarity);
        
        output.color.a = transparency;
    }
    else
    {
        output.color = gMaterial.color;
    }
    
    return output;
}