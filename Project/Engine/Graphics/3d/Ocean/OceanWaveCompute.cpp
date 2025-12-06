#include "OceanWaveCompute.h"
// C++
#include <cassert>
// Service
#include "Engine/System/Service/ServiceLocator.h"
#include "Engine/System/Managers/SRVManager.h"
// Math
#include "Math/sMath.h"
#include "Math/MatrixMath.h"


///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
OceanWaveCompute::~OceanWaveCompute() {
    waveInfoBuffer_.reset();
    rippleBuffer_.reset();
    settingsBuffer_.reset();
    outputUAV_.reset();
    outputBuffer_.Reset();
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
// UAVのIndex取得
uint32_t OceanWaveCompute::GetUAVIndex() const { return uavIndex_; }
// SRVのIndex取得
uint32_t OceanWaveCompute::GetSRVIndex() const { return srvIndex_; }
// 出力バッファリソースの取得
ID3D12Resource* OceanWaveCompute::GetOutputBuffer() const { return outputBuffer_.Get(); }


///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void OceanWaveCompute::Initialize(ID3D12Device* device, int gridSize) {
    gridSize_ = gridSize;
    vertexCount_ = (gridSize_ + 1) * (gridSize_ + 1);

    // SRVManagerを取得
    srvManager_ = ServiceLocator::GetSRVManager();

    // === 波情報バッファ（12個の波）StructuredBuffer === //
    waveInfoBuffer_ = std::make_unique<BufferBase>();
    waveInfoBuffer_->Create(device, sizeof(OceanShaderInfo) * 12);
	// マッピング
    waveInfoBuffer_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&waveInfoData_));

    // デフォルト値で初期化
    for (int i = 0; i < kWaveCount_; ++i) {
        waveInfoData_[i].distance = { 0.0f, 0.0f, 0.0f };
        waveInfoData_[i].amplitude = 0.0f;
        waveInfoData_[i].length = 10.0f;
        waveInfoData_[i].speed = 2.0f;
        waveInfoData_[i].time = 0.0f;
        waveInfoData_[i].padding = 0.0f;
    }

    // === 波紋バッファ（ConstantBuffer） === //
    rippleBuffer_ = std::make_unique<BufferBase>();
    rippleBuffer_->Create(device, sizeof(RippleBufferForGPU));
	// マッピング
    rippleBuffer_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&rippleBufferData_));

    // === 設定バッファ（ConstantBuffer） === //
    settingsBuffer_ = std::make_unique<BufferBase>();
    settingsBuffer_->Create(device, sizeof(WaveSettings));
	// マッピング
    settingsBuffer_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&settingsData_));
	// 設定値の初期化
    settingsData_->gridSize = gridSize_;
    settingsData_->gridWidth = 100.0f;
    settingsData_->gridDepth = 100.0f;
    settingsData_->normalEpsilon = 0.1f;
    settingsData_->worldMatrix = Math::MakeIdentity4x4();
    settingsData_->worldOffset = { 0.0f, 0.0f, 0.0f };
    settingsData_->padding1 = 0.0f;

    // === 出力バッファ（UAV用）=== //
	CreateUAVBuffer(device);

    // === SRVとUAVの作成 === //
    CreateViews(device, srvManager_);
}

///-------------------------------------------/// 
/// 波の計算を実行
///-------------------------------------------///
void OceanWaveCompute::Dispatch(ID3D12GraphicsCommandList* commandList) {

    // OceanWaveCompute::Dispatch を呼び出す前に追加
    ID3D12DescriptorHeap* descriptorHeaps[] = {
        srvManager_->GetDescriptorHeap()  // SRVManagerからヒープを取得
    };
    commandList->SetDescriptorHeaps(1, descriptorHeaps);
        
    // COMMON -> UNORDERED_ACCESS への遷移バリア
    D3D12_RESOURCE_BARRIER barrierToUAV = {};
    barrierToUAV.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrierToUAV.Transition.pResource = outputBuffer_.Get();
    barrierToUAV.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
    barrierToUAV.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    barrierToUAV.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList->ResourceBarrier(1, &barrierToUAV);

    // [0] CBV - 設定バッファ (b0)
    commandList->SetComputeRootConstantBufferView(0, settingsBuffer_->GetBuffer()->GetGPUVirtualAddress());

    // [1] CBV - 波紋バッファ (b1)
    commandList->SetComputeRootConstantBufferView(1, rippleBuffer_->GetBuffer()->GetGPUVirtualAddress());

    // [2] DescriptorTable - SRV (t0: 波情報)
    commandList->SetComputeRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(srvIndex_));

    // [3] DescriptorTable - UAV (u0: 出力バッファ)
    commandList->SetComputeRootDescriptorTable(3, srvManager_->GetGPUDescriptorHandle(uavIndex_));

    // ディスパッチ（8x8のスレッドグループ）
    int dispatchX = (gridSize_ + 7) / 8;
    int dispatchY = (gridSize_ + 7) / 8;
    commandList->Dispatch(dispatchX, dispatchY, 1);

    // UAVバリア（書き込み完了を保証）
    D3D12_RESOURCE_BARRIER uavBarrier = {};
    uavBarrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
    uavBarrier.UAV.pResource = outputBuffer_.Get();
    commandList->ResourceBarrier(1, &uavBarrier);

    // UNORDERED_ACCESS -> COMMON への遷移バリア（次回の使用のため）
    D3D12_RESOURCE_BARRIER barrierToCommon = {};
    barrierToCommon.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrierToCommon.Transition.pResource = outputBuffer_.Get();
    barrierToCommon.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    barrierToCommon.Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
    barrierToCommon.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList->ResourceBarrier(1, &barrierToCommon);
}

///-------------------------------------------/// 
/// 波情報の更新
///-------------------------------------------///
void OceanWaveCompute::UpdateWaveInfos(const std::array<OceanShaderInfo, kWaveCount_>& waveInfos) {
    for (int i = 0; i < kWaveCount_; ++i) {
        waveInfoData_[i] = waveInfos[i];
    }
}

///-------------------------------------------/// 
/// 波紋情報の更新
///-------------------------------------------///
void OceanWaveCompute::UpdateRippleBuffer(const RippleBufferForGPU& rippleBuffer) {*rippleBufferData_ = rippleBuffer;}

///-------------------------------------------/// 
/// ワールド行列の更新
///-------------------------------------------///
void OceanWaveCompute::UpdateWorldMatrix(const Matrix4x4& worldMatrix) {settingsData_->worldMatrix = worldMatrix;}

///-------------------------------------------/// 
/// 計算結果を頂点バッファにコピー
///-------------------------------------------///
void OceanWaveCompute::CopyResultsToVertexBuffer(ID3D12GraphicsCommandList* commandList, ID3D12Resource* vertexBuffer) {
   // UAVバッファのみ COMMON -> COPY_SOURCE へ遷移
    D3D12_RESOURCE_BARRIER barrierToCopySource = {};
    barrierToCopySource.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrierToCopySource.Transition.pResource = outputBuffer_.Get();
    barrierToCopySource.Transition.StateBefore = D3D12_RESOURCE_STATE_COMMON;
    barrierToCopySource.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
    barrierToCopySource.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList->ResourceBarrier(1, &barrierToCopySource);

    // コピー実行
    commandList->CopyResource(vertexBuffer, outputBuffer_.Get());

    // UAVバッファを COPY_SOURCE -> COMMON へ戻す
    D3D12_RESOURCE_BARRIER barrierToCommon = {};
    barrierToCommon.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrierToCommon.Transition.pResource = outputBuffer_.Get();
    barrierToCommon.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
    barrierToCommon.Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
    barrierToCommon.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList->ResourceBarrier(1, &barrierToCommon);
}

///-------------------------------------------/// 
/// UAV用バッファの作成
///-------------------------------------------///
void OceanWaveCompute::CreateUAVBuffer(ID3D12Device* device) {
    HRESULT hr;

    D3D12_HEAP_PROPERTIES defaultHeapProps = {};
    defaultHeapProps.Type = D3D12_HEAP_TYPE_DEFAULT;

    D3D12_RESOURCE_DESC outputResourceDesc = {};
    outputResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
    outputResourceDesc.Width = sizeof(WaveVertexData) * vertexCount_;
    outputResourceDesc.Height = 1;
    outputResourceDesc.DepthOrArraySize = 1;
    outputResourceDesc.MipLevels = 1;
    outputResourceDesc.Format = DXGI_FORMAT_UNKNOWN;
    outputResourceDesc.SampleDesc.Count = 1;
    outputResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
    outputResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

    hr = device->CreateCommittedResource(
        &defaultHeapProps,
        D3D12_HEAP_FLAG_NONE,
        &outputResourceDesc,
        D3D12_RESOURCE_STATE_COMMON,
        nullptr,
        IID_PPV_ARGS(&outputBuffer_)
    );
    assert(SUCCEEDED(hr));
}

///-------------------------------------------/// 
/// SRVとUAVの作成
///-------------------------------------------///
void OceanWaveCompute::CreateViews(ID3D12Device* device, SRVManager* srvManger) {

    // SRVの作成
    srvIndex_ = srvManger->Allocate();
    srvManger->CreateSRVForStructuredBuffer(
        srvIndex_, waveInfoBuffer_->GetBuffer(),
        kWaveCount_,
        sizeof(OceanShaderInfo)
    );

    // UAVの作成
    uavIndex_ = srvManger->Allocate();
    outputUAV_ = std::make_unique<UAV>();
    outputUAV_->Create(
        device,
        outputBuffer_.Get(),
        vertexCount_,  // numElements
        sizeof(WaveVertexData),  // structureByteStride
        srvManger->GetCPUDescriptorHandle(uavIndex_)
    );
}