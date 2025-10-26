#include "Oshan.h"
// c++
#include <cassert>
#include <cmath>
// Engine
#include "Engine/System/Service/GraphicsResourceGetter.h"
#include "Engine/System/Service/Render.h"
#include "Engine/System/Service/CameraService.h"
// camera
#include "application/Game/Camera/GameCamera.h"
// Math
#include "Math/sMath.h"
#include "Math/MatrixMath.h"

#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI


///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
Oshan::~Oshan() {
    vertex_.reset();
    index_.reset();
    material_.reset();
    wvp_.reset();
    directionallight_.reset();
    camera3D_.reset();
    for (int i = 0; i < 3; ++i) {
        oceanShaderInfo_[i].reset();
    }
    rippleBuffer_.reset();
	oceanColorBuffer_.reset();
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void Oshan::SetWaveInfo(int waveIndex, const Vector3& direction, float amplitude, float length, float speed) {
    if (waveIndex >= 0 && waveIndex < 3 && oceanShaderData_[waveIndex]) {
        oceanShaderData_[waveIndex]->distance = direction;
        oceanShaderData_[waveIndex]->amplitude = amplitude;
        oceanShaderData_[waveIndex]->length = length;
        oceanShaderData_[waveIndex]->speed = speed;
    }
}
void Oshan::SetColorInfo(const OceanColorInfo& colorInfo) {
    colorInfo_ = colorInfo;
}
void Oshan::SetRippleSpeed(float speed) { rippleSpeed_ = speed; }
void Oshan::SetRippleDecay(float decay) { rippleDecay_ = decay; }

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void Oshan::Initialize(int gridSize) {
    /// ===コマンドリストのポインタの取得=== ///
    ID3D12Device* device = GraphicsResourceGetter::GetDXDevice();

    /// ===生成=== ///
    vertex_ = std::make_unique<VertexBuffer3D>();
    index_ = std::make_unique<IndexBuffer3D>();
    material_ = std::make_unique<Material3D>();
    wvp_ = std::make_unique<Transform3D>();
    camera3D_ = std::make_unique<BufferBase>();
	// Light関連バッファ
    directionallight_ = std::make_unique<BufferBase>();
	// Ocean専用バッファ
    for (int i = 0; i < 3; ++i) {
        oceanShaderInfo_[i] = std::make_unique<BufferBase>();
    }
    rippleBuffer_ = std::make_unique<BufferBase>();
    oceanColorBuffer_ = std::make_unique<BufferBase>();


    /// ===グリッドサイズの設定=== ///
    gridSize_ = gridSize;
    vertexCount_ = (gridSize_ + 1) * (gridSize_ + 1);  // 修正：+1が必要
    indexCount_ = gridSize_ * gridSize_ * 6;


    /// ===worldTransform=== ///
    worldTransform_ = { { 10.0f, 1.0f, 10.0f }, { 0.0f, 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 0.0f } };
    uvTransform_ = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f} };

    /// ===vertex=== ///
    vertex_->Create(device, sizeof(VertexData3D) * vertexCount_);
    vertex_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&vertexData_));
    CreateGridMesh();
    vertexBufferView_.BufferLocation = vertex_->GetBuffer()->GetGPUVirtualAddress();
    vertexBufferView_.SizeInBytes = sizeof(VertexData3D) * vertexCount_;
    vertexBufferView_.StrideInBytes = sizeof(VertexData3D);

    /// ===index=== ///
    index_->Create(device, sizeof(uint32_t) * indexCount_);
    index_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&indexData_));

    // インデックスデータの書き込み
    int idx = 0;
    for (int z = 0; z < gridSize_; ++z) {
        for (int x = 0; x < gridSize_; ++x) {
            // 頂点インデックスの計算（gridSize_+1が1行の頂点数）
            int topLeft = z * (gridSize_ + 1) + x;
            int topRight = topLeft + 1;
            int bottomLeft = (z + 1) * (gridSize_ + 1) + x;
            int bottomRight = bottomLeft + 1;

            // 三角形1
            indexData_[idx++] = topLeft;
            indexData_[idx++] = bottomLeft;
            indexData_[idx++] = topRight;

            // 三角形2
            indexData_[idx++] = topRight;
            indexData_[idx++] = bottomLeft;
            indexData_[idx++] = bottomRight;
        }
    }

    indexBufferView_.BufferLocation = index_->GetBuffer()->GetGPUVirtualAddress();
    indexBufferView_.SizeInBytes = sizeof(uint32_t) * indexCount_;
    indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

    /// ===OshanCommon=== ///
	OshanCommon::Initialize(device);

    // ===OceanShaderInfo=== ///
    for (int i = 0; i < 3; ++i) {
        oceanShaderInfo_[i]->Create(device, sizeof(OceanShaderInfo));
        oceanShaderInfo_[i]->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&oceanShaderData_[i]));
    }

    // より細かく穏やかな波のパラメータ
    oceanShaderData_[0]->distance = { 0.56f, 0.0f, 0.41f };
    oceanShaderData_[0]->amplitude = 2.0f;
    oceanShaderData_[0]->length = 20.0f;
    oceanShaderData_[0]->speed = 0.5f;
    oceanShaderData_[0]->time = 0.0f;

    oceanShaderData_[1]->distance = { 0.66f, 0.0f, 0.14f };
    oceanShaderData_[1]->amplitude = 1.5f;
    oceanShaderData_[1]->length = 15.0f;
    oceanShaderData_[1]->speed = 0.3f;
    oceanShaderData_[1]->time = 0.0f;

    oceanShaderData_[2]->distance = { -0.22f, 0.0f, 0.66f };
    oceanShaderData_[2]->amplitude = 1.0f;
    oceanShaderData_[2]->length = 10.0f;
    oceanShaderData_[2]->speed = 0.4f;
    oceanShaderData_[2]->time = 0.0f;

    /// ===RippleBuffer=== ///
    rippleBuffer_->Create(device, sizeof(RippleBufferForGPU));
    rippleBuffer_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&rippleBufferData_));

    // 波紋の初期化
    for (int i = 0; i < 8; ++i) {
        ripples_[i].isActive = false;
        ripples_[i].position = { 0.0f, 0.0f };
        ripples_[i].startTime = 0.0f;
        ripples_[i].intensity = 0.0f;
    }
    rippleBufferData_->activeCount = 0;
    rippleBufferData_->currentTime = 0.0f;
    rippleBufferData_->rippleSpeed = rippleSpeed_;
    rippleBufferData_->rippleDecay = rippleDecay_;

    /// ===OceanColorBuffer=== ///
    oceanColorBuffer_->Create(device, sizeof(OceanColorInfo));
    oceanColorBuffer_->GetBuffer()->Map(0, nullptr, reinterpret_cast<void**>(&oceanColorData_));
    *oceanColorData_ = colorInfo_;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void Oshan::Update() {
    /// ===カメラの設定=== ///
    camera_ = CameraService::GetActiveCamera().get();

    /// ===時間の更新=== ///
    currentTime_ += 1.0f / 60.0f;
    for (int i = 0; i < 3; ++i) {
        if (oceanShaderData_[i]) {
            oceanShaderData_[i]->time = currentTime_;
        }
    }

    /// ===波紋の更新=== ///
    UpdateRipples();

    /// ===データの書き込み=== ///
	OshanCommon::Update();
    OceanShaderDataWrite();
    RippleDataWrite();
    ColorDataWrite();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void Oshan::Draw(BlendMode mode) {
    /// ===コマンドリストのポインタの取得=== ///
    ID3D12GraphicsCommandList* commandList = GraphicsResourceGetter::GetDXCommandList();

    /// ===コマンドリストに設定=== ///
    // PSOの設定（オーシャン用のPipelineを使用）
    Render::SetPSO(commandList, PipelineType::PrimitiveOshan, mode);

    // Viewの設定
    commandList->IASetVertexBuffers(0, 1, &vertexBufferView_);
    commandList->IASetIndexBuffer(&indexBufferView_);

    // MaterialBufferの設定
    commandList->SetGraphicsRootConstantBufferView(0, material_->GetBuffer()->GetGPUVirtualAddress());
    // wvpMatrixBufferの設定
    commandList->SetGraphicsRootConstantBufferView(1, wvp_->GetBuffer()->GetGPUVirtualAddress());
    // DirectionlLightの設定
    commandList->SetGraphicsRootConstantBufferView(2, directionallight_->GetBuffer()->GetGPUVirtualAddress());
    // CameraBufferの設定
    commandList->SetGraphicsRootConstantBufferView(3, camera3D_->GetBuffer()->GetGPUVirtualAddress());

    // OceanShaderInfoの設定（register b1, b2, b3）
    commandList->SetGraphicsRootConstantBufferView(4, oceanShaderInfo_[0]->GetBuffer()->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(5, oceanShaderInfo_[1]->GetBuffer()->GetGPUVirtualAddress());
    commandList->SetGraphicsRootConstantBufferView(6, oceanShaderInfo_[2]->GetBuffer()->GetGPUVirtualAddress());

    // RippleBufferの設定（register b4）
    commandList->SetGraphicsRootConstantBufferView(7, rippleBuffer_->GetBuffer()->GetGPUVirtualAddress());

    // OceanColorBufferの設定（register b2 for PS）
    commandList->SetGraphicsRootConstantBufferView(8, oceanColorBuffer_->GetBuffer()->GetGPUVirtualAddress());

    // 描画（DrawCall）
    commandList->DrawIndexedInstanced(indexCount_, 1, 0, 0, 0);
}

///-------------------------------------------/// 
/// ImGui
///-------------------------------------------///
void Oshan::ShowImGui() {
#ifdef USE_IMGUI
    ImGui::Begin("OshanInfo");

    // === Transform情報 === //
    if (ImGui::CollapsingHeader("Transform", ImGuiTreeNodeFlags_DefaultOpen)) {
		ImGui::DragFloat3("Translate", &worldTransform_.translate.x, 0.1f);
        ImGui::DragFloat4("Rotate", &worldTransform_.rotate.x, 0.01f);
		ImGui::DragFloat3("Scale", &worldTransform_.scale.x, 0.1f);
    }

    // === グリッド情報 === //
    if (ImGui::CollapsingHeader("Grid Info")) {
        ImGui::Text("Grid Size: %d x %d", gridSize_, gridSize_);
        ImGui::Text("Vertex Count: %d", vertexCount_);
        ImGui::Text("Index Count: %d", indexCount_);
        ImGui::Text("Triangle Count: %d", indexCount_ / 3);
    }

    // === 波情報 === //
    if (ImGui::CollapsingHeader("Wave Parameters", ImGuiTreeNodeFlags_DefaultOpen)) {
        for (int i = 0; i < 3; ++i) {
            ImGui::PushID(i);
            if (ImGui::TreeNode("Wave", "Wave %d", i)) {
                if (oceanShaderData_[i]) {
                    ImGui::Text("Direction: (%.2f, %.2f, %.2f)",
                        oceanShaderData_[i]->distance.x,
                        oceanShaderData_[i]->distance.y,
                        oceanShaderData_[i]->distance.z);
                    ImGui::Text("Amplitude: %.2f", oceanShaderData_[i]->amplitude);
                    ImGui::Text("Length: %.2f", oceanShaderData_[i]->length);
                    ImGui::Text("Speed: %.2f", oceanShaderData_[i]->speed);
                    ImGui::Text("Time: %.2f", oceanShaderData_[i]->time);

                    ImGui::Separator();

                    if (ImGui::DragFloat3("Edit Direction", &oceanShaderData_[i]->distance.x, 0.01f)) {
                        // 値が変更された
                    }

                    // 編集可能なスライダー
                    if (ImGui::SliderFloat("Edit Amplitude", &oceanShaderData_[i]->amplitude, 0.0f, 2.0f)) {
                        // 値が変更された
                    }
                    if (ImGui::SliderFloat("Edit Length", &oceanShaderData_[i]->length, 1.0f, 30.0f)) {
                        // 値が変更された
                    }
                    if (ImGui::SliderFloat("Edit Speed", &oceanShaderData_[i]->speed, 0.5f, 10.0f)) {
                        // 値が変更された
                    }
                }
                ImGui::TreePop();
            }
            ImGui::PopID();
        }
    }

    // === 波紋情報 === //
    if (ImGui::CollapsingHeader("Ripple Info", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Text("Active Ripples: %d / 8", rippleBufferData_->activeCount);
        ImGui::Text("Ripple Speed: %.2f", rippleSpeed_);
        ImGui::Text("Ripple Decay: %.2f", rippleDecay_);
        ImGui::Text("Current Time: %.2f", currentTime_);

        ImGui::Separator();

        // 編集可能なスライダー
        if (ImGui::SliderFloat("Edit Speed", &rippleSpeed_, 1.0f, 10.0f)) {
            rippleBufferData_->rippleSpeed = rippleSpeed_;
        }
        if (ImGui::SliderFloat("Edit Decay", &rippleDecay_, 0.1f, 3.0f)) {
            rippleBufferData_->rippleDecay = rippleDecay_;
        }

        ImGui::Separator();

        // アクティブな波紋の詳細
        if (ImGui::TreeNode("Active Ripples Details")) {
            for (int i = 0; i < 8; ++i) {
                if (ripples_[i].isActive) {
                    float elapsed = currentTime_ - ripples_[i].startTime;
                    ImGui::Text("Ripple %d: Pos(%.1f, %.1f) Time:%.2fs Intensity:%.2f",
                        i,
                        ripples_[i].position.x,
                        ripples_[i].position.y,
                        elapsed,
                        ripples_[i].intensity);
                }
            }
            ImGui::TreePop();
        }

        // 波紋クリアボタン
        if (ImGui::Button("Clear All Ripples")) {
            ClearRipples();
        }
    }

    // === 色情報 === //
    if (ImGui::CollapsingHeader("Color Parameters")) {
        ImGui::ColorEdit3("Sea Base", &colorInfo_.seaBase.x);
        ImGui::ColorEdit3("Sea Shallow", &colorInfo_.seaShallow.x);
        ImGui::ColorEdit3("Sky", &colorInfo_.sky.x);
        ImGui::ColorEdit3("Deep Water", &colorInfo_.deepWater.x);

        ImGui::Separator();

        ImGui::SliderFloat("Base Strength", &colorInfo_.baseStrength, 0.0f, 2.0f);
        ImGui::SliderFloat("Height Offset", &colorInfo_.heightOffset, 0.0f, 1.0f);

        // 透明度調整を追加
        if (ImGui::SliderFloat("Water Clarity (Transparency)", &colorInfo_.waterClarity, 0.0f, 1.0f)) {
            // 値が変更された時の処理
            // 1.0に近いほど透明、0.0に近いほど不透明
        }

        ImGui::SliderFloat("Foam Threshold", &colorInfo_.foamThreshold, 0.0f, 3.0f);
    }

    // === ライト情報 === //
    if (ImGui::CollapsingHeader("Light Info")) {
        ImGui::Text("Shininess: %.2f", light_.shininess);
        ImGui::ColorEdit4("Directional Color", &light_.directional.color.x);
        ImGui::SliderFloat3("Direction", &light_.directional.direction.x, -1.0f, 1.0f);
        ImGui::SliderFloat("Intensity", &light_.directional.intensity, 0.0f, 5.0f);
    }

    // === 衝突記録 === //
    if (ImGui::CollapsingHeader("Collision Records")) {
        ImGui::Text("Total Hit Records: %zu", hitRecords_.size());

        if (ImGui::TreeNode("Hit Details")) {
            for (size_t i = 0; i < hitRecords_.size(); ++i) {
                const auto& record = hitRecords_[i];
                ImGui::Text("Object %d: Pos(%.1f, %.1f, %.1f) Hitting:%s",
                    record.objectID,
                    record.lastPosition.x,
                    record.lastPosition.y,
                    record.lastPosition.z,
                    record.wasHitting ? "Yes" : "No");
            }
            ImGui::TreePop();
        }

        // 衝突記録クリアボタン
        if (ImGui::Button("Clear Hit Records")) {
            hitRecords_.clear();
        }
    }

    // === パフォーマンス情報 === //
    if (ImGui::CollapsingHeader("Performance")) {
        ImGui::Text("Draw Calls: 1");
        ImGui::Text("Triangles: %d", indexCount_ / 3);
        ImGui::Text("Vertices: %d", vertexCount_);

        // メモリ使用量の概算
        size_t vertexMemory = sizeof(VertexData3D) * vertexCount_;
        size_t indexMemory = sizeof(uint32_t) * indexCount_;
        size_t totalMemory = vertexMemory + indexMemory;

        ImGui::Text("Vertex Buffer: %.2f KB", vertexMemory / 1024.0f);
        ImGui::Text("Index Buffer: %.2f KB", indexMemory / 1024.0f);
        ImGui::Text("Total Geometry: %.2f KB", totalMemory / 1024.0f);
    }

    // === テスト機能 === //
    if (ImGui::CollapsingHeader("Test Functions")) {
        static float testIntensity = 1.0f;
        static float testPosX = 0.0f;
        static float testPosZ = 0.0f;

        ImGui::SliderFloat("Test Intensity", &testIntensity, 0.1f, 3.0f);
        ImGui::SliderFloat("Test Pos X", &testPosX, -50.0f, 50.0f);
        ImGui::SliderFloat("Test Pos Z", &testPosZ, -50.0f, 50.0f);

        if (ImGui::Button("Add Test Ripple")) {
            AddRipple({ testPosX, 0.0f, testPosZ }, testIntensity);
        }

        ImGui::SameLine();

        if (ImGui::Button("Random Ripple")) {
            float randX = static_cast<float>(rand() % 100 - 50);
            float randZ = static_cast<float>(rand() % 100 - 50);
            float randIntensity = 0.5f + (rand() % 100) / 100.0f * 1.5f;
            AddRipple({ randX, 0.0f, randZ }, randIntensity);
        }
    }

    ImGui::End();
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// グリッドメッシュの生成
///-------------------------------------------///
void Oshan::CreateGridMesh() {
    // より広い範囲のグリッドを生成
    float gridWidth = 100.0f;   // グリッドの幅
    float gridDepth = 100.0f;   // グリッドの奥行き
    float step = gridWidth / gridSize_;  // 各グリッドのサイズ

    int vertexIndex = 0;
    for (int i = 0; i <= gridSize_; i++) {  // <= に変更（頂点数は gridSize_+1）
        for (int j = 0; j <= gridSize_; j++) {
            // 中心を原点とする座標系
            float x = -gridWidth * 0.5f + j * step;
            float z = -gridDepth * 0.5f + i * step;

            // UV座標は0.0～1.0の範囲
            float u = (float)j / gridSize_;
            float v = (float)i / gridSize_;

            vertexData_[vertexIndex] = {
                {x, 0.0f, z, 1.0f},      // 位置
                {u, v},                   // UV
                {0.0f, 1.0f, 0.0f}       // 法線（初期値、シェーダーで計算される）
            };

            vertexIndex++;
        }
    }
}

///-------------------------------------------/// 
/// OceanShaderDataの書き込み
///-------------------------------------------///
void Oshan::OceanShaderDataWrite() {
    // 既にUpdateで時間は更新済み
}

///-------------------------------------------/// 
/// RippleDataの書き込み
///-------------------------------------------///
void Oshan::RippleDataWrite() {
    rippleBufferData_->currentTime = currentTime_;
    rippleBufferData_->rippleSpeed = rippleSpeed_;
    rippleBufferData_->rippleDecay = rippleDecay_;
}

///-------------------------------------------/// 
/// ColorDataの書き込み
///-------------------------------------------///
void Oshan::ColorDataWrite() {
    *oceanColorData_ = colorInfo_;
}

///-------------------------------------------/// 
/// 波紋の更新
///-------------------------------------------///
void Oshan::UpdateRipples() {
    int activeCount = 0;

    for (int i = 0; i < 8; ++i) {
        if (ripples_[i].isActive) {
            float elapsed = currentTime_ - ripples_[i].startTime;

            // 波紋の寿命チェック（3秒程度で消える）
            if (elapsed > 3.0f) {
                ripples_[i].isActive = false;
            } else {
                rippleBufferData_->ripples[activeCount].position = ripples_[i].position;
                rippleBufferData_->ripples[activeCount].startTime = ripples_[i].startTime;
                rippleBufferData_->ripples[activeCount].intensity = ripples_[i].intensity;
                activeCount++;
            }
        }
    }

    rippleBufferData_->activeCount = activeCount;
}

///-------------------------------------------/// 
/// 波紋を追加
///-------------------------------------------///
void Oshan::AddRipple(const Vector3& worldPosition, float intensity) {
    // 空いているスロットを探す
    for (int i = 0; i < 8; ++i) {
        if (!ripples_[i].isActive) {
            ripples_[i].position = { worldPosition.x, worldPosition.z };
            ripples_[i].startTime = currentTime_;
            ripples_[i].intensity = intensity;
            ripples_[i].isActive = true;
            break;
        }
    }
}

///-------------------------------------------/// 
/// 衝突時に呼び出す関数
///-------------------------------------------///
void Oshan::OnCollision(const Vector3& collisionPosition, float collisionTime, float intensity) {
    // 波紋を生成（衝突時刻を指定して開始）
    for (int i = 0; i < 8; ++i) {
        if (!ripples_[i].isActive) {
            ripples_[i].position = { collisionPosition.x, collisionPosition.z };
            ripples_[i].startTime = collisionTime;
            ripples_[i].intensity = intensity;
            ripples_[i].isActive = true;
            break;
        }
    }
}

///-------------------------------------------/// 
/// オブジェクト衝突時の波紋生成
///-------------------------------------------///
void Oshan::OnObjectHit(const Vector3& worldPosition, int objectID) {
    HitRecord* record = FindHitRecord(objectID);

    if (record == nullptr) {
        // 新しい物体の衝突
        HitRecord newRecord;
        newRecord.objectID = objectID;
        newRecord.lastPosition = worldPosition;
        newRecord.wasHitting = true;
        hitRecords_.push_back(newRecord);

        // 波紋を生成
        AddRipple(worldPosition, 1.0f);
    } else {
        // 既存の物体
        if (!record->wasHitting) {
            // 前回は当たっていなかった = 新しい衝突
            record->wasHitting = true;
            record->lastPosition = worldPosition;
            AddRipple(worldPosition, 1.0f);
        } else {
            // 連続して当たっている場合、位置が大きく変わった場合のみ新しい波紋
            Vector3 diff = {
                worldPosition.x - record->lastPosition.x,
                worldPosition.y - record->lastPosition.y,
                worldPosition.z - record->lastPosition.z
            };
            float distanceSq = diff.x * diff.x + diff.y * diff.y + diff.z * diff.z;

            if (distanceSq > 4.0f) { // 2.0f以上離れた場合
                record->lastPosition = worldPosition;
                AddRipple(worldPosition, 1.0f);
            }
        }
    }
}

///-------------------------------------------/// 
/// 衝突記録を探す
///-------------------------------------------///
Oshan::HitRecord* Oshan::FindHitRecord(int objectID) {
    for (auto& record : hitRecords_) {
        if (record.objectID == objectID) {
            return &record;
        }
    }
    return nullptr;
}

///-------------------------------------------/// 
/// すべての波紋をクリア
///-------------------------------------------///
void Oshan::ClearRipples() {
    for (int i = 0; i < 8; ++i) {
        ripples_[i].isActive = false;
    }
    rippleBufferData_->activeCount = 0;
    hitRecords_.clear();
}