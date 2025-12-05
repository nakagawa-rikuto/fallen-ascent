#include "OceanFFTCompute.h"
// Service
#include "Engine/System/Service/ServiceLocator.h"
#include "Engine/System/Managers/SRVManager.h"
#include "Engine/System/Service/Render.h"
#include "Engine/System/Service/DeltaTimeSevice.h"
#include "Engine/System/Service/GraphicsResourceGetter.h"
// Math
#include "Math/sMath.h"
// C++
#include <cassert>
#include <cmath>

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
OceanFFTCompute::~OceanFFTCompute() {
	phillipsBuffer_.reset();
	settingsBuffer_.reset();
	fftParamsBuffer_.reset();
	h0kTexture_.Reset();
	hktTexture_.Reset();
	dxTexture_.Reset();
	dyTexture_.Reset();
	dzTexture_.Reset();
	displacementTexture_.Reset();
	normalTexture_.Reset();
	for (auto& tex : fftPingPong_) {
		tex.Reset();
	}
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void OceanFFTCompute::Initialize(ID3D12Device* device, int resolution) {
	resolution_ = resolution;
	srvManager_ = ServiceLocator::GetSRVManager();

	// デフォルトパラメータ設定
	phillipsParams_.windSpeed = 30.0f;
	phillipsParams_.windDirection = { 1.0f, 0.0f };
	phillipsParams_.amplitude = 0.02f;
	phillipsParams_.suppression = 0.001f;

	settings_.resolution = static_cast<int>(resolution * 2.5f);
	settings_.oceanSize = kOceanSize;
	settings_.time = 0.0f;
	settings_.choppiness = 1.5f;
	settings_.tileScale = 1.0f;
	settings_.cascadeCount = 1;

	printf("=== Ocean FFT Initialize ===\n");
	printf("Resolution: %d\n", resolution_);
	printf("Ocean Size: %.1f\n", settings_.oceanSize);
	printf("Amplitude: %.6f\n", phillipsParams_.amplitude);
	printf("Wind Speed: %.1f\n", phillipsParams_.windSpeed);

	// バッファ作成
	phillipsBuffer_ = std::make_unique<BufferBase>();
	phillipsBuffer_->Create(device, sizeof(PhillipsSpectrumParams));

	settingsBuffer_ = std::make_unique<BufferBase>();
	settingsBuffer_->Create(device, sizeof(OceanSimulationSettings));

	fftParamsBuffer_ = std::make_unique<BufferBase>();
	fftParamsBuffer_->Create(device, sizeof(FFTParams));

	// テクスチャ作成
	CreateTextures(device);

	// ビュー作成
	CreateBuffersAndViews(device, srvManager_);

	// これは初期化時に1回だけ実行する
	ID3D12GraphicsCommandList* commandList = GraphicsResourceGetter::GetDXCommandList();
	// Descriptor Heapをセット
	ID3D12DescriptorHeap* descriptorHeaps[] = {
		srvManager_->GetDescriptorHeap()
	};
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	// 初期スペクトル生成
	GenerateInitialSpectrum(commandList);
}

///-------------------------------------------/// 
/// テクスチャ作成
///-------------------------------------------///
void OceanFFTCompute::CreateTextures(ID3D12Device* device) {
	HRESULT hr;

	// テクスチャの共通設定
	D3D12_HEAP_PROPERTIES defaultHeap = {};
	defaultHeap.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_RESOURCE_DESC texDesc = {};
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Width = resolution_;
	texDesc.Height = resolution_;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
	texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;

	// h0(k) テクスチャ
	hr = device->CreateCommittedResource(
		&defaultHeap, D3D12_HEAP_FLAG_NONE, &texDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
		IID_PPV_ARGS(&h0kTexture_));
	assert(SUCCEEDED(hr));

	// h(k,t) テクスチャ
	hr = device->CreateCommittedResource(
		&defaultHeap, D3D12_HEAP_FLAG_NONE, &texDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
		IID_PPV_ARGS(&hktTexture_));
	assert(SUCCEEDED(hr));

	// 変位テクスチャ群
	hr = device->CreateCommittedResource(
		&defaultHeap, D3D12_HEAP_FLAG_NONE, &texDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
		IID_PPV_ARGS(&dxTexture_));
	assert(SUCCEEDED(hr));

	hr = device->CreateCommittedResource(
		&defaultHeap, D3D12_HEAP_FLAG_NONE, &texDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
		IID_PPV_ARGS(&dyTexture_));
	assert(SUCCEEDED(hr));

	hr = device->CreateCommittedResource(
		&defaultHeap, D3D12_HEAP_FLAG_NONE, &texDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
		IID_PPV_ARGS(&dzTexture_));
	assert(SUCCEEDED(hr));

	// 統合変位マップ
	hr = device->CreateCommittedResource(
		&defaultHeap, D3D12_HEAP_FLAG_NONE, &texDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
		IID_PPV_ARGS(&displacementTexture_));
	assert(SUCCEEDED(hr));

	// 法線マップ
	hr = device->CreateCommittedResource(
		&defaultHeap, D3D12_HEAP_FLAG_NONE, &texDesc,
		D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
		IID_PPV_ARGS(&normalTexture_));
	assert(SUCCEEDED(hr));

	// Ping-pongバッファ(FFT用)
	for (int i = 0; i < 2; ++i) {
		hr = device->CreateCommittedResource(
			&defaultHeap, D3D12_HEAP_FLAG_NONE, &texDesc,
			D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr,
			IID_PPV_ARGS(&fftPingPong_[i]));
		assert(SUCCEEDED(hr));
	}
}

///-------------------------------------------/// 
/// バッファとビュー作成
///-------------------------------------------///
void OceanFFTCompute::CreateBuffersAndViews(ID3D12Device* device, SRVManager* srvManager) {
	// UAVの作成ヘルパー
	auto CreateUAVForTexture = [&](ID3D12Resource* resource) -> uint32_t {
		uint32_t index = srvManager->Allocate();

		D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
		uavDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		uavDesc.ViewDimension = D3D12_UAV_DIMENSION_TEXTURE2D;
		uavDesc.Texture2D.MipSlice = 0;

		device->CreateUnorderedAccessView(
			resource, nullptr, &uavDesc,
			srvManager->GetCPUDescriptorHandle(index));

		return index;
		};

	// 各テクスチャにUAVを作成
	h0kUAV_ = CreateUAVForTexture(h0kTexture_.Get());
	hktUAV_ = CreateUAVForTexture(hktTexture_.Get());
	dxUAV_ = CreateUAVForTexture(dxTexture_.Get());
	dyUAV_ = CreateUAVForTexture(dyTexture_.Get());
	dzUAV_ = CreateUAVForTexture(dzTexture_.Get());
	displacementUAV_ = CreateUAVForTexture(displacementTexture_.Get());
	normalUAV_ = CreateUAVForTexture(normalTexture_.Get());

	for (int i = 0; i < 2; ++i) {
		pingPongUAV_[i] = CreateUAVForTexture(fftPingPong_[i].Get());
	}

	// SRVの作成
	auto CreateSRVForTexture = [&](ID3D12Resource* resource) -> uint32_t {
		uint32_t index = srvManager->Allocate();
		srvManager->CreateSRVForTexture2D(
			index, resource,
			DXGI_FORMAT_R32G32B32A32_FLOAT, 1);
		return index;
		};

	hktSRV_ = CreateSRVForTexture(hktTexture_.Get());
	displacementSRV_ = CreateSRVForTexture(displacementTexture_.Get());
	normalSRV_ = CreateSRVForTexture(normalTexture_.Get());

	for (int i = 0; i < 2; ++i) {
		pingPongSRV_[i] = CreateSRVForTexture(fftPingPong_[i].Get());
	}
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void OceanFFTCompute::Update() {
	currentTime_ += DeltaTimeSevice::GetDeltaTime();
	settings_.time = currentTime_;
}

///-------------------------------------------/// 
/// Compute Shader実行
///-------------------------------------------///
void OceanFFTCompute::Dispatch(ID3D12GraphicsCommandList* commandList) {
	// Descriptor Heapをセット
	ID3D12DescriptorHeap* descriptorHeaps[] = {
		srvManager_->GetDescriptorHeap()  // SRV/UAV用のDescriptor Heap
	};
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	
	// 初回以降のみ、SRV→UAVに戻すバリアを張る
	static bool isFirstFrame = true;
	if (!isFirstFrame) {
		D3D12_RESOURCE_BARRIER barriersToUAV[2] = {};

		barriersToUAV[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barriersToUAV[0].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barriersToUAV[0].Transition.pResource = displacementTexture_.Get();
		barriersToUAV[0].Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
		barriersToUAV[0].Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		barriersToUAV[0].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		barriersToUAV[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barriersToUAV[1].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barriersToUAV[1].Transition.pResource = normalTexture_.Get();
		barriersToUAV[1].Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
		barriersToUAV[1].Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		barriersToUAV[1].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;

		commandList->ResourceBarrier(2, barriersToUAV);
	} else {
		isFirstFrame = false;
	}

	// 時間依存スペクトル更新
	UpdateSpectrum(commandList);

	// FFT実行
	ExecuteFFT(commandList);

	// 法線と変位計算
	CalculateNormalsAndDerivatives(commandList);
}

///-------------------------------------------/// 
/// 初期スペクトル生成
///-------------------------------------------///
void OceanFFTCompute::GenerateInitialSpectrum(ID3D12GraphicsCommandList* commandList) {
	// Descriptor Heapをセット
	ID3D12DescriptorHeap* descriptorHeaps[] = {
		srvManager_->GetDescriptorHeap()
	};
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	
	// Compute PSOを設定
	Render::SetPSO(commandList, PipelineType::CSOceanSpectrumH0, BlendMode::KBlendModeNormal);

	// パラメータ書き込み
	void* phillipsData;
	phillipsBuffer_->GetBuffer()->Map(0, nullptr, &phillipsData);
	memcpy(phillipsData, &phillipsParams_, sizeof(PhillipsSpectrumParams));
	phillipsBuffer_->GetBuffer()->Unmap(0, nullptr);

	// [0] CBV - Phillips Params (b0)
	commandList->SetComputeRootConstantBufferView(0, phillipsBuffer_->GetBuffer()->GetGPUVirtualAddress());

	// [1] CBV - Settings (b1)
	commandList->SetComputeRootConstantBufferView(1, settingsBuffer_->GetBuffer()->GetGPUVirtualAddress());

	// [2] UAV - h0k, hkt
	commandList->SetComputeRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(h0kUAV_));

	int dispatchSize = (resolution_ + 7) / 8;
	commandList->Dispatch(dispatchSize, dispatchSize, 1);

	// UAVバリア
	D3D12_RESOURCE_BARRIER barriers[2] = {};
	barriers[0].Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barriers[0].UAV.pResource = h0kTexture_.Get();
	barriers[1].Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barriers[1].UAV.pResource = hktTexture_.Get();
	commandList->ResourceBarrier(2, barriers);
}

///-------------------------------------------/// 
/// 時間依存スペクトル更新
///-------------------------------------------///
void OceanFFTCompute::UpdateSpectrum(ID3D12GraphicsCommandList* commandList) {
	// Descriptor Heapをセット
	ID3D12DescriptorHeap* descriptorHeaps[] = {
		srvManager_->GetDescriptorHeap()
	};
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);
	
	// Compute PSOを設定
	Render::SetPSO(commandList, PipelineType::CSOceanSpectrum, BlendMode::KBlendModeNormal);

	// 設定更新
	void* settingsData;
	settingsBuffer_->GetBuffer()->Map(0, nullptr, &settingsData);
	memcpy(settingsData, &settings_, sizeof(OceanSimulationSettings));
	settingsBuffer_->GetBuffer()->Unmap(0, nullptr);

	// [0] CBV - Phillips Params
	commandList->SetComputeRootConstantBufferView(0, phillipsBuffer_->GetBuffer()->GetGPUVirtualAddress());

	// [1] CBV - Settings
	commandList->SetComputeRootConstantBufferView(1, settingsBuffer_->GetBuffer()->GetGPUVirtualAddress());

	// [2] UAV - h0k 
	commandList->SetComputeRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(h0kUAV_));
	commandList->SetComputeRootDescriptorTable(3, srvManager_->GetGPUDescriptorHandle(hktUAV_));

	int dispatchSize = (resolution_ + 7) / 8;
	commandList->Dispatch(dispatchSize, dispatchSize, 1);

	// UAVバリア
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barrier.UAV.pResource = hktTexture_.Get();
	commandList->ResourceBarrier(1, &barrier);
}

///-------------------------------------------/// 
/// FFT実行(Cooley-Tukey algorithm)
///-------------------------------------------///
void OceanFFTCompute::ExecuteFFT(ID3D12GraphicsCommandList* commandList) {
	// Descriptor Heapをセット
	ID3D12DescriptorHeap* descriptorHeaps[] = {
		srvManager_->GetDescriptorHeap()
	};
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// Compute PSOを設定
	Render::SetPSO(commandList, PipelineType::CSOceanFFT, BlendMode::KBlendModeNormal);

	int numStages = static_cast<int>(std::log2(resolution_));

	/// ===水平FFT=== ///
	for (int stage = 0; stage < numStages; ++stage) {
		FFTParams params;
		params.stage = stage;
		params.direction = 0; // horizontal
		params.resolution = resolution_;
		params.pingPong = stage % 2;

		void* fftData;
		fftParamsBuffer_->GetBuffer()->Map(0, nullptr, &fftData);
		memcpy(fftData, &params, sizeof(FFTParams));
		fftParamsBuffer_->GetBuffer()->Unmap(0, nullptr);

		// [0] CBV - FFT Params
		commandList->SetComputeRootConstantBufferView(0, fftParamsBuffer_->GetBuffer()->GetGPUVirtualAddress());

		// 入力SRVの選択
		ID3D12Resource* inputResource = (stage == 0) ? hktTexture_.Get() : fftPingPong_[(stage - 1) % 2].Get();
		uint32_t inputSRV = (stage == 0) ? hktSRV_ : pingPongSRV_[(stage - 1) % 2];

		// UAVからSRVへリソースバリア（入力用）
		D3D12_RESOURCE_BARRIER barrierToSRV = {};
		barrierToSRV.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrierToSRV.Transition.pResource = inputResource;
		barrierToSRV.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		barrierToSRV.Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
		barrierToSRV.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		commandList->ResourceBarrier(1, &barrierToSRV);

		// [1] SRV - Input
		commandList->SetComputeRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(inputSRV));

		// [2] UAV - Output
		commandList->SetComputeRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(pingPongUAV_[params.pingPong]));

		commandList->Dispatch(1, (resolution_ + 7) / 8, 1);

		// UAVバリア (出力用)
		D3D12_RESOURCE_BARRIER barrierUAV = {};
		barrierUAV.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
		barrierUAV.UAV.pResource = fftPingPong_[params.pingPong].Get();
		commandList->ResourceBarrier(1, &barrierUAV);

		// SRV -> UAV バリア (入力を戻す)
		D3D12_RESOURCE_BARRIER barrierToUAV = {};
		barrierToUAV.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrierToUAV.Transition.pResource = inputResource;
		barrierToUAV.Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
		barrierToUAV.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		barrierToUAV.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		commandList->ResourceBarrier(1, &barrierToUAV);
	}

	/// ===垂直FFT=== ///
	for (int stage = 0; stage < numStages; ++stage) {
		FFTParams params;
		params.stage = stage;
		params.direction = 1; // vertical
		params.resolution = resolution_;
		params.pingPong = stage % 2;

		void* fftData;
		fftParamsBuffer_->GetBuffer()->Map(0, nullptr, &fftData);
		memcpy(fftData, &params, sizeof(FFTParams));
		fftParamsBuffer_->GetBuffer()->Unmap(0, nullptr);

		commandList->SetComputeRootConstantBufferView(0, fftParamsBuffer_->GetBuffer()->GetGPUVirtualAddress());

		// 入力リソースの選択
		ID3D12Resource* inputResource = (stage == 0) ? fftPingPong_[(numStages - 1) % 2].Get() : fftPingPong_[(stage - 1) % 2].Get();
		uint32_t inputSRV = (stage == 0) ? pingPongSRV_[(numStages - 1) % 2] : pingPongSRV_[(stage - 1) % 2];

		// UAV -> SRV バリア (入力用)
		D3D12_RESOURCE_BARRIER barrierToSRV = {};
		barrierToSRV.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrierToSRV.Transition.pResource = inputResource;
		barrierToSRV.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		barrierToSRV.Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
		barrierToSRV.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		commandList->ResourceBarrier(1, &barrierToSRV);

		commandList->SetComputeRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(inputSRV));
		commandList->SetComputeRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(pingPongUAV_[params.pingPong]));

		commandList->Dispatch((resolution_ + 7) / 8, 1, 1);

		// UAVバリア (出力用)
		D3D12_RESOURCE_BARRIER barrierUAV = {};
		barrierUAV.Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
		barrierUAV.UAV.pResource = fftPingPong_[params.pingPong].Get();
		commandList->ResourceBarrier(1, &barrierUAV);

		// SRV -> UAV バリア (入力を戻す)
		D3D12_RESOURCE_BARRIER barrierToUAV = {};
		barrierToUAV.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrierToUAV.Transition.pResource = inputResource;
		barrierToUAV.Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
		barrierToUAV.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
		barrierToUAV.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
		commandList->ResourceBarrier(1, &barrierToUAV);
	}
}

///-------------------------------------------/// 
/// 法線とJacobian計算
///-------------------------------------------///
void OceanFFTCompute::CalculateNormalsAndDerivatives(ID3D12GraphicsCommandList* commandList) {
	// Descriptor Heapをセット
	ID3D12DescriptorHeap* descriptorHeaps[] = {
		srvManager_->GetDescriptorHeap()
	};
	commandList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);

	// Compute PSOを設定
	Render::SetPSO(commandList, PipelineType::CSOceanDisplacement, BlendMode::KBlendModeNormal);

	int numStages = static_cast<int>(std::log2(resolution_));
	ID3D12Resource* fftResultResource = fftPingPong_[(numStages - 1) % 2].Get();
	uint32_t inputSRV = pingPongSRV_[(numStages - 1) % 2];

	// UAV -> SRV バリア (FFT結果を読むため)
	D3D12_RESOURCE_BARRIER barrierToSRV = {};
	barrierToSRV.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrierToSRV.Transition.pResource = fftResultResource;
	barrierToSRV.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	barrierToSRV.Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
	barrierToSRV.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	commandList->ResourceBarrier(1, &barrierToSRV);

	// [0] CBV - Settings
	commandList->SetComputeRootConstantBufferView(0, settingsBuffer_->GetBuffer()->GetGPUVirtualAddress());

	// [1] SRV - FFT結果
	commandList->SetComputeRootDescriptorTable(1, srvManager_->GetGPUDescriptorHandle(inputSRV));

	// [2] UAV - Displacement, Normal
	commandList->SetComputeRootDescriptorTable(2, srvManager_->GetGPUDescriptorHandle(displacementUAV_));

	int dispatchSize = (resolution_ + 7) / 8;
	commandList->Dispatch(dispatchSize, dispatchSize, 1);

	// UAVバリア
	D3D12_RESOURCE_BARRIER barriers[2] = {};
	barriers[0].Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barriers[0].UAV.pResource = displacementTexture_.Get();
	barriers[1].Type = D3D12_RESOURCE_BARRIER_TYPE_UAV;
	barriers[1].UAV.pResource = normalTexture_.Get();
	commandList->ResourceBarrier(2, barriers);

	// SRV -> UAV バリア (FFT結果を戻す)
	D3D12_RESOURCE_BARRIER barrierToUAV = {};
	barrierToUAV.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrierToUAV.Transition.pResource = fftResultResource;
	barrierToUAV.Transition.StateBefore = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
	barrierToUAV.Transition.StateAfter = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
	barrierToUAV.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
	commandList->ResourceBarrier(1, &barrierToUAV);
}