#include "OceanFFT.h"
// C++
#include <cassert>
// Service
#include "Engine/System/Service/GraphicsResourceGetter.h"
#include "Engine/System/Service/Render.h"
#include "Engine/System/Service/DeltaTimeSevice.h"
#include "Engine/System/Service/ServiceLocator.h"
#include "Engine/System/Managers/SRVManager.h"
// Math
#include "Math/sMath.h"
#include "Math/MatrixMath.h"

#ifdef USE_IMGUI
#include <imgui.h>
#endif

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
OceanFFT::~OceanFFT() {
    vertex_.reset();
    index_.reset();
    material_.reset();
    wvp_.reset();
    directionallight_.reset();
    camera3D_.reset();
    oceanColorBuffer_.reset();
    fftCompute_.reset();
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void OceanFFT::SetWindSpeed(float speed) {
    windSpeed_ = speed;
    if (fftCompute_) {
        fftCompute_->SetWindSpeed(speed);
    }
}

void OceanFFT::SetWindDirection(const Vector2& dir) {
    windDirection_ = dir;
    if (fftCompute_) {
        fftCompute_->SetWindDirection(dir);
    }
}

void OceanFFT::SetChoppiness(float value) {
    choppiness_ = value;
    if (fftCompute_) {
        fftCompute_->SetChoppiness(value);
    }
}

void OceanFFT::SetAmplitude(float value) {
    amplitude_ = value;
    if (fftCompute_) {
        fftCompute_->SetAmplitude(value);
    }
}

void OceanFFT::SetSuppression(float value) {
    suppression_ = value;
}

void OceanFFT::SetColorInfo(const OceanFFTColorInfo& colorInfo) {
    colorInfo_ = colorInfo;
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void OceanFFT::Initialize(int gridSize, int fftResolution) {
    ID3D12Device* device = GraphicsResourceGetter::GetDXDevice();

    /// ===生成=== ///
    vertex_ = std::make_unique<BufferBase>();
    index_ = std::make_unique<BufferBase>();
    material_ = std::make_unique<Material3D>();
    wvp_ = std::make_unique<Transform3D>();
    camera3D_ = std::make_unique<BufferBase>();
    directionallight_ = std::make_unique<BufferBase>();
    oceanColorBuffer_ = std::make_unique<BufferBase>();
    fftCompute_ = std::make_unique<OceanFFTCompute>();

    /// ===グリッドサイズの設定=== ///
    gridSize_ = gridSize;
    fftResolution_ = fftResolution;
    vertexCount_ = (gridSize_ + 1) * (gridSize_ + 1);
    indexCount_ = gridSize_ * gridSize_ * 6;

    /// ===worldTransform=== ///
    worldTransform_ = { { 10.0f, 1.0f, 10.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
    uvTransform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

    /// ===FFT Compute初期化=== ///
    fftCompute_->Initialize(device, fftResolution_);
    fftCompute_->SetWindSpeed(windSpeed_);
    fftCompute_->SetWindDirection(windDirection_);
    fftCompute_->SetChoppiness(choppiness_);
    fftCompute_->SetAmplitude(amplitude_);

    /// ===vertex=== ///
    vertex_->Create(device, sizeof(VertexData3D) * vertexCount_);
    vertex_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
    CreateGridMesh(); // グリッドメッシュ生成

    vertexBufferView_.BufferLocation = vertex_->GetBuffer()->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = sizeof(VertexData3D) * vertexCount_;
    vertexBufferView_.StrideInBytes = sizeof(VertexData3D);

    /// ===index=== ///
    index_->Create(device, sizeof(uint32_t) * indexCount_);
    index_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

    int idx = 0;
    for (int z = 0; z < gridSize_; ++z) {
        for (int x = 0; x < gridSize_; ++x) {
            int topLeft = z * (gridSize_ + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * (gridSize_ + 1) + x;
            int bottomRight = bottomLeft + 1;

            indexData_[idx++] = topLeft;
            indexData_[idx++] = bottomLeft;
            indexData_[idx++] = topRight;
            indexData_[idx++] = topRight;
            indexData_[idx++] = bottomLeft;
            indexData_[idx++] = bottomRight;
        }
    }

    indexBufferView_.BufferLocation = index_->GetBuffer()->GetGPUVirtualAddress();
    indexBufferView_.SizeInBytes = sizeof(uint32_t) * indexCount_;
    indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

    /// ===OceanCommon=== ///
    OceanCommon::Initialize(device);

    /// ===OceanColorBuffer=== ///
    oceanColorBuffer_->Create(device, sizeof(OceanFFTColorInfo));
    oceanColorBuffer_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&oceanColorData_));
    *oceanColorData_ = colorInfo_;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void OceanFFT::Update() {
    currentTime_ += DeltaTimeSevice::GetDeltaTime();

    /// ===FFT Compute更新=== ///
    fftCompute_->Update();

    /// ===データの書き込み=== ///
    OceanCommon::Update();
    ColorDataWrite();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void OceanFFT::Draw(BlendMode mode) {
    ID3D12GraphicsCommandList* commandList = GraphicsResourceGetter::GetDXCommandList();

    /// ===Compute Shader実行（FFT計算）=== ///
    ExecuteFFTCompute(commandList);

    /// ===Graphics描画=== ///
    // Graphics PSOを設定
    Render::SetPSO(commandList, PipelineType::PrimitiveOceanFFT, mode);

    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
    commandList->IASetIndexBuffer(&indexBufferView_);

    // バッファの設定
    // [0] Transform (b0 - Vertex Shader)
    commandList->SetGraphicsRootConstantBufferView(0, wvp_->GetBuffer()->GetGPUVirtualAddress());

    // [1] Displacement Map (t0 - Vertex Shader)
    commandList->SetGraphicsRootDescriptorTable(1,
        ServiceLocator::GetSRVManager()->GetGPUDescriptorHandle(fftCompute_->GetDisplacementSRV()));

    // [2] Normal Map (t1 - Vertex Shader)
    commandList->SetGraphicsRootDescriptorTable(2,
        ServiceLocator::GetSRVManager()->GetGPUDescriptorHandle(fftCompute_->GetNormalSRV()));

    // [3] Material (b0 - Pixel Shader)
    commandList->SetGraphicsRootConstantBufferView(3, material_->GetBuffer()->GetGPUVirtualAddress());

    // [4] DirectionalLight (b1 - Pixel Shader)
    commandList->SetGraphicsRootConstantBufferView(4, directionallight_->GetBuffer()->GetGPUVirtualAddress());

    // [5] Camera (b2 - Pixel Shader)
    commandList->SetGraphicsRootConstantBufferView(5, camera3D_->GetBuffer()->GetGPUVirtualAddress());

    // [6] OceanColor (b3 - Pixel Shader)
    commandList->SetGraphicsRootConstantBufferView(6, oceanColorBuffer_->GetBuffer()->GetGPUVirtualAddress());

    commandList->DrawIndexedInstanced(indexCount_, 1, 0, 0, 0);
}

///-------------------------------------------/// 
/// ImGui
///-------------------------------------------///
void OceanFFT::ShowImGui() {
#ifdef USE_IMGUI
    ImGui::Begin("Ocean FFT");

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::DragFloat3("Translate", &worldTransform_.translate.x, 0.1f);
        ImGui::DragFloat4("Rotate", &worldTransform_.rotate.x, 0.01f);
        ImGui::DragFloat3("Scale", &worldTransform_.scale.x, 0.1f);
    }

    if (ImGui::CollapsingHeader("グリッド情報")) {
        ImGui::Text("グリッドサイズ: %d x %d", gridSize_, gridSize_);
        ImGui::Text("FFT解像度: %d x %d", fftResolution_, fftResolution_);
        ImGui::Text("Vertex Count: %d", vertexCount_);
        ImGui::Text("Index Count: %d", indexCount_);
        ImGui::Text("Triangle Count: %d", indexCount_ / 3);
    }

    if (ImGui::CollapsingHeader("FFTパラメータ", ImGuiTreeNodeFlags_DefaultOpen)) {
        if (ImGui::SliderFloat("Wind Speed (m/s)", &windSpeed_, 10.0f, 50.0f)) {
            SetWindSpeed(windSpeed_);
        }

        if (ImGui::SliderFloat2("Wind Direction", &windDirection_.x, -1.0f, 1.0f)) {
            // 正規化
            float length = std::sqrt(windDirection_.x * windDirection_.x + windDirection_.y * windDirection_.y);
            if (length > 0.001f) {
                windDirection_.x /= length;
                windDirection_.y /= length;
            }
            SetWindDirection(windDirection_);
        }

        if (ImGui::SliderFloat("Amplitude", &amplitude_, 0.00f, 100.0f, "%.5f")) {
            SetAmplitude(amplitude_);
        }

        if (ImGui::SliderFloat("Choppiness", &choppiness_, 0.0f, 100.0f)) {
            SetChoppiness(choppiness_);
        }

        if (ImGui::SliderFloat("Suppression", &suppression_, 0.0001f, 100.0f, "%.5f")) {
            SetSuppression(suppression_);
        }

        ImGui::Text("Current Time: %.2f", currentTime_);
    }

    if (ImGui::CollapsingHeader("色")) {
        ImGui::ColorEdit3("Sea Base", &colorInfo_.seaBase.x);
        ImGui::ColorEdit3("Sea Shallow", &colorInfo_.seaShallow.x);
        ImGui::ColorEdit3("Sky", &colorInfo_.sky.x);
        ImGui::ColorEdit3("Deep Water", &colorInfo_.deepWater.x);
        ImGui::SliderFloat("Base Strength", &colorInfo_.baseStrength, 0.0f, 2.0f);
        ImGui::SliderFloat("Water Clarity", &colorInfo_.waterClarity, 0.0f, 1.0f);
        ImGui::SliderFloat("Foam Threshold", &colorInfo_.foamThreshold, 0.0f, 3.0f);
        ImGui::SliderFloat("Foam Scale", &colorInfo_.foamScale, 0.0f, 2.0f);
    }

    if (ImGui::CollapsingHeader("パフォーマンス情報")) {
        ImGui::Text("FFTステージ数: %d", static_cast<int>(std::log2(fftResolution_)));
        ImGui::Text("総ディスパッチ数: %d", static_cast<int>(std::log2(fftResolution_)) * 2 + 2);
    }

    if (ImGui::CollapsingHeader("デバッグ")) {
        ImGui::Text("Ocean Grid Size: %.1f x %.1f", 100.0f, 100.0f);
        ImGui::Text("FFT Resolution: %d", fftResolution_);
        ImGui::Text("Grid Resolution: %d", gridSize_);
        ImGui::Text("Texel per Grid Cell: %.2f", static_cast<float>(fftResolution_) / gridSize_);
    }

    ImGui::End();
#endif
}

///-------------------------------------------/// 
/// グリッドメッシュの生成
///-------------------------------------------///
void OceanFFT::CreateGridMesh() {
    float gridWidth = 100.0f;
    float gridDepth = 100.0f;
    float step = gridWidth / gridSize_;

    int vertexIndex = 0;
    for (int i = 0; i <= gridSize_; i++) {
        for (int j = 0; j <= gridSize_; j++) {
            float x = -gridWidth * 0.5f + j * step;
            float z = -gridDepth * 0.5f + i * step;
            float u = static_cast<float>(j) / gridSize_;
            float v = static_cast<float>(i) / gridSize_;

            vertexData_[vertexIndex] = {
                {x, 0.0f, z, 1.0f},
                {u, v},
                {0.0f, 1.0f, 0.0f}
            };
            vertexIndex++;
        }
    }
}

///-------------------------------------------/// 
/// ColorDataの書き込み
///-------------------------------------------///
void OceanFFT::ColorDataWrite() {
    *oceanColorData_ = colorInfo_;
}

///-------------------------------------------/// 
/// Compute Shader実行（FFT計算）
///-------------------------------------------///
void OceanFFT::ExecuteFFTCompute(ID3D12GraphicsCommandList* commandList) {
    // FFT Computeのディスパッチを実行
    fftCompute_->Dispatch(commandList);

    // DisplacementとNormalテクスチャをUAV→SRVに遷移
    D3D12_RESOURCE_BARRIER barriers[2] = {};
    // Displacement: UAV → SRV
    barriers[0].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barriers[0].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barriers[0].Transition.pResource = fftCompute_->GetDisplacementTexture();
    barriers[0].Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    barriers[0].Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
    barriers[0].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    // Normal: UAV → SRV
    barriers[1].Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barriers[1].Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
    barriers[1].Transition.pResource = fftCompute_->GetNormalTexture();
    barriers[1].Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    barriers[1].Transition.StateAfter = D3D12_RESOURCE_STATE_NON_PIXEL_SHADER_RESOURCE;
    barriers[1].Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
    commandList->ResourceBarrier(2, barriers);
}