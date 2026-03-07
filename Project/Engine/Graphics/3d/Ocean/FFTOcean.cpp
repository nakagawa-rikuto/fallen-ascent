#include "FFTOcean.h"
// c++
#include <cassert>
// Service
#include "Service/GraphicsResourceGetter.h"
#include "Service/Render.h"
#include "Service/DeltaTime.h"
// Math
#include "Math/sMath.h"
#include "Math/MatrixMath.h"

#ifdef USE_IMGUI
#include <imgui.h>
#endif

namespace MiiEngine {
    ///-------------------------------------------/// 
    /// デストラクタ
    ///-------------------------------------------///
    FFTOcean::~FFTOcean() {
        vertex_.reset();
        index_.reset();
        material_.reset();
        wvp_.reset();
        directionalLight_.reset();
        camera3D_.reset();
        oceanColorBuffer_.reset();
        fftCompute_.reset();
    }

    ///-------------------------------------------/// 
    /// Setter
    ///-------------------------------------------///
    // FFTパラメータの設定
    void FFTOcean::SetFFTParams(const Vector2& windDirection, float windSpeed, float amplitude, float choppiness) {
        fftParams_.windDirection = windDirection;
        fftParams_.windSpeed = windSpeed;
        fftParams_.amplitude = amplitude;
        fftParams_.choppiness = choppiness;
    }

    // 色情報の設定
    void FFTOcean::SetColorInfo(const OceanColorInfo& colorInfo) {
        colorInfo_ = colorInfo;
    }

    ///-------------------------------------------/// 
    /// 初期化
    ///-------------------------------------------///
    void FFTOcean::Initialize(int gridSize) {
        ID3D12Device* device = Service::GraphicsResourceGetter::GetDXDevice();

        /// ===生成=== ///
        vertex_ = std::make_unique<BufferBase>();
        index_ = std::make_unique<BufferBase>();
        material_ = std::make_unique<Material3D>();
        wvp_ = std::make_unique<Transform3D>();
        camera3D_ = std::make_unique<BufferBase>();
        directionalLight_ = std::make_unique<BufferBase>();
        oceanColorBuffer_ = std::make_unique<BufferBase>();
        fftCompute_ = std::make_unique<FFTOceanCompute>();

        /// ===グリッドサイズの設定=== ///
        gridSize_ = gridSize;
        vertexCount_ = (gridSize_ + 1) * (gridSize_ + 1);
        indexCount_ = gridSize_ * gridSize_ * 6;

        /// ===worldTransform=== ///
        worldTransform_ = { { 0.5f, 1.0f, 0.5f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
        uvTransform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

        /// ===FFTCompute初期化=== ///
        fftCompute_->Initialize(device, gridSize_);

        /// ===vertex=== ///
        // Compute Shaderの出力サイズに合わせる（WaveVertexDataと同じサイズ）
        vertex_->SetBuffer(fftCompute_->GetOutputBuffer());
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
        oceanColorBuffer_->Create(device, sizeof(OceanColorInfo));
        oceanColorBuffer_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&oceanColorData_));
        *oceanColorData_ = colorInfo_;
    }

    ///-------------------------------------------/// 
    /// 更新
    ///-------------------------------------------///
    void FFTOcean::Update() {
        currentTime_ += Service::DeltaTime::GetDeltaTime();

        /// ===FFTパラメータの時間を更新=== ///
        fftParams_.time = currentTime_;

        /// ===WorldMatrixの更新=== ///
        fftCompute_->UpdateWorldMatrix(Math::MakeAffineQuaternionMatrix(worldTransform_.scale, worldTransform_.rotate, worldTransform_.translate));

        /// ===Compute ShaderへFFTパラメータを送る=== ///
        fftCompute_->UpdateFFTParams(
            fftParams_.windDirection,
            fftParams_.windSpeed,
            fftParams_.amplitude,
            fftParams_.choppiness,
            fftParams_.time
        );

        /// ===データの書き込み=== ///
        OceanCommon::Update();
        ColorDataWrite();
    }

    ///-------------------------------------------/// 
    /// 描画
    ///-------------------------------------------///
    void FFTOcean::Draw(BlendMode mode) {
        ID3D12GraphicsCommandList* commandList = Service::GraphicsResourceGetter::GetDXCommandList();

        // Compute PSOとRoot Signatureを設定
        Service::Render::SetPSO(commandList, PipelineType::CSFFTOcean, BlendMode::KBlendModeNormal);
        // Compute Shaderを実行
        fftCompute_->Dispatch(commandList);
        // UAV -> Vertex Bufferへの状態遷移
        D3D12_RESOURCE_BARRIER barrier = {};
        barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrier.Transition.pResource = vertex_->GetBuffer();
        barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_UNORDERED_ACCESS;
        barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
        commandList->ResourceBarrier(1, &barrier);

        // Graphics PSOを設定
        Service::Render::SetPSO(commandList, PipelineType::PrimitiveFFTOcean, mode);

        commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
        commandList->IASetIndexBuffer(&indexBufferView_);

        // バッファの設定
        commandList->SetGraphicsRootConstantBufferView(0, material_->GetBuffer()->GetGPUVirtualAddress());
        commandList->SetGraphicsRootConstantBufferView(1, wvp_->GetBuffer()->GetGPUVirtualAddress());
        commandList->SetGraphicsRootConstantBufferView(2, directionalLight_->GetBuffer()->GetGPUVirtualAddress());
        commandList->SetGraphicsRootConstantBufferView(3, camera3D_->GetBuffer()->GetGPUVirtualAddress());
        // OceanColorBufferの設定
        commandList->SetGraphicsRootConstantBufferView(4, oceanColorBuffer_->GetBuffer()->GetGPUVirtualAddress());

        commandList->DrawIndexedInstanced(indexCount_, 1, 0, 0, 0);

        // 描画後、次のフレームのために VERTEX_AND_CONSTANT_BUFFER -> COMMON へ戻す
        D3D12_RESOURCE_BARRIER barrierToCommon = {};
        barrierToCommon.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
        barrierToCommon.Transition.pResource = vertex_->GetBuffer();
        barrierToCommon.Transition.StateBefore = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER;
        barrierToCommon.Transition.StateAfter = D3D12_RESOURCE_STATE_COMMON;
        barrierToCommon.Transition.Subresource = D3D12_RESOURCE_BARRIER_ALL_SUBRESOURCES;
        commandList->ResourceBarrier(1, &barrierToCommon);
    }

    ///-------------------------------------------/// 
    /// ImGui
    ///-------------------------------------------///
    void FFTOcean::ShowImGui() {
#ifdef USE_IMGUI
        ImGui::Begin("FFT海洋シェーダー");

        /// ===Transform情報=== ///
        if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::DragFloat3("Translate", &worldTransform_.translate.x, 0.1f);
            ImGui::DragFloat4("Rotate", &worldTransform_.rotate.x, 0.01f);
            ImGui::DragFloat3("Scale", &worldTransform_.scale.x, 0.1f);
        }

        /// ===グリッド情報=== ///
        if (ImGui::CollapsingHeader("グリッド情報")) {
            ImGui::Text("グリッドサイズ : %d x %d", gridSize_, gridSize_);
            ImGui::Text("Vertex数 : %d", vertexCount_);
            ImGui::Text("Index数 : %d", indexCount_);
            ImGui::Text("Triangle数 : %d", indexCount_ / 3);
        }

        /// ===FFTパラメータ=== ///
        if (ImGui::CollapsingHeader("FFTパラメータ", ImGuiTreeNodeFlags_DefaultOpen)) {
            ImGui::DragFloat2("風の方向", &fftParams_.windDirection.x, 0.01f, -1.0f, 1.0f);
            ImGui::SliderFloat("風の速度", &fftParams_.windSpeed, 0.0f, 50.0f);
            ImGui::SliderFloat("波の振幅", &fftParams_.amplitude, 0.0f, 5.0f);
            ImGui::SliderFloat("チョッピネス", &fftParams_.choppiness, 0.0f, 5.0f);
            ImGui::Text("現在時刻 : %.2f", fftParams_.time);
        }

        /// ===色情報=== ///
        if (ImGui::CollapsingHeader("色")) {
            ImGui::ColorEdit3("海面の基本色", &colorInfo_.seaBase.x);
            ImGui::ColorEdit3("浅瀬の色", &colorInfo_.seaShallow.x);
            ImGui::ColorEdit3("空の色", &colorInfo_.sky.x);
            ImGui::ColorEdit3("深海の色", &colorInfo_.deepWater.x);
            ImGui::SliderFloat("基本色の強度", &colorInfo_.baseStrength, 0.0f, 2.0f);
            ImGui::SliderFloat("水の透明度", &colorInfo_.waterClarity, 0.0f, 1.0f);
            ImGui::SliderFloat("泡の閾値", &colorInfo_.foamThreshold, 0.0f, 3.0f);
        }

        /// ===ライト情報=== ///
        if (ImGui::CollapsingHeader("ライト情報")) {
            ImGui::Text("光沢度: %.2f", light_.shininess);
            ImGui::DragFloat("光沢度", &light_.shininess, 0.1f);
            ImGui::ColorEdit4("ライトの色", &light_.directional.color.x);
            ImGui::SliderFloat3("方向", &light_.directional.direction.x, -1.0f, 1.0f);
            ImGui::SliderFloat("強度", &light_.directional.intensity, 0.0f, 5.0f);
        }

        ImGui::End();
#endif
    }

    ///-------------------------------------------/// 
    /// ColorDataの書き込み
    ///-------------------------------------------///
    void FFTOcean::ColorDataWrite() {
        *oceanColorData_ = colorInfo_;
    }
}
