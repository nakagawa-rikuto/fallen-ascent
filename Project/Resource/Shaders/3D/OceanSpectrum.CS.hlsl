/// ===================================================
/// Ocean Spectrum Generation Compute Shader
/// ===================================================

#define PI 3.14159265359
#define GRAVITY 9.81

// 複素数構造体
struct Complex
{
    float real;
    float imag;
};

// Phillipsスペクトルパラメータ
cbuffer PhillipsParams : register(b0)
{
    float windSpeed;
    float2 windDirection;
    float amplitude;
    float suppression;
    float3 padding0;
};

// シミュレーション設定
cbuffer SimulationSettings : register(b1)
{
    int resolution;
    float oceanSize;
    float time;
    float choppiness;
    float tileScale;
    int cascadeCount;
    float2 padding1;
};

// 出力: 初期スペクトル h0(k)
RWTexture2D<float4> h0k : register(u0);

// 出力: 時間依存スペクトル h(k,t)
RWTexture2D<float4> hkt : register(u1);

// ガウシアン乱数生成(Box-Muller法)
float2 GaussianRandom(uint2 id)
{
    // シード生成
    uint seed = id.x * 73856093 ^ id.y * 19349663;
    
    // Xorshift乱数生成
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    float u1 = float(seed) / 4294967296.0;
    
    seed ^= seed << 13;
    seed ^= seed >> 17;
    seed ^= seed << 5;
    float u2 = float(seed) / 4294967296.0;
    
    // Box-Muller変換
    float r = sqrt(-2.0 * log(u1 + 0.0001));
    float theta = 2.0 * PI * u2;
    
    return float2(r * cos(theta), r * sin(theta));
}

// Phillips スペクトル
float PhillipsSpectrum(float2 k)
{
    float k_length = length(k);
    if (k_length < 0.0001)
        return 0.0;
    
    float k_length2 = k_length * k_length;
    float k_length4 = k_length2 * k_length2;
    
    // L = V² / g (最大波長)
    float L = (windSpeed * windSpeed) / GRAVITY;
    float L2 = L * L;
    
    // 風の方向との内積
    float2 k_normalized = k / k_length;
    float2 w_normalized = normalize(windDirection);
    float k_dot_w = dot(k_normalized, w_normalized);
    float k_dot_w2 = k_dot_w * k_dot_w;
    
    // Phillips スペクトル本体
    float phillips = amplitude * (exp(-1.0 / (k_length2 * L2)) / k_length4) * k_dot_w2;
    
    // 小さい波の抑制
    float l = L * suppression; 
    phillips *= exp(-k_length2 * l * l);
    
    return phillips;
}

// 複素数の乗算
Complex ComplexMul(Complex a, Complex b)
{
    Complex result;
    result.real = a.real * b.real - a.imag * b.imag;
    result.imag = a.real * b.imag + a.imag * b.real;
    return result;
}

// 複素数の加算
Complex ComplexAdd(Complex a, Complex b)
{
    Complex result;
    result.real = a.real + b.real;
    result.imag = a.imag + b.imag;
    return result;
}

// 共役複素数
Complex ComplexConjugate(Complex a)
{
    Complex result;
    result.real = a.real;
    result.imag = -a.imag;
    return result;
}

// 分散関係: ω = sqrt(g|k|)
float Dispersion(float k_length)
{
    return sqrt(GRAVITY * k_length);
}

/// メインエントリーポイント: 初期スペクトル h0(k) 生成
[numthreads(8, 8, 1)]
void GenerateH0(uint3 DTid : SV_DispatchThreadID)
{
    uint2 id = DTid.xy;
    if (id.x >= (uint) resolution || id.y >= (uint) resolution)
        return;
    
    // 波数ベクトル k の計算
    // k = 2π * (x - N/2) / L
    int n = int(id.x) - resolution / 2;
    int m = int(id.y) - resolution / 2;
    float2 k;
    k.x = (2.0 * PI * n) / oceanSize;
    k.y = (2.0 * PI * m) / oceanSize;
    
    // Phillips スペクトル P(k)
    float P_h = PhillipsSpectrum(k);
    
    // ガウシアン乱数 (ξr, ξi)
    float2 gaussian = GaussianRandom(id);
    
    // h0(k) = (1/√2) * (ξr + i*ξi) * √P(k)
    // = ξr * √(P/2) + i * ξi * √(P/2)
    Complex h0_k;
    h0_k.real = gaussian.x * sqrt(P_h * 0.5);
    h0_k.imag = gaussian.y * sqrt(P_h * 0.5);
    
    // h0(-k) の計算のため、-kのインデックスを求める
    uint2 minus_id;
    minus_id.x = (resolution - id.x) % resolution;
    minus_id.y = (resolution - id.y) % resolution;
    
    // 格納: (h0.real, h0.imag, 0, 0)
    //h0k[id] = float4(h0_k.real, h0_k.imag, 0, 0);
}

/// 時間依存スペクトル h(k,t) 更新
[numthreads(8, 8, 1)]
void UpdateHKT(uint3 DTid : SV_DispatchThreadID)
{
    uint2 id = DTid.xy;
    if (id.x >= (uint) resolution || id.y >= (uint) resolution)
        return;
    
    // h0(k) を読み込み
    float4 h0_data = h0k[id];
    Complex h0k_val;
    h0k_val.real = h0_data.x;
    h0k_val.imag = h0_data.y;
    
    // h0(-k) を読み込み
    uint2 minus_id;
    minus_id.x = (resolution - id.x) % resolution;
    minus_id.y = (resolution - id.y) % resolution;
    float4 h0_minus_data = h0k[minus_id];
    Complex h0minusk;
    h0minusk.real = h0_minus_data.x;
    h0minusk.imag = h0_minus_data.y;
    
    // h0*(-k) = h0(-k)の共役
    Complex h0minusk_conj = ComplexConjugate(h0minusk);
    
    // 波数ベクトル k
    int n = int(id.x) - resolution / 2;
    int m = int(id.y) - resolution / 2;
    float2 k;
    k.x = (2.0 * PI * n) / oceanSize;
    k.y = (2.0 * PI * m) / oceanSize;
    float k_length = length(k);
    
    // 角周波数 ω(k) = √(g|k|)
    float omega = Dispersion(k_length);
    float omega_t = omega * time;
    
    // オイラーの公式: e^(iθ) = cos(θ) + i*sin(θ)
    // e^(iωt)
    Complex exp_iwt;
    exp_iwt.real = cos(omega_t);
    exp_iwt.imag = sin(omega_t);
    
    // e^(-iωt)
    Complex exp_minus_iwt;
    exp_minus_iwt.real = cos(omega_t);
    exp_minus_iwt.imag = -sin(omega_t);
    
    // h(k,t) = h0(k) * e^(iωt) + h0*(-k) * e^(-iωt)
    Complex term1 = ComplexMul(h0k_val, exp_iwt);
    Complex term2 = ComplexMul(h0minusk_conj, exp_minus_iwt);
    Complex hkt_val = ComplexAdd(term1, term2);
    
    // 格納: (hkt.real, hkt.imag, k.x, k.y)
    hkt[id] = float4(hkt_val.real, hkt_val.imag, k.x, k.y);
}
