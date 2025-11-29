#include "ParticleEditor.h"
#include "Engine/Graphics/Particle/ParticleFactory.h"
#include "Engine/Graphics/Particle/ParticleGroup.h"
// ImGui
#include <imgui.h>
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
}

///-------------------------------------------/// 
/// プレビュー描画
///-------------------------------------------///
void ParticleEditor::DrawPreview(BlendMode mode) {
    if (isPlaying_ && previewParticle_) {
        previewParticle_->Draw(mode);
    }
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
}

///-------------------------------------------/// 
/// JSON読み込み
///-------------------------------------------///
void ParticleEditor::LoadFromJson() {
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
}

///-------------------------------------------/// 
/// 新規作成
///-------------------------------------------///
void ParticleEditor::CreateNew() {
    currentDefinition_ = ParticleDefinition();
    currentFilePath_.clear();
    strcpy_s(filePathBuffer_, kDefaultSavePath);

    if (isPlaying_) {
        StopPreview();
    }
}

///-------------------------------------------/// 
/// プレビュー再生
///-------------------------------------------///
void ParticleEditor::PlayPreview() {
    if (!previewParticle_) {
        CreatePreviewParticle();
    }
    isPlaying_ = true;
}

///-------------------------------------------/// 
/// プレビュー停止
///-------------------------------------------///
void ParticleEditor::StopPreview() {
    isPlaying_ = false;
}

///-------------------------------------------/// 
/// プレビューリセット
///-------------------------------------------///
void ParticleEditor::ResetPreview() {
    previewParticle_.reset();
    isPlaying_ = false;
}

///-------------------------------------------/// 
/// プレビューパーティクル生成
///-------------------------------------------///
void ParticleEditor::CreatePreviewParticle() {
    // ParticleGroupを生成してDefinitionを適用
    previewParticle_ = std::make_unique<ParticleGroup>();
    previewParticle_->Initialze(previewPosition_, currentDefinition_);
}

///-------------------------------------------/// 
/// メニューバー描画
///-------------------------------------------///
void ParticleEditor::RenderMenuBar() {
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
}

///-------------------------------------------/// 
/// 基本設定UI
///-------------------------------------------///
void ParticleEditor::RenderBasicSettings() {
    ImGui::SeparatorText("基本設定");

    // 名前
    char nameBuffer[256];
    strcpy_s(nameBuffer, currentDefinition_.name.c_str());
    if (ImGui::InputText("パーティクル名", nameBuffer, sizeof(nameBuffer))) {
        currentDefinition_.name = nameBuffer;
    }
    ImGui::TextDisabled("※ エミッター登録時に使用する識別名");

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
    }
    ImGui::TextDisabled("※ カスタム頂点バッファを使用する形状");

    ImGui::Spacing();

    // 最大インスタンス数
    ImGui::DragInt("最大パーティクル数", reinterpret_cast<int*>(&currentDefinition_.maxInstance), 1, 1, 10000);
    ImGui::TextDisabled("※ 同時に表示できる最大数");

    ImGui::Spacing();
    ImGui::Separator();

    // プレビュー位置
    ImGui::Text("プレビュー設定");
    ImGui::DragFloat3("発生位置", &previewPosition_.x, 0.1f);
}

///-------------------------------------------/// 
/// 物理設定UI
///-------------------------------------------///
void ParticleEditor::RenderPhysicsSettings() {
    ImGui::SeparatorText("物理設定");

    ImGui::Checkbox("ランダム速度を使用", &currentDefinition_.physics.useRandomVelocity);
    ImGui::TextDisabled("※ オフの場合は爆発方向ベースの速度になります");

    ImGui::Spacing();

    if (currentDefinition_.physics.useRandomVelocity) {
        ImGui::Text("速度範囲");
        ImGui::DragFloat3("最小速度", &currentDefinition_.physics.velocityMin.x, 0.1f, -100.0f, 100.0f);
        ImGui::DragFloat3("最大速度", &currentDefinition_.physics.velocityMax.x, 0.1f, -100.0f, 100.0f);
    } else {
        ImGui::DragFloat3("初期速度", &currentDefinition_.physics.velocityMin.x, 0.1f, -100.0f, 100.0f);
    }

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Text("加速度");
    ImGui::DragFloat3("加速度ベクトル", &currentDefinition_.physics.acceleration.x, 0.1f, -50.0f, 50.0f);
    ImGui::TextDisabled("※ 毎フレーム速度に加算される量");

    ImGui::Spacing();

    ImGui::DragFloat("重力", &currentDefinition_.physics.gravity, 0.1f, -50.0f, 50.0f);
    ImGui::TextDisabled("※ Y軸方向の加速度（負の値で下向き）");

    ImGui::DragFloat("上方向の初期加速", &currentDefinition_.physics.upwardForce, 0.1f, 0.0f, 50.0f);
    ImGui::TextDisabled("※ 発生時にY軸速度に加算される値");

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Text("発生範囲");
    ImGui::DragFloat("爆発半径", &currentDefinition_.physics.explosionRadius, 0.1f, 0.0f, 20.0f);
    ImGui::TextDisabled("※ 発生位置からのランダム範囲（球状）");
}

///-------------------------------------------/// 
/// 見た目設定UI
///-------------------------------------------///
void ParticleEditor::RenderAppearanceSettings() {
    ImGui::SeparatorText("見た目設定");

    // 色設定
    ImGui::Text("色設定");
    ImGui::Checkbox("色のグラデーション", &currentDefinition_.appearance.useColorGradient);

    ImGui::ColorEdit4("開始色", &currentDefinition_.appearance.startColor.x);
    if (currentDefinition_.appearance.useColorGradient) {
        ImGui::ColorEdit4("終了色", &currentDefinition_.appearance.endColor.x);
        ImGui::TextDisabled("※ 寿命に応じて開始色→終了色へ変化");
    }

    ImGui::Spacing();
    ImGui::Separator();

    // スケール設定
    ImGui::Text("スケール設定");
    ImGui::Checkbox("スケールアニメーション", &currentDefinition_.appearance.useScaleAnimation);

    ImGui::Text("開始スケール範囲");
    ImGui::DragFloat3("最小スケール", &currentDefinition_.appearance.startScaleMin.x, 0.01f, 0.0f, 10.0f);
    ImGui::DragFloat3("最大スケール", &currentDefinition_.appearance.startScaleMax.x, 0.01f, 0.0f, 10.0f);

    if (currentDefinition_.appearance.useScaleAnimation) {
        ImGui::DragFloat3("終了スケール", &currentDefinition_.appearance.endScale.x, 0.01f, 0.0f, 10.0f);
        ImGui::TextDisabled("※ 寿命に応じて縮小・拡大します");
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
        }
    } else {
        char textureBuffer[256];
        strcpy_s(textureBuffer, currentDefinition_.appearance.texturePath.c_str());
        if (ImGui::InputText("テクスチャパス", textureBuffer, sizeof(textureBuffer))) {
            currentDefinition_.appearance.texturePath = textureBuffer;
        }
    }

    // 現在のテクスチャパスを表示
    if (!currentDefinition_.appearance.texturePath.empty()) {
        ImGui::TextDisabled("現在: %s", currentDefinition_.appearance.texturePath.c_str());
    }
}

///-------------------------------------------/// 
/// 回転設定UI
///-------------------------------------------///
void ParticleEditor::RenderRotationSettings() {
    ImGui::SeparatorText("回転設定");

    ImGui::Checkbox("回転を有効化", &currentDefinition_.rotation.enableRotation);

    if (currentDefinition_.rotation.enableRotation) {
        ImGui::Spacing();

        ImGui::Checkbox("ランダム回転", &currentDefinition_.rotation.randomRotation);
        ImGui::TextDisabled("※ オフの場合は固定速度で回転");

        ImGui::Spacing();

        if (currentDefinition_.rotation.randomRotation) {
            ImGui::Text("回転速度範囲（ラジアン/秒）");
            ImGui::DragFloat3("最小回転速度", &currentDefinition_.rotation.rotationSpeedMin.x, 0.1f, -10.0f, 10.0f);
            ImGui::DragFloat3("最大回転速度", &currentDefinition_.rotation.rotationSpeedMax.x, 0.1f, -10.0f, 10.0f);
        } else {
            ImGui::Text("固定回転速度（ラジアン/秒）");
            ImGui::DragFloat3("回転速度", &currentDefinition_.rotation.rotationSpeedMin.x, 0.1f, -10.0f, 10.0f);
        }

        ImGui::Spacing();
        ImGui::TextDisabled("※ 3.14 ≒ 180度");
    } else {
        ImGui::Spacing();
        ImGui::TextDisabled("回転が無効化されています");
    }
}

///-------------------------------------------/// 
/// 発生設定UI
///-------------------------------------------///
void ParticleEditor::RenderEmissionSettings() {
    ImGui::SeparatorText("発生設定");

    // 寿命設定
    ImGui::Text("パーティクル寿命（秒）");
    ImGui::DragFloat("最小寿命", &currentDefinition_.emission.lifetimeMin, 0.1f, 0.1f, 100.0f);
    ImGui::DragFloat("最大寿命", &currentDefinition_.emission.lifetimeMax, 0.1f, 0.1f, 100.0f);
    ImGui::TextDisabled("※ 各パーティクルの寿命はこの範囲でランダム");

    ImGui::Spacing();
    ImGui::Separator();

    // 発生モード
    ImGui::Text("発生モード");
    ImGui::Checkbox("バーストモード", &currentDefinition_.emission.isBurst);
    ImGui::TextDisabled("※ 一度に大量発生するか、継続的に発生するか");

    ImGui::Spacing();

    if (currentDefinition_.emission.isBurst) {
        // バーストモード設定
        ImGui::DragInt("バースト数", reinterpret_cast<int*>(&currentDefinition_.emission.burstCount), 1, 1, 10000);
        ImGui::TextDisabled("※ 一度に発生するパーティクルの数");
    } else {
        // 連続発生モード設定
        ImGui::DragFloat("発生レート（個/秒）", &currentDefinition_.emission.emissionRate, 0.1f, 0.1f, 1000.0f);
        ImGui::TextDisabled("※ 1秒あたりに発生するパーティクル数");

        ImGui::DragFloat("発生頻度（秒）", &currentDefinition_.emission.frequency, 0.01f, 0.01f, 10.0f);
        ImGui::TextDisabled("※ パーティクルを発生させる間隔");

        ImGui::Spacing();
        ImGui::Text("実際の発生数");
        uint32_t actualCount = static_cast<uint32_t>(
            currentDefinition_.emission.emissionRate * currentDefinition_.emission.frequency);
        ImGui::TextColored(ImVec4(0.4f, 1.0f, 0.4f, 1.0f), "%u 個ずつ発生", actualCount);
    }
}

///-------------------------------------------/// 
/// プレビューコントロールUI
///-------------------------------------------///
void ParticleEditor::RenderPreviewControls() {
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
}

///-------------------------------------------/// 
/// ファイル操作UI
///-------------------------------------------///
void ParticleEditor::RenderFileOperations() {
    ImGui::Separator();
    ImGui::SeparatorText("ファイル操作");

    // ファイルパス入力
    ImGui::InputText("保存先パス", filePathBuffer_, sizeof(filePathBuffer_));
    ImGui::TextDisabled("※ .json拡張子は自動で付加されます");

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
}

///-------------------------------------------/// 
/// 利用可能なモデル更新
///-------------------------------------------///
void ParticleEditor::UpdateAvailableModels() {
    availableModels_.clear();
    // 実際の実装では、Assets/Modelsディレクトリをスキャンする
    availableModels_.push_back("plane");
    availableModels_.push_back("sphere");
    availableModels_.push_back("cube");
}

///-------------------------------------------/// 
/// 利用可能なテクスチャ更新
///-------------------------------------------///
void ParticleEditor::UpdateAvailableTextures() {
    availableTextures_.clear();
    // 実際の実装では、Assets/Texturesディレクトリをスキャンする
    availableTextures_.push_back("particle.png");
    availableTextures_.push_back("circle.png");
    availableTextures_.push_back("star.png");
}

///-------------------------------------------/// 
/// 利用可能なプリセット更新
///-------------------------------------------///
void ParticleEditor::UpdateAvailablePresets() {
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
}