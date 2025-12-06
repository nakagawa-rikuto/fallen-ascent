#include "Ocean.h"
// c++
#include <cassert>
// Service
#include "Engine/System/Service/GraphicsResourceGetter.h"
#include "Engine/System/Service/Render.h"
#include "Engine/System/Service/DeltaTimeSevice.h"
// Math
#include "Math/sMath.h"
#include "Math/MatrixMath.h"

#ifdef USE_IMGUI
#include <imgui.h>
#endif

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Ocean::~Ocean() {
    vertex_.reset();
    index_.reset();
    material_.reset();
    wvp_.reset();
    directionallight_.reset();
    camera3D_.reset();
    oceanColorBuffer_.reset();
    waveCompute_.reset();
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// 波情報の設定
void Ocean::SetWaveInfo(int waveIndex, const Vector3& direction, float amplitude, float length, float speed) {
    if (waveIndex >= 0 && waveIndex < kWaveCount_) {
        waveInfos_[waveIndex].distance = direction;
        waveInfos_[waveIndex].amplitude = amplitude;
        waveInfos_[waveIndex].length = length;
        waveInfos_[waveIndex].speed = speed;
    }
}
// 色情報の設定
void Ocean::SetColorInfo(const OceanColorInfo& colorInfo) {
    colorInfo_ = colorInfo;
}
// 波紋パラメータの設定
void Ocean::SetRippleSpeed(float speed) { rippleSpeed_ = speed; }
void Ocean::SetRippleDecay(float decay) { rippleDecay_ = decay; }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Ocean::Initialize(int gridSize) {
    ID3D12Device* device = GraphicsResourceGetter::GetDXDevice();

    /// ===生成=== ///
    vertex_ = std::make_unique<BufferBase>();
    index_ = std::make_unique<BufferBase>();
    material_ = std::make_unique<Material3D>();
    wvp_ = std::make_unique<Transform3D>();
    camera3D_ = std::make_unique<BufferBase>();
    directionallight_ = std::make_unique<BufferBase>();
    oceanColorBuffer_ = std::make_unique<BufferBase>();
    waveCompute_ = std::make_unique<OceanWaveCompute>();

    /// ===グリッドサイズの設定=== ///
    gridSize_ = gridSize;
    vertexCount_ = (gridSize_ + 1) * (gridSize_ + 1);
    indexCount_ = gridSize_ * gridSize_ * 6;

    /// ===worldTransform=== ///
    worldTransform_ = { { 0.5f, 1.0f, 0.5f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
    uvTransform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

    /// ===WaveCompute初期化=== ///
    waveCompute_->Initialize(device, gridSize_);

    /// ===vertex=== ///
    // Compute Shaderの出力サイズに合わせる（WaveVertexDataと同じサイズ）
    vertex_->SetBuffer(waveCompute_->GetOutputBuffer());
    vertexBufferView_.BufferLocation = vertex_->GetBuffer()->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = sizeof(VertexData3D) * vertexCount_;
    vertexBufferView_.StrideInBytes = sizeof(VertexData3D);
    //CreateGridMesh(); // 初期状態（平面）を作成

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

    /// ===波情報の初期化=== ///
    InitializeWaveInfos();

    /// ===波紋バッファの初期化=== ///
    for (int i = 0; i < 8; ++i) {
        ripples_[i].isActive = false;
        ripples_[i].position = { 0.0f, 0.0f };
        ripples_[i].startTime = 0.0f;
        ripples_[i].intensity = 0.0f;
        ripples_[i].duration = 3.0f;
        ripples_[i].maxRadius = 20.0f;
        ripples_[i].speed = 4.0f;
        ripples_[i].priority = 0;
    }

    rippleBuffer_.activeCount = 0;
    rippleBuffer_.currentTime = 0.0f;
    rippleBuffer_.rippleSpeed = rippleSpeed_;
    rippleBuffer_.rippleDecay = rippleDecay_;

    // 波紋データの初期化
    for (int i = 0; i < 8; ++i) {
        rippleBuffer_.ripples[i].position = { 0.0f, 0.0f };
        rippleBuffer_.ripples[i].startTime = 0.0f;
        rippleBuffer_.ripples[i].intensity = 0.0f;
        rippleBuffer_.ripples[i].duration = 0.0f;
        rippleBuffer_.ripples[i].maxRadius = 0.0f;
        rippleBuffer_.ripples[i].speed = 0.0f;
        rippleBuffer_.ripples[i].padding = 0.0f;
    }

    nextPriority_ = 0;

    /// ===OceanColorBuffer=== ///
    oceanColorBuffer_->Create(device, sizeof(OceanColorInfo));
    oceanColorBuffer_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&oceanColorData_));
    *oceanColorData_ = colorInfo_;
}

///-------------------------------------------/// 
/// 波情報の初期化（12個）
///-------------------------------------------///
void Ocean::InitializeWaveInfos() {
   
    //// 最初の3つは従来の波
    /*waveInfos_[0] = { { 0.53f, 0.0f, 0.77f }, 1.308f, 10.033f, 2.187f, 0.0f, 0.0f };
    waveInfos_[1] = { { 0.0f, 0.2f, 0.6f }, 0.056f, 7.369f, 2.320f, 0.0f, 0.0f };
    waveInfos_[2] = { { 0.28f, 0.03f, 0.0f }, 0.879f, 4.355f, 2.098f, 0.0f, 0.0f };*/

    // 最初の3つは従来の波
    waveInfos_[0] = { { 0.0f, 0.0f, 0.0f }, 1.308f, 10.033f, 2.187f, 0.0f, 0.0f };
    waveInfos_[1] = { { 0.0f, 0.0f, 0.0f }, 0.056f, 7.369f, 2.320f, 0.0f, 0.0f };
    waveInfos_[2] = { { 0.0f, 0.0f, 0.0f }, 0.879f, 4.355f, 2.098f, 0.0f, 0.0f };
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Ocean::Update() {
    currentTime_ += DeltaTimeSevice::GetDeltaTime();

    /// ===時間の更新=== ///
    for (int i = 0; i < kWaveCount_; ++i) {
        waveInfos_[i].time = currentTime_;
    }

    /// ===波紋の更新=== ///
    UpdateRipples();

    /// ===WorldMatrixの更新=== ///
	waveCompute_->UpdateWorldMatrix(Math::MakeAffineQuaternionMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate));

    /// ===Compute Shaderで波を計算=== ///
    waveCompute_->UpdateWaveInfos(waveInfos_);
    waveCompute_->UpdateRippleBuffer(rippleBuffer_);

    /// ===データの書き込み=== ///
    OceanCommon::Update();
    ColorDataWrite();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Ocean::Draw(BlendMode mode) {
    ID3D12GraphicsCommandList* commandList = GraphicsResourceGetter::GetDXCommandList();

    // Compute PSOとRoot Signatureを設定
    Render::SetPSO(commandList, PipelineType::CSOcean, BlendMode::KBlendModeNormal);
    // Compute Shaderを実行
    waveCompute_->Dispatch(commandList);
    // UAV -> Vertex Bufferへの状態遷移
    D3D12_RESOURCE_BARRIER barrier = {};
    barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
    barrier.Transition.pResource = vertex_->GetBuffer();
    barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
    barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
    commandList->ResourceBarrier(1, &barrier);

    // Graphics PSOを設定
    Render::SetPSO(commandList, PipelineType::PrimitiveOcean, mode);

    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
    commandList->IASetIndexBuffer(&indexBufferView_);

    // バッファの設定
    commandList->SetGraphicsRootConstantBufferView(0, material_->GetBuffer()->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(1, wvp_->GetBuffer()->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(2, directionallight_->GetBuffer()->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(3, camera3D_->GetBuffer()->GetGPUVirtualAddress());
    // OceanColorBufferの設定
    commandList->SetGraphicsRootConstantBufferView(4, oceanColorBuffer_->GetBuffer()->GetGPUVirtualAddress());

    commandList->DrawIndexedInstanced(indexCount_, 1, 0, 0, 0);
}

///-------------------------------------------/// 
/// ImGui
///-------------------------------------------///
void Ocean::ShowImGui() {
#ifdef USE_IMGUI
    ImGui::Begin("Ocean");

    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::DragFloat3("Translate", &worldTransform_.translate.x, 0.1f);
        ImGui::DragFloat4("Rotate", &worldTransform_.rotate.x, 0.01f);
        ImGui::DragFloat3("Scale", &worldTransform_.scale.x, 0.1f);
    }

    if (ImGui::CollapsingHeader("グリッド情報")) {
        ImGui::Text("グリッドサイズ : %d x %d", gridSize_, gridSize_);
        ImGui::Text("Vertex Count : %d", vertexCount_);
        ImGui::Text("Index Count : %d", indexCount_);
        ImGui::Text("Triangle Count : %d", indexCount_ / 3);
    }

    if (ImGui::CollapsingHeader("波のパラメータ", ImGuiTreeNodeFlags_DefaultOpen)) {
        for (int i = 0; i < kWaveCount_; ++i) {
            ImGui::PushID(i);
            if (ImGui::TreeNode("波", "波 %d", i)) {
                ImGui::Text("Direction: (%.2f, %.2f, %.2f)", waveInfos_[i].distance.x, waveInfos_[i].distance.y, waveInfos_[i].distance.z);
                ImGui::Text("Amplitude: %.2f", waveInfos_[i].amplitude);
                ImGui::Text("Length: %.2f", waveInfos_[i].length);
                ImGui::Text("Speed: %.2f", waveInfos_[i].speed);

                ImGui::Separator();

                ImGui::DragFloat3("Edit Direction", &waveInfos_[i].distance.x, 0.01f, -1.0f, 1.0f);
                ImGui::SliderFloat("Edit Amplitude", &waveInfos_[i].amplitude, 0.0f, 2.0f);
                ImGui::SliderFloat("Edit Length", &waveInfos_[i].length, 1.0f, 30.0f);
                ImGui::SliderFloat("Edit Speed", &waveInfos_[i].speed, 0.5f, 10.0f);

                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    }

    // 波紋情報
    if (ImGui::CollapsingHeader("波紋情報", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Active Ripples: %d / 8", rippleBuffer_.activeCount);

        if (ImGui::SliderFloat("Speed", &rippleSpeed_, 1.0f, 10.0f)) {
            rippleBuffer_.rippleSpeed = rippleSpeed_;
        }
        if (ImGui::SliderFloat("Decay", &rippleDecay_, 0.1f, 3.0f)) {
            rippleBuffer_.rippleDecay = rippleDecay_;
        }

        if (ImGui::Button("Clear All Ripples")) {
            ClearRipples();
        }
    }

    // 色情報
    if (ImGui::CollapsingHeader("色")) {
        ImGui::ColorEdit3("Sea Base", &colorInfo_.seaBase.x);
        ImGui::ColorEdit3("Sea Shallow", &colorInfo_.seaShallow.x);
        ImGui::ColorEdit3("Sky", &colorInfo_.sky.x);
        ImGui::ColorEdit3("Deep Water", &colorInfo_.deepWater.x);
        ImGui::SliderFloat("Base Strength", &colorInfo_.baseStrength, 0.0f, 2.0f);
        ImGui::SliderFloat("Water Clarity", &colorInfo_.waterClarity, 0.0f, 1.0f);
        ImGui::SliderFloat("Foam Threshold", &colorInfo_.foamThreshold, 0.0f, 3.0f);
    }

    // テスト機能
    if (ImGui::CollapsingHeader("テスト")) {
        static float testIntensity = 1.0f;
        static float testPosX = 0.0f;
        static float testPosZ = 0.0f;

        ImGui::SliderFloat("Test Intensity", &testIntensity, 0.1f, 3.0f);
        ImGui::SliderFloat("Test Pos X", &testPosX, -50.0f, 50.0f);
        ImGui::SliderFloat("Test Pos Z", &testPosZ, -50.0f, 50.0f);

        if (ImGui::Button("Add Test Ripple")) {
            AddCircularRipple({ testPosX, 0.0f, testPosZ }, 3.0f, testIntensity);
        }

        ImGui::SameLine();

        if (ImGui::Button("Random Ripple")) {
            float randX = static_cast<float>(rand() % 100 - 50);
            float randZ = static_cast<float>(rand() % 100 - 50);
            float randIntensity = 0.5f + (rand() % 100) / 100.0f * 1.5f;
            AddCircularRipple({ randX, 0.0f, randZ }, 3.0f, randIntensity);
        }
    }

    ImGui::End();
#endif
}

///-------------------------------------------/// 
/// グリッドメッシュの生成
///-------------------------------------------///
void Ocean::CreateGridMesh() {
    float gridWidth = 100.0f;
    float gridDepth = 100.0f;
    float step = gridWidth / gridSize_;

    int vertexIndex = 0;
    for (int i = 0; i <= gridSize_; i++) {
        for (int j = 0; j <= gridSize_; j++) {
            float x = -gridWidth * 0.5f + j * step;
            float z = -gridDepth * 0.5f + i * step;
            float u = (float)j / gridSize_;
            float v = (float)i / gridSize_;

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
void Ocean::ColorDataWrite() {
    *oceanColorData_ = colorInfo_;
}

///-------------------------------------------/// 
/// 波紋の更新
///-------------------------------------------///
void Ocean::UpdateRipples() {
    int activeCount = 0;

    for (int i = 0; i < 8; ++i) {
        if (ripples_[i].isActive) {
            float elapsed = currentTime_ - ripples_[i].startTime;

            if (elapsed > ripples_[i].duration) {
                ripples_[i].isActive = false;
            } else {
                rippleBuffer_.ripples[activeCount].position = ripples_[i].position;
                rippleBuffer_.ripples[activeCount].startTime = ripples_[i].startTime;
                rippleBuffer_.ripples[activeCount].intensity = ripples_[i].intensity;
                rippleBuffer_.ripples[activeCount].duration = ripples_[i].duration;
                rippleBuffer_.ripples[activeCount].maxRadius = ripples_[i].maxRadius;
                rippleBuffer_.ripples[activeCount].speed = ripples_[i].speed;
                activeCount++;
            }
        }
    }

    rippleBuffer_.activeCount = activeCount;
    rippleBuffer_.currentTime = currentTime_;
    rippleBuffer_.rippleSpeed = rippleSpeed_;
    rippleBuffer_.rippleDecay = rippleDecay_;
}

///-------------------------------------------/// 
/// 波紋の追加
///-------------------------------------------///
void Ocean::AddCircularRipple(const Vector3& center, float duration, float intensity, float maxRadius) {
    int targetSlot = -1;

    for (int i = 0; i < 8; ++i) {
        if (!ripples_[i].isActive) {
            targetSlot = i;
            break;
        }
    }

    if (targetSlot == -1) {
        int lowestPriority = ripples_[0].priority;
        targetSlot = 0;

        for (int i = 1; i < 8; ++i) {
            if (ripples_[i].priority < lowestPriority) {
                lowestPriority = ripples_[i].priority;
                targetSlot = i;
            }
        }
    }

    if (targetSlot >= 0 && targetSlot < 8) {
        ripples_[targetSlot].position = { center.x, center.z };
        ripples_[targetSlot].startTime = currentTime_;
        ripples_[targetSlot].intensity = intensity;
        ripples_[targetSlot].duration = duration;
        ripples_[targetSlot].maxRadius = maxRadius;
        ripples_[targetSlot].speed = maxRadius / duration;
        ripples_[targetSlot].priority = nextPriority_++;
        ripples_[targetSlot].isActive = true;
    }
}

///-------------------------------------------/// 
/// 波紋のクリア
///-------------------------------------------///
void Ocean::ClearRipples() {
    for (int i = 0; i < 8; ++i) {
        ripples_[i].isActive = false;
    }
    rippleBuffer_.activeCount = 0;
    hitRecords_.clear();
}