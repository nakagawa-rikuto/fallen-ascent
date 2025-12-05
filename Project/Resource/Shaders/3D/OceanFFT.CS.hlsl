/// ===================================================
/// Ocean FFT Compute Shader
/// ===================================================

#define PI 3.14159265359

// 複素数構造体
struct Complex
{
    float real;
    float imag;
};

// FFTパラメータ
cbuffer FFTParams : register(b0)
{
    int stage; // 現在のFFTステージ
    int pingPong; // 0 or 1 (ping-pong buffer)
    int direction; // 0: horizontal, 1: vertical
    int resolution;
    float4 padding;
};

// 入力テクスチャ
Texture2D<float4> inputTexture : register(t0);

// 出力テクスチャ
RWTexture2D<float4> outputTexture : register(u0);

// 複素数の加算
Complex ComplexAdd(Complex a, Complex b)
{
    Complex result;
    result.real = a.real + b.real;
    result.imag = a.imag + b.imag;
    return result;
}

// 複素数の減算
Complex ComplexSub(Complex a, Complex b)
{
    Complex result;
    result.real = a.real - b.real;
    result.imag = a.imag - b.imag;
    return result;
}

// 複素数の乗算
Complex ComplexMul(Complex a, Complex b)
{
    Complex result;
    result.real = a.real * b.real - a.imag * b.imag;
    result.imag = a.real * b.imag + a.imag * b.real;
    return result;
}

// 回転因子 W_N^k = e^(-2πik/N)
Complex TwiddleFactor(int k, int N)
{
    float angle = -2.0 * PI * float(k) / float(N);
    Complex w;
    w.real = cos(angle);
    w.imag = sin(angle);
    return w;
}

/// Horizontal FFT (X方向)
[numthreads(1, 8, 1)]
void HorizontalFFT(uint3 DTid : SV_DispatchThreadID)
{
    uint y = DTid.y;
    if (y >= uint(resolution))
        return;
    
    int numStages = int(log2(float(resolution)));
    int butterflySpan = 1U << uint(stage);
    int butterflyWidth = 2 * butterflySpan;
    
    for (uint x = 0; x < uint(resolution); x++)
    {
        int butterflyIndex = int(x) / butterflyWidth;
        int indexInButterfly = int(x) % butterflyWidth;
        
        bool topWing = indexInButterfly < butterflySpan;
        
        uint x1 = uint(butterflyIndex * butterflyWidth + indexInButterfly);
        uint x2 = topWing ? x1 + uint(butterflySpan) : x1 - uint(butterflySpan);
        
        // 入力データ読み込み
        float4 data1 = inputTexture[uint2(x1, y)];
        float4 data2 = inputTexture[uint2(x2, y)];
        
        Complex h1, h2;
        h1.real = data1.x;
        h1.imag = data1.y;
        h2.real = data2.x;
        h2.imag = data2.y;
        
        // 回転因子
        int k = topWing ? indexInButterfly : indexInButterfly - butterflySpan;
        Complex w = TwiddleFactor(k, butterflyWidth);
        
        // Butterfly演算
        Complex h2w = ComplexMul(h2, w);
        Complex result;
        if (topWing)
        {
            result = ComplexAdd(h1, h2w);
        }
        else
        {
            result = ComplexSub(h1, h2w);
        }
        
        // ビットリバース(最終ステージのみ)
        uint outputX = x;
        if (stage == numStages - 1)
        {
            // ビットリバース処理
            outputX = 0;
            uint temp = x;
            for (int i = 0; i < numStages; i++)
            {
                outputX = (outputX << 1U) | (temp & 1U);
                temp >>= 1U;
            }
        }
        
        outputTexture[uint2(outputX, y)] = float4(result.real, result.imag, data1.z, data1.w);
    }
}

/// Vertical FFT (Y方向)
[numthreads(8, 1, 1)]
void VerticalFFT(uint3 DTid : SV_DispatchThreadID)
{
    uint x = DTid.x;
    if (x >= uint(resolution))
        return;
    
    int numStages = int(log2(float(resolution)));
    int butterflySpan = 1U << uint(stage);
    int butterflyWidth = 2 * butterflySpan;
    
    for (uint y = 0; y < uint(resolution); y++)
    {
        int butterflyIndex = int(y) / butterflyWidth;
        int indexInButterfly = int(y) % butterflyWidth;
        
        bool topWing = indexInButterfly < butterflySpan;
        
        uint y1 = uint(butterflyIndex * butterflyWidth + indexInButterfly);
        uint y2 = topWing ? y1 + uint(butterflySpan) : y1 - uint(butterflySpan);
        
        // 入力データ読み込み
        float4 data1 = inputTexture[uint2(x, y1)];
        float4 data2 = inputTexture[uint2(x, y2)];
        
        Complex h1, h2;
        h1.real = data1.x;
        h1.imag = data1.y;
        h2.real = data2.x;
        h2.imag = data2.y;
        
        // 回転因子
        int k = topWing ? indexInButterfly : indexInButterfly - butterflySpan;
        Complex w = TwiddleFactor(k, butterflyWidth);
        
        // Butterfly演算
        Complex h2w = ComplexMul(h2, w);
        Complex result;
        if (topWing)
        {
            result = ComplexAdd(h1, h2w);
        }
        else
        {
            result = ComplexSub(h1, h2w);
        }
        
        // ビットリバース(最終ステージのみ)
        uint outputY = y;
        if (stage == numStages - 1)
        {
            outputY = 0;
            uint temp = y;
            for (int i = 0; i < numStages; i++)
            {
                outputY = (outputY << 1U) | (temp & 1U);
                temp >>= 1U;
            }
        }
        
        // 正規化(IFFT用)
        float scale = 1.0;
        if (stage == numStages - 1)
        {
            scale = 1.0 / float(resolution * resolution);
        }
        
        outputTexture[uint2(x, outputY)] = float4(
            result.real * scale,
            result.imag * scale,
            data1.z,
            data1.w
        );
    }
}

/// メインエントリーポイント
[numthreads(8, 8, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (direction == 0)
    {
        HorizontalFFT(DTid);
    }
    else
    {
        VerticalFFT(DTid);
    }
}