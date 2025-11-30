#include "ParticleEditor.h"
#include "Engine/Graphics/Particle/ParticleFactory.h"
#include "Engine/Graphics/Particle/ParticleGroup.h"
#ifdef USE_IMGUI
// ImGui
#include <imgui.h>
#endif // USE_IMGUI
// c++
#include <fstream>
#include <filesystem>

///-------------------------------------------/// 
/// コンストラクタ・デストラクタ
///-------------------------------------------///
ParticleEditor::ParticleEditor() {
    // デフォルトのファイルパスを設定
    strcpy_s(filePathBuffer_, kDefaultSavePath);
}

ParticleEditor::~ParticleEditor() {
    previewParticle_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void ParticleEditor::Initialize() {
    // 利用可能なリソースを更新
    UpdateAvailableModels();
    UpdateAvailableTextures();
    UpdateAvailablePresets();

    // デフォルトの定義を設定
    currentDefinition_ = ParticleDefinition();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void ParticleEditor::Update() {
    if (!isVisible_) return;

    // プレビューパーティクルの更新
    if (isPlaying_ && previewParticle_) {
        previewParticle_->Update();

        // 終了したら自動リプレイ
        if (autoReplay_ && previewParticle_->IsFinish()) {
            ResetPreview();
            PlayPreview();
        }
    }
}

///-------------------------------------------/// 
/// ImGui描画
///-------------------------------------------///
void ParticleEditor::Render() {
#ifdef USE_IMGUI
    if (!isVisible_) return;

    ImGui::Begin("パーティクルエディター", &isVisible_, ImGuiWindowFlags_MenuBar);

    // メニューバー
    RenderMenuBar();

    // タブで分割
    if (ImGui::BeginTabBar("ParticleEditorTabs")) {

        // 基本設定タブ
        if (ImGui::BeginTabItem("基本設定")) {
            RenderBasicSettings();
            ImGui::EndTabItem();
        }

        // 物理設定タブ
        if (ImGui::BeginTabItem("物理設定")) {
            RenderPhysicsSettings();
            ImGui::EndTabItem();
        }

        // 見た目設定タブ
        if (ImGui::BeginTabItem("見た目設定")) {
            RenderAppearanceSettings();
            ImGui::EndTabItem();
        }

        // 回転設定タブ
        if (ImGui::BeginTabItem("回転設定")) {
            RenderRotationSettings();
            ImGui::EndTabItem();
        }

        // 発生設定タブ
        if (ImGui::BeginTabItem("発生設定")) {
            RenderEmissionSettings();
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::Separator();

    // プレビューコントロール
    RenderPreviewControls();

    // ファイル操作
    RenderFileOperations();

    ImGui::End();
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// プレビュー描画
///-------------------------------------------///
void ParticleEditor::DrawPreview(BlendMode mode) {
    mode;
#ifdef USE_IMGUI
    if (isPlaying_ && previewParticle_) {
        previewParticle_->Draw(mode);
    }
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 表示切り替え
///-------------------------------------------///
void ParticleEditor::ToggleVisibility() {
    isVisible_ = !isVisible_;
}

void ParticleEditor::SetVisible(bool visible) {
    isVisible_ = visible;
}

bool ParticleEditor::IsVisible() const {
    return isVisible_;
}

///-------------------------------------------/// 
/// JSON保存
///-------------------------------------------///
void ParticleEditor::SaveToJson() {
#ifdef USE_IMGUI
    try {
        nlohmann::json j = currentDefinition_.ToJson();

        std::string filepath = filePathBuffer_;
        if (filepath.find(kFileExtension) == std::string::npos) {
            filepath += kFileExtension;
        }

        std::ofstream file(filepath);
        file << j.dump(4); // インデント4でフォーマット
        file.close();

        currentFilePath_ = filepath;

        // プリセット一覧を更新
        UpdateAvailablePresets();

    } catch (const std::exception& e) {
        // エラーハンドリング（コンソールに出力など）
        e;
    }
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// JSON読み込み
///-------------------------------------------///
void ParticleEditor::LoadFromJson() {
#ifdef USE_IMGUI
    try {
        std::string filepath = filePathBuffer_;
        if (filepath.find(kFileExtension) == std::string::npos) {
            filepath += kFileExtension;
        }

        std::ifstream file(filepath);
        nlohmann::json j;
        file >> j;
        file.close();

        currentDefinition_ = ParticleDefinition::FromJson(j);
        currentFilePath_ = filepath;

        // プレビューを更新
        if (isPlaying_) {
            ResetPreview();
            CreatePreviewParticle();
            PlayPreview();
        }

    } catch (const std::exception& e) {
        // エラーハンドリング
        e;
    }
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 新規作成
///-------------------------------------------///
void ParticleEditor::CreateNew() {
#ifdef USE_IMGUI
    currentDefinition_ = ParticleDefinition();
    currentFilePath_.clear();
    strcpy_s(filePathBuffer_, kDefaultSavePath);

    if (isPlaying_) {
        StopPreview();
    }
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// プレビュー再生
///-------------------------------------------///
void ParticleEditor::PlayPreview() {
#ifdef USE_IMGUI
    if (!previewParticle_) {
        CreatePreviewParticle();
    }
    isPlaying_ = true;
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// プレビュー停止
///-------------------------------------------///
void ParticleEditor::StopPreview() {
#ifdef USE_IMGUI
    isPlaying_ = false;
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// プレビューリセット
///-------------------------------------------///
void ParticleEditor::ResetPreview() {
#ifdef USE_IMGUI
    previewParticle_.reset();
    isPlaying_ = false;
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// プレビューパーティクル生成
///-------------------------------------------///
void ParticleEditor::CreatePreviewParticle() {
#ifdef USE_IMGUI
    // ParticleGroupを生成してDefinitionを適用
    previewParticle_ = std::make_unique<ParticleGroup>();
    previewParticle_->Initialze(previewPosition_, currentDefinition_);

    // ★テクスチャが指定されていれば明示的に設定（二重適用になるが安全のため）
    if (!currentDefinition_.appearance.texturePath.empty()) {
        previewParticle_->SetTexture(currentDefinition_.appearance.texturePath);
    }
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// メニューバー描画
///-------------------------------------------///
void ParticleEditor::RenderMenuBar() {
#ifdef USE_IMGUI
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("ファイル")) {
            if (ImGui::MenuItem("新規作成", "Ctrl+N")) {
                CreateNew();
            }
            if (ImGui::MenuItem("開く", "Ctrl+O")) {
                showLoadDialog_ = true;
            }
            if (ImGui::MenuItem("保存", "Ctrl+S")) {
                SaveToJson();
            }
            if (ImGui::MenuItem("名前を付けて保存...")) {
                showSaveDialog_ = true;
            }
            ImGui::Separator();
            if (ImGui::MenuItem("閉じる")) {
                isVisible_ = false;
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("ヘルプ")) {
            if (ImGui::MenuItem("使い方")) {
                // TODO: ヘルプダイアログ表示
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 基本設定UI
///-------------------------------------------///
void ParticleEditor::RenderBasicSettings() {
#ifdef USE_IMGUI
    ImGui::SeparatorText("基本設定");

    // 名前
    char nameBuffer[256];
    strcpy_s(nameBuffer, currentDefinition_.name.c_str());
    if (ImGui::InputText("パーティクル", nameBuffer, sizeof(nameBuffer))) {
        currentDefinition_.name = nameBuffer;
    }
    ImGui::TextDisabled("エミッター登録時に使用する識別名");

    ImGui::Spacing();

    // モデル選択
    if (!availableModels_.empty()) {
        if (ImGui::Combo("モデル", &selectedModelIndex_,
            [](void* data, int idx, const char** out_text) {
                auto& models = *static_cast<std::vector<std::string>*>(data);
                *out_text = models[idx].c_str();
                return true;
            }, &availableModels_, static_cast<int>(availableModels_.size()))) {
            currentDefinition_.modelName = availableModels_[selectedModelIndex_];

            // モデル変更時はプレビューを再生成
            if (isPlaying_) {
                ResetPreview();
                CreatePreviewParticle();
                PlayPreview();
            }
        }
    } else {
        char modelBuffer[256];
        strcpy_s(modelBuffer, currentDefinition_.modelName.c_str());
        if (ImGui::InputText("モデル名", modelBuffer, sizeof(modelBuffer))) {
            currentDefinition_.modelName = modelBuffer;
        }
    }

    // 形状選択
    const char* shapes[] = { "円", "円柱", "なし" };
    int currentShape = static_cast<int>(currentDefinition_.shape);
    if (ImGui::Combo("形状タイプ", &currentShape, shapes, IM_ARRAYSIZE(shapes))) {
        currentDefinition_.shape = static_cast<shapeType>(currentShape);

        // 形状変更時もプレビューを再生成
        if (isPlaying_) {
            ResetPreview();
            CreatePreviewParticle();
            PlayPreview();
        }
    }
    ImGui::TextDisabled("カスタム頂点バッファを使用する形状");

    ImGui::Spacing();

    // 最大インスタンス数
    int maxInstance = static_cast<int>(currentDefinition_.maxInstance);
    if (ImGui::DragInt("最大パーティクル数", &maxInstance, 1, 1, 10000)) {
        currentDefinition_.maxInstance = static_cast<uint32_t>(maxInstance);

        // 最大数変更時もプレビューを再生成
        if (isPlaying_) {
            ResetPreview();
            CreatePreviewParticle();
            PlayPreview();
        }
    }
    ImGui::TextDisabled("同時に表示できる最大数");

    ImGui::Spacing();
    ImGui::Separator();

    // ★ プレビュー位置設定 (拡張版)
    ImGui::SeparatorText("プレビュー設定");

    bool positionChanged = false;

    // XYZ個別スライダー
    positionChanged |= ImGui::DragFloat("発生位置 X", &previewPosition_.x, 0.1f, -100.0f, 100.0f);
    positionChanged |= ImGui::DragFloat("発生位置 Y", &previewPosition_.y, 0.1f, -100.0f, 100.0f);
    positionChanged |= ImGui::DragFloat("発生位置 Z", &previewPosition_.z, 0.1f, -100.0f, 100.0f);

    // まとめてリセット
    if (ImGui::Button("位置をリセット", ImVec2(150, 25))) {
        previewPosition_ = { 0.0f, 0.0f, 0.0f };
        positionChanged = true;
    }

    ImGui::SameLine();

    // プリセット位置
    if (ImGui::Button("地面レベル", ImVec2(150, 25))) {
        previewPosition_ = { 0.0f, 0.0f, 0.0f };
        positionChanged = true;
    }

    if (ImGui::Button("上空位置", ImVec2(150, 25))) {
        previewPosition_ = { 0.0f, 10.0f, 0.0f };
        positionChanged = true;
    }

    ImGui::SameLine();

    if (ImGui::Button("前方位置", ImVec2(150, 25))) {
        previewPosition_ = { 0.0f, 0.0f, 5.0f };
        positionChanged = true;
    }

    // 位置変更時にプレビューパーティクルの位置も更新
    if (positionChanged && previewParticle_) {
        previewParticle_->SetEmitterPosition(previewPosition_);
    }

    ImGui::Spacing();
    ImGui::TextDisabled("Tips: プレビュー再生中でも位置を変更できます");
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 物理設定UI
///-------------------------------------------///
void ParticleEditor::RenderPhysicsSettings() {
#ifdef USE_IMGUI
    ImGui::SeparatorText("物理設定");

    if (ImGui::Checkbox("ランダム速度を使用", &currentDefinition_.physics.useRandomVelocity)) {
        // チェックボックス変更時にプレビューパーティクルに適用
        if (previewParticle_) {
            previewParticle_->SetDefinition(currentDefinition_);
        }
    }
    ImGui::TextDisabled("オフの場合はランダム速度になります");

    ImGui::Spacing();

    bool velocityChanged = false;

    if (currentDefinition_.physics.useRandomVelocity) {
        ImGui::Text("速度範囲");
        velocityChanged |= ImGui::DragFloat3("最小速度", &currentDefinition_.physics.velocityMin.x, 0.1f, -100.0f, 100.0f);
        velocityChanged |= ImGui::DragFloat3("最大速度", &currentDefinition_.physics.velocityMax.x, 0.1f, -100.0f, 100.0f);
    } else {
        velocityChanged |= ImGui::DragFloat3("初期速度", &currentDefinition_.physics.velocityMin.x, 0.1f, -100.0f, 100.0f);
    }

    // ★速度変更時にプレビューに適用
    if (velocityChanged && previewParticle_) {
        previewParticle_->SetParameter(ParticleParameter::VelocityMinX, currentDefinition_.physics.velocityMin.x);
        previewParticle_->SetParameter(ParticleParameter::VelocityMinY, currentDefinition_.physics.velocityMin.y);
        previewParticle_->SetParameter(ParticleParameter::VelocityMinZ, currentDefinition_.physics.velocityMin.z);
        previewParticle_->SetParameter(ParticleParameter::VelocityMaxX, currentDefinition_.physics.velocityMax.x);
        previewParticle_->SetParameter(ParticleParameter::VelocityMaxY, currentDefinition_.physics.velocityMax.y);
        previewParticle_->SetParameter(ParticleParameter::VelocityMaxZ, currentDefinition_.physics.velocityMax.z);
    }

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Text("加速度");
    if (ImGui::DragFloat3("加速度ベクトル", &currentDefinition_.physics.acceleration.x, 0.1f, -50.0f, 50.0f)) {
        if (previewParticle_) {
            previewParticle_->SetParameter(ParticleParameter::AccelerationX, currentDefinition_.physics.acceleration.x);
            previewParticle_->SetParameter(ParticleParameter::AccelerationY, currentDefinition_.physics.acceleration.y);
            previewParticle_->SetParameter(ParticleParameter::AccelerationZ, currentDefinition_.physics.acceleration.z);
        }
    }
    ImGui::TextDisabled("毎フレーム速度に加算される量");

    ImGui::Spacing();

    if (ImGui::DragFloat("重力", &currentDefinition_.physics.gravity, 0.1f, -50.0f, 50.0f)) {
        if (previewParticle_) {
            previewParticle_->SetParameter(ParticleParameter::Gravity, currentDefinition_.physics.gravity);
        }
    }
    ImGui::TextDisabled("Y軸方向の加速度");

    if (ImGui::DragFloat("上方向の初期加速", &currentDefinition_.physics.upwardForce, 0.1f, 0.0f, 50.0f)) {
        if (previewParticle_) {
            previewParticle_->SetParameter(ParticleParameter::UpwardForce, currentDefinition_.physics.upwardForce);
        }
    }
    ImGui::TextDisabled("発生時にY軸速度に加算される値");

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Text("発生範囲");
    if (ImGui::DragFloat("爆発半径", &currentDefinition_.physics.explosionRadius, 0.1f, 0.0f, 20.0f)) {
        if (previewParticle_) {
            previewParticle_->SetParameter(ParticleParameter::ExplosionRadius, currentDefinition_.physics.explosionRadius);
        }
    }
    ImGui::TextDisabled("発生位置からのランダム範囲");
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 見た目設定UI
///-------------------------------------------///
void ParticleEditor::RenderAppearanceSettings() {
#ifdef USE_IMGUI
    ImGui::SeparatorText("見た目設定");

    // 色設定
    ImGui::Text("色設定");
    if (ImGui::Checkbox("色のグラデーション", &currentDefinition_.appearance.useColorGradient)) {
        if (previewParticle_) {
            previewParticle_->SetDefinition(currentDefinition_);
        }
    }

    if (ImGui::ColorEdit4("開始色", &currentDefinition_.appearance.startColor.x)) {
        if (previewParticle_) {
            previewParticle_->SetParameter(ParticleParameter::StartColorR, currentDefinition_.appearance.startColor.x);
            previewParticle_->SetParameter(ParticleParameter::StartColorG, currentDefinition_.appearance.startColor.y);
            previewParticle_->SetParameter(ParticleParameter::StartColorB, currentDefinition_.appearance.startColor.z);
            previewParticle_->SetParameter(ParticleParameter::StartColorA, currentDefinition_.appearance.startColor.w);
        }
    }

    if (currentDefinition_.appearance.useColorGradient) {
        if (ImGui::ColorEdit4("終了色", &currentDefinition_.appearance.endColor.x)) {
            if (previewParticle_) {
                previewParticle_->SetParameter(ParticleParameter::EndColorR, currentDefinition_.appearance.endColor.x);
                previewParticle_->SetParameter(ParticleParameter::EndColorG, currentDefinition_.appearance.endColor.y);
                previewParticle_->SetParameter(ParticleParameter::EndColorB, currentDefinition_.appearance.endColor.z);
                previewParticle_->SetParameter(ParticleParameter::EndColorA, currentDefinition_.appearance.endColor.w);
            }
        }
        ImGui::TextDisabled("寿命に応じて開始色→終了色へ変化");
    }

    ImGui::Spacing();
    ImGui::Separator();

    // スケール設定
    ImGui::Text("スケール設定");
    if (ImGui::Checkbox("スケールアニメーション", &currentDefinition_.appearance.useScaleAnimation)) {
        if (previewParticle_) {
            previewParticle_->SetDefinition(currentDefinition_);
        }
    }

    ImGui::Text("開始スケール範囲");
    bool scaleChanged = false;
    scaleChanged |= ImGui::DragFloat3("最小スケール", &currentDefinition_.appearance.startScaleMin.x, 0.01f, 0.0f, 10.0f);
    scaleChanged |= ImGui::DragFloat3("最大スケール", &currentDefinition_.appearance.startScaleMax.x, 0.01f, 0.0f, 10.0f);

    if (currentDefinition_.appearance.useScaleAnimation) {
        scaleChanged |= ImGui::DragFloat3("終了スケール", &currentDefinition_.appearance.endScale.x, 0.01f, 0.0f, 10.0f);
        ImGui::TextDisabled("寿命に応じて縮小・拡大します");
    }

    if (scaleChanged && previewParticle_) {
        previewParticle_->SetDefinition(currentDefinition_);
    }

    ImGui::Spacing();
    ImGui::Separator();

    // テクスチャ選択
    ImGui::Text("テクスチャ");

    if (!availableTextures_.empty()) {
        if (ImGui::Combo("テクスチャファイル", &selectedTextureIndex_,
            [](void* data, int idx, const char** out_text) {
                auto& textures = *static_cast<std::vector<std::string>*>(data);
                *out_text = textures[idx].c_str();
                return true;
            }, &availableTextures_, static_cast<int>(availableTextures_.size()))) {
            currentDefinition_.appearance.texturePath = availableTextures_[selectedTextureIndex_];

            if (previewParticle_) {
                previewParticle_->SetTexture(currentDefinition_.appearance.texturePath);
            }
        }
    } else {
        char textureBuffer[256];
        strcpy_s(textureBuffer, currentDefinition_.appearance.texturePath.c_str());
        if (ImGui::InputText("テクスチャパス", textureBuffer, sizeof(textureBuffer))) {
            currentDefinition_.appearance.texturePath = textureBuffer;

            if (previewParticle_) {
                previewParticle_->SetTexture(currentDefinition_.appearance.texturePath);
            }
        }
    }

    if (!currentDefinition_.appearance.texturePath.empty()) {
        ImGui::TextDisabled("現在: %s", currentDefinition_.appearance.texturePath.c_str());
    }
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 回転設定UI
///-------------------------------------------///
void ParticleEditor::RenderRotationSettings() {
#ifdef USE_IMGUI
    ImGui::SeparatorText("回転設定");

    if (ImGui::Checkbox("回転を有効化", &currentDefinition_.rotation.enableRotation)) {
        if (previewParticle_) {
            previewParticle_->SetDefinition(currentDefinition_);
        }
    }

    if (currentDefinition_.rotation.enableRotation) {
        ImGui::Spacing();

        if (ImGui::Checkbox("ランダム回転", &currentDefinition_.rotation.randomRotation)) {
            if (previewParticle_) {
                previewParticle_->SetDefinition(currentDefinition_);
            }
        }
        ImGui::TextDisabled("オフの場合は固定速度で回転");

        ImGui::Spacing();

        bool rotationChanged = false;

        if (currentDefinition_.rotation.randomRotation) {
            ImGui::Text("回転速度範囲（ラジアン/秒)");
            rotationChanged |= ImGui::DragFloat3("最小回転速度", &currentDefinition_.rotation.rotationSpeedMin.x, 0.1f, -10.0f, 10.0f);
            rotationChanged |= ImGui::DragFloat3("最大回転速度", &currentDefinition_.rotation.rotationSpeedMax.x, 0.1f, -10.0f, 10.0f);
        } else {
            ImGui::Text("固定回転速度（ラジアン/秒)");
            rotationChanged |= ImGui::DragFloat3("回転速度", &currentDefinition_.rotation.rotationSpeedMin.x, 0.1f, -10.0f, 10.0f);
        }

        if (rotationChanged && previewParticle_) {
            previewParticle_->SetParameter(ParticleParameter::RotationSpeedX, currentDefinition_.rotation.rotationSpeedMin.x);
            previewParticle_->SetParameter(ParticleParameter::RotationSpeedY, currentDefinition_.rotation.rotationSpeedMin.y);
            previewParticle_->SetParameter(ParticleParameter::RotationSpeedZ, currentDefinition_.rotation.rotationSpeedMin.z);
        }

        ImGui::Spacing();
        ImGui::TextDisabled("3.14 ≒ 180度");
    } else {
        ImGui::Spacing();
        ImGui::TextDisabled("回転が無効化されています");
    }
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 発生設定UI
///-------------------------------------------///
void ParticleEditor::RenderEmissionSettings() {
#ifdef USE_IMGUI
    ImGui::SeparatorText("発生設定");

    // 寿命設定
    ImGui::Text("パーティクル寿命 (秒)");
    bool lifetimeChanged = false;
    lifetimeChanged |= ImGui::DragFloat("最小寿命", &currentDefinition_.emission.lifetimeMin, 0.1f, 0.1f, 100.0f);
    lifetimeChanged |= ImGui::DragFloat("最大寿命", &currentDefinition_.emission.lifetimeMax, 0.1f, 0.1f, 100.0f);
    ImGui::TextDisabled("各パーティクルの寿命はこの範囲でランダム");

    if (lifetimeChanged && previewParticle_) {
        previewParticle_->SetParameter(ParticleParameter::LifetimeMin, currentDefinition_.emission.lifetimeMin);
        previewParticle_->SetParameter(ParticleParameter::LifetimeMax, currentDefinition_.emission.lifetimeMax);
    }

    ImGui::Spacing();
    ImGui::Separator();

    // 発生モード
    ImGui::Text("発生モード");
    if (ImGui::Checkbox("バーストモード", &currentDefinition_.emission.isBurst)) {
        if (previewParticle_) {
            // バーストモード切り替え時は再生成
            ResetPreview();
            CreatePreviewParticle();
            if (isPlaying_) {
                PlayPreview();
            }
        }
    }
    ImGui::TextDisabled("一度に大量発生するか、継続的に発生するか");

    ImGui::Spacing();

    if (currentDefinition_.emission.isBurst) {
        // バーストモード設定
        int burstCount = static_cast<int>(currentDefinition_.emission.burstCount);
        if (ImGui::DragInt("バースト数", &burstCount, 1, 1, 10000)) {
            currentDefinition_.emission.burstCount = static_cast<uint32_t>(burstCount);

            if (previewParticle_) {
                previewParticle_->SetParameter(ParticleParameter::BurstCount, static_cast<float>(currentDefinition_.emission.burstCount));
            }
        }
        ImGui::TextDisabled("一度に発生するパーティクルの数");
    } else {
        // 連続発生モード設定
        bool emissionChanged = false;
        emissionChanged |= ImGui::DragFloat("発生レート（個/秒）", &currentDefinition_.emission.emissionRate, 0.1f, 0.1f, 1000.0f);
        ImGui::TextDisabled("1秒あたりに発生するパーティクル数");

        emissionChanged |= ImGui::DragFloat("発生頻度（秒）", &currentDefinition_.emission.frequency, 0.01f, 0.01f, 10.0f);
        ImGui::TextDisabled("パーティクルを発生させる間隔");

        if (emissionChanged && previewParticle_) {
            previewParticle_->SetParameter(ParticleParameter::EmissionRate, currentDefinition_.emission.emissionRate);
            previewParticle_->SetParameter(ParticleParameter::Frequency, currentDefinition_.emission.frequency);
        }

        ImGui::Spacing();
        ImGui::Text("実際の発生数");
        uint32_t actualCount = static_cast<uint32_t>(
            currentDefinition_.emission.emissionRate * currentDefinition_.emission.frequency);
        ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "%u 個ずつ発生", actualCount);
    }
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// プレビューコントロールUI
///-------------------------------------------///
void ParticleEditor::RenderPreviewControls() {
#ifdef USE_IMGUI
    ImGui::SeparatorText("プレビュー");

    // 再生/停止ボタン
    if (isPlaying_) {
        if (ImGui::Button("停止", ImVec2(100, 30))) {
            StopPreview();
        }
    } else {
        if (ImGui::Button("再生", ImVec2(100, 30))) {
            PlayPreview();
        }
    }

    ImGui::SameLine();

    // リセットボタン
    if (ImGui::Button("リセット", ImVec2(100, 30))) {
        ResetPreview();
    }

    ImGui::SameLine();

    // 自動リプレイチェックボックス
    ImGui::Checkbox("自動リプレイ", &autoReplay_);

    // プレビュー状態表示
    if (isPlaying_ && previewParticle_) {
        ImGui::Spacing();
        ImGui::Text("アクティブパーティクル数: %u", previewParticle_->GetActiveParticleCount());
    }
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// ファイル操作UI
///-------------------------------------------///
void ParticleEditor::RenderFileOperations() {
#ifdef USE_IMGUI
    ImGui::Separator();
    ImGui::SeparatorText("ファイル操作");

    // ファイルパス入力
    ImGui::InputText("保存先パス", filePathBuffer_, sizeof(filePathBuffer_));
    ImGui::TextDisabled(".json拡張子は自動で付加されます");

    ImGui::Spacing();

    // 保存・読み込みボタン
    if (ImGui::Button("保存", ImVec2(120, 30))) {
        SaveToJson();
    }

    ImGui::SameLine();

    if (ImGui::Button("読み込み", ImVec2(120, 30))) {
        LoadFromJson();
    }

    // 現在のファイルパスを表示
    if (!currentFilePath_.empty()) {
        ImGui::Spacing();
        ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "現在のファイル:");
        ImGui::TextWrapped("%s", currentFilePath_.c_str());
    }

    // プリセット選択
    if (!availablePresets_.empty()) {
        ImGui::Spacing();
        ImGui::Separator();
        ImGui::Text("プリセット読み込み:");
        if (ImGui::Combo("##Presets", &selectedPresetIndex_,
            [](void* data, int idx, const char** out_text) {
                auto& presets = *static_cast<std::vector<std::string>*>(data);
                *out_text = presets[idx].c_str();
                return true;
            }, &availablePresets_, static_cast<int>(availablePresets_.size()))) {
            strcpy_s(filePathBuffer_, availablePresets_[selectedPresetIndex_].c_str());
            LoadFromJson();
        }
    }
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 利用可能なモデル更新
///-------------------------------------------///
void ParticleEditor::UpdateAvailableModels() {
#ifdef USE_IMGUI
    availableModels_.clear();
    // 実際の実装では、Assets/Modelsディレクトリをスキャンする
    availableModels_.push_back("plane");
    availableModels_.push_back("sphere");
    availableModels_.push_back("cube");
    availableModels_.push_back("triangle");
#endif // USE_IMGUI
}

///-------------------------------------------/// 
/// 利用可能なテクスチャ更新
///-------------------------------------------///
void ParticleEditor::UpdateAvailableTextures() {
#ifdef USE_IMGUI
    availableTextures_.clear();

    // テクスチャの種類
    availableTextures_.push_back("circle");
    availableTextures_.push_back("circle2");
    availableTextures_.push_back("gradationLine");
#endif // USE_IMGUI   
}

///-------------------------------------------/// 
/// 利用可能なプリセット更新
///-------------------------------------------///
void ParticleEditor::UpdateAvailablePresets() {
#ifdef USE_IMGUI
    availablePresets_.clear();

    // Assets/Particlesディレクトリをスキャン
    std::string path = kDefaultSavePath;
    if (std::filesystem::exists(path)) {
        for (const auto& entry : std::filesystem::directory_iterator(path)) {
            if (entry.path().extension() == kFileExtension) {
                availablePresets_.push_back(entry.path().string());
            }
        }
    }
#endif // USE_IMGUI
}