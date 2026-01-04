#include "AttackEditor.h"
// PlayerWeapon
#include "application/Game/Entity/Player/Weapon/PlayerWeapon.h"
// Service
#include "Engine/System/Service/ServiceLocator.h"
// Line
#include "Engine/Graphics/3d/Line/LineObject3D.h"
// ImGui
#ifdef USE_IMGUI
#include <imgui.h>
#endif

#include <filesystem>

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
AttackEditor::~AttackEditor() {
    line_.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void AttackEditor::Initialize() {
    // Lineの初期化
    line_ = std::make_unique<Line>();

    // デフォルトのファイルパスを設定
    strcpy_s(filePathBuffer_, kDefaultSavePath);

    // 利用可能なプリセットを更新
    UpdateAvailablePresets();

    // デフォルトの攻撃データを作成
    CreateNew();
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void AttackEditor::Update(const float deltaTime) {
    if (!isVisible_) return;

    // プレビューの更新
    if (isPlaying_) {
        UpdateTrajectoryPreview(deltaTime);
    }
}

///-------------------------------------------/// 
/// ImGui描画
///-------------------------------------------///
void AttackEditor::Render() {
#ifdef USE_IMGUI
    if (!isVisible_) return;

    ImGui::Begin("プレイヤー攻撃エディター", &isVisible_, ImGuiWindowFlags_MenuBar);

    // メニューバー
    RenderMenuBar();

    // 左側：攻撃リスト
    ImGui::BeginChild("AttackList", ImVec2(250, 0), true);
    RenderAttackList();
    ImGui::EndChild();

    ImGui::SameLine();

    // 右側：編集パネル
    ImGui::BeginChild("EditPanel", ImVec2(0, 0), true);
    if (selectedAttackIndex_ >= 0 && selectedAttackIndex_ < static_cast<int>(attacks_.size())) {
        AttackData& currentAttack = attacks_[selectedAttackIndex_];

        // タブで分割
        if (ImGui::BeginTabBar("AttackEditorTabs")) {

            if (ImGui::BeginTabItem("基本設定")) {
                RenderBasicSettings(currentAttack);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("タイミング")) {
                RenderTimingSettings(currentAttack);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("軌道")) {
                RenderTrajectorySettings(currentAttack);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("エフェクト")) {
                RenderEffectSettings(currentAttack);
                ImGui::EndTabItem();
            }

            if (ImGui::BeginTabItem("コンボ")) {
                RenderComboSettings(currentAttack);
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }

        ImGui::Separator();

        // 保存ボタン
        if (ImGui::Button("この攻撃を保存", ImVec2(-1, 0))) {
            std::string savePath = std::string(kDefaultSavePath) + currentAttack.attackName + ".json";
            serializer_->SaveToJson(currentAttack, savePath);
        }
    } else {
        ImGui::Text("攻撃を選択してください");
    }
    ImGui::EndChild();

    ImGui::End();

    // プレビューコントロールウィンドウ
    RenderPreviewControl();
#endif
}

///-------------------------------------------/// 
/// プレビュー描画
///-------------------------------------------///
void AttackEditor::DrawPreview() {
    if (!isVisible_ || selectedAttackIndex_ < 0 || selectedAttackIndex_ >= static_cast<int>(attacks_.size())) {
        return;
    }

    AttackData& currentAttack = attacks_[selectedAttackIndex_];

    if (!currentAttack.showTrajectory) {
        return;
    }

    // 武器位置を基準にワールド座標に変換
    Vector3 weaponPos = previewWeaponPosition_;
    std::vector<BezierControlPointData> worldPoints;

    for (const auto& point : currentAttack.trajectoryPoints) {
        BezierControlPointData worldPoint;
        worldPoint.position = weaponPos + point.position;
        worldPoint.time = point.time;
        worldPoints.push_back(worldPoint);
    }

    // ベジェ曲線を描画
    Vector4 trajectoryColor{ currentAttack.trajectoryColor.x, currentAttack.trajectoryColor.y, currentAttack.trajectoryColor.z, 1.0f };
    line_->CreateSmoothCurve(worldPoints, trajectoryColor, currentAttack.curveSegments);

    // 制御点を可視化
    ServiceLocator::GetLineObject3D()->DrawBezierControlPoints(
        worldPoints,
        Vector4{ 1.0f, 1.0f, 0.0f, 1.0f },  // 黄色
        Vector4{ 0.5f, 0.5f, 0.5f, 0.5f },  // 半透明グレー
        0.3f
    );

    // プレビュー用武器の描画（オプション）
    if (previewWeapon_ && isPlaying_) {
        previewWeapon_->Draw(BlendMode::KBlendModeNormal);
    }
}

///-------------------------------------------/// 
/// 表示/非表示切り替え
///-------------------------------------------///
void AttackEditor::ToggleVisibility() {
    isVisible_ = !isVisible_;
}

///-------------------------------------------/// 
/// 表示状態を設定
///-------------------------------------------///
void AttackEditor::SetVisible(bool visible) {
    isVisible_ = visible;
}

///-------------------------------------------/// 
/// JSONファイルに保存
///-------------------------------------------///
void AttackEditor::SaveToJson() {
    if (selectedAttackIndex_ < 0 || selectedAttackIndex_ >= static_cast<int>(attacks_.size())) {
        return;
    }

    AttackData& currentAttack = attacks_[selectedAttackIndex_];
    std::string savePath = std::string(filePathBuffer_) + currentAttack.attackName + ".json";
    serializer_->SaveToJson(currentAttack, savePath);
}

///-------------------------------------------/// 
/// JSONファイルから読み込み
///-------------------------------------------///
void AttackEditor::LoadFromJson() {
    // 実装:  ファイル選択ダイアログから読み込み
}

///-------------------------------------------/// 
/// 新規作成
///-------------------------------------------///
void AttackEditor::CreateNew() {
    AttackData newAttack;
    newAttack.attackName = "NewAttack_" + std::to_string(static_cast<int>(attacks_.size()));
    newAttack.attackID = static_cast<int>(attacks_.size());
    attacks_.push_back(newAttack);
    selectedAttackIndex_ = static_cast<int>(attacks_.size()) - 1;
}

///-------------------------------------------/// 
/// 全ての攻撃を保存
///-------------------------------------------///
void AttackEditor::SaveAllAttacks() {
    for (const auto& attack : attacks_) {
        std::string savePath = std::string(kDefaultSavePath) + attack.attackName + ".json";
        serializer_->SaveToJson(attack, savePath);
    }
}

///-------------------------------------------/// 
/// プリセットフォルダから全JSONを読み込み
///-------------------------------------------///
void AttackEditor::LoadAllAttacks() {
    attacks_.clear();

    if (!std::filesystem::exists(kDefaultSavePath)) {
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(kDefaultSavePath)) {
        if (entry.path().extension() == ".json") {
            AttackData data;
            if (serializer_->LoadFromJson(data, entry.path().string())) {
                attacks_.push_back(data);
            }
        }
    }
}

///-------------------------------------------/// 
/// プレビュー再生開始
///-------------------------------------------///
void AttackEditor::PlayPreview() {
    isPlaying_ = true;
    previewTimer_ = 0.0f;
}

///-------------------------------------------/// 
/// プレビュー停止
///-------------------------------------------///
void AttackEditor::StopPreview() {
    isPlaying_ = false;
}

///-------------------------------------------/// 
/// プレビューリセット
///-------------------------------------------///
void AttackEditor::ResetPreview() {
    previewTimer_ = 0.0f;
}

///-------------------------------------------/// 
/// メニューバーの描画
///-------------------------------------------///
void AttackEditor::RenderMenuBar() {
#ifdef USE_IMGUI
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("ファイル")) {
            if (ImGui::MenuItem("新規作成", "Ctrl+N")) {
                CreateNew();
            }
            if (ImGui::MenuItem("保存", "Ctrl+S")) {
                SaveToJson();
            }
            if (ImGui::MenuItem("全て保存")) {
                SaveAllAttacks();
            }
            if (ImGui::MenuItem("読み込み", "Ctrl+O")) {
                LoadFromJson();
            }
            if (ImGui::MenuItem("全て読み込み")) {
                LoadAllAttacks();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("編集")) {
            if (ImGui::MenuItem("削除", nullptr, false, selectedAttackIndex_ >= 0)) {
                DeleteSelectedAttack();
            }
            if (ImGui::MenuItem("複製", nullptr, false, selectedAttackIndex_ >= 0)) {
                DuplicateSelectedAttack();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
#endif
}

///-------------------------------------------/// 
/// 攻撃リストの描画
///-------------------------------------------///
void AttackEditor::RenderAttackList() {
#ifdef USE_IMGUI
    ImGui::Text("攻撃リスト");
    ImGui::Separator();

    for (int i = 0; i < static_cast<int>(attacks_.size()); ++i) {
        bool isSelected = (selectedAttackIndex_ == i);
        if (ImGui::Selectable(attacks_[i].attackName.c_str(), isSelected)) {
            selectedAttackIndex_ = i;
        }
    }

    ImGui::Separator();
    if (ImGui::Button("+ 新規追加", ImVec2(-1, 0))) {
        CreateNew();
    }
#endif
}

///-------------------------------------------/// 
/// 基本設定UIの描画
///-------------------------------------------///
void AttackEditor::RenderBasicSettings(AttackData& data) {
    data;
#ifdef USE_IMGUI
    ImGui::SeparatorText("基本情報");

    char nameBuffer[256];
    strncpy_s(nameBuffer, data.attackName.c_str(), sizeof(nameBuffer));
    if (ImGui::InputText("攻撃名", nameBuffer, sizeof(nameBuffer))) {
        data.attackName = nameBuffer;
    }

    char descBuffer[512];
    strncpy_s(descBuffer, data.description.c_str(), sizeof(descBuffer));
    if (ImGui::InputTextMultiline("説明", descBuffer, sizeof(descBuffer), ImVec2(-1, 60))) {
        data.description = descBuffer;
    }

    ImGui::InputInt("攻撃ID", &data.attackID);

    ImGui::SeparatorText("プレイヤーの動き");
    ImGui::SliderFloat("移動速度倍率", &data.moveSpeedMultiplier, 0.0f, 1.0f, "%.2f");
    ImGui::DragFloat3("ルートモーション", &data.rootMotion.x, 0.1f);
#endif
}

///-------------------------------------------/// 
/// タイミング設定UIの描画
///-------------------------------------------///
void AttackEditor::RenderTimingSettings(AttackData& data) {
    data;
#ifdef USE_IMGUI
    ImGui::SeparatorText("タイミング設定");
    ImGui::DragFloat("攻撃時間 (秒)", &data.activeDuration, 0.01f, 0.01f, 10.0f, "%.2f");
    ImGui::DragFloat("コンボ受付時間 (秒)", &data.comboWindowTime, 0.01f, 0.0f, 5.0f, "%.2f");
    ImGui::DragFloat("クールダウン (秒)", &data.cooldownTime, 0.01f, 0.0f, 5.0f, "%.2f");
#endif
}

///-------------------------------------------/// 
/// 軌道設定UIの描画
///-------------------------------------------///
void AttackEditor::RenderTrajectorySettings(AttackData& data) {
    data;
#ifdef USE_IMGUI
    ImGui::SeparatorText("ベジェ曲線の制御点");

    ImGui::DragFloat("武器の距離", &data.weaponLength, 0.1f, 1.0f, 50.0f);
    ImGui::DragInt("曲線の分割数", &data.curveSegments, 1, 5, 100);

    // 制御点リスト
    for (int i = 0; i < static_cast<int>(data.trajectoryPoints.size()); ++i) {
        ImGui::PushID(i);

        ImGui::Text("制御点 %d", i);
        ImGui::DragFloat3("位置", &data.trajectoryPoints[i].position.x, 0.1f);
        ImGui::SliderFloat("時間", &data.trajectoryPoints[i].time, 0.0f, 1.0f, "%.2f");

        ImGui::SameLine();
        if (ImGui::Button("削除") && data.trajectoryPoints.size() > 2) {
            data.trajectoryPoints.erase(data.trajectoryPoints.begin() + i);
            ImGui::PopID();
            break;
        }

        ImGui::Separator();
        ImGui::PopID();
    }

    if (ImGui::Button("+ 制御点を追加")) {
        BezierControlPointData newPoint;
        newPoint.position = Vector3{ 0.0f, 0.0f, 0.0f };
        newPoint.time = 0.5f;
        data.trajectoryPoints.push_back(newPoint);
    }

    ImGui::SeparatorText("回転設定");
    ImGui::DragFloat4("開始回転 (Quaternion)", &data.startRotation.x, 0.01f);
    ImGui::DragFloat4("終了回転 (Quaternion)", &data.endRotation.x, 0.01f);
    ImGui::Checkbox("回転もベジェ曲線で補間", &data.useRotationCurve);

    ImGui::SeparatorText("プレビュー");
    ImGui::Checkbox("軌道を表示", &data.showTrajectory);
    ImGui::ColorEdit3("軌道の色", &data.trajectoryColor.x);
#endif
}

///-------------------------------------------/// 
/// エフェクト設定UIの描画
///-------------------------------------------///
void AttackEditor::RenderEffectSettings(AttackData& data) {
    data;
#ifdef USE_IMGUI
    ImGui::SeparatorText("パーティクルエフェクト");

    char particleBuffer[256];
    strncpy_s(particleBuffer, data.particleEffectName.c_str(), sizeof(particleBuffer));
    if (ImGui::InputText("エフェクト名", particleBuffer, sizeof(particleBuffer))) {
        data.particleEffectName = particleBuffer;
    }

    ImGui::DragFloat3("エフェクトオフセット", &data.particleOffset.x, 0.1f);

    ImGui::SeparatorText("カメラシェイク");
    ImGui::DragFloat("シェイク強度", &data.cameraShakeIntensity, 0.01f, 0.0f, 10.0f);
    ImGui::DragFloat("シェイク時間", &data.cameraShakeDuration, 0.01f, 0.0f, 5.0f);

    ImGui::SeparatorText("サウンド");

    char swingBuffer[256];
    strncpy_s(swingBuffer, data.swingSoundName.c_str(), sizeof(swingBuffer));
    if (ImGui::InputText("振りサウンド", swingBuffer, sizeof(swingBuffer))) {
        data.swingSoundName = swingBuffer;
    }

    char hitBuffer[256];
    strncpy_s(hitBuffer, data.hitSoundName.c_str(), sizeof(hitBuffer));
    if (ImGui::InputText("ヒットサウンド", hitBuffer, sizeof(hitBuffer))) {
        data.hitSoundName = hitBuffer;
    }
#endif
}

///-------------------------------------------/// 
/// コンボ設定UIの描画
///-------------------------------------------///
void AttackEditor::RenderComboSettings(AttackData& data) {
    data;
#ifdef USE_IMGUI
    ImGui::SeparatorText("コンボ連携");

    ImGui::Checkbox("次のコンボに繋げる", &data.canComboToNext);

    if (data.canComboToNext) {
        ImGui::InputInt("次のコンボID", &data.nextComboID);

        ImGui::SeparatorText("分岐コンボ");
        ImGui::Text("分岐可能なコンボID:");

        for (int i = 0; i < static_cast<int>(data.branchComboIDs.size()); ++i) {
            ImGui::PushID(i);
            ImGui::InputInt("##branchID", &data.branchComboIDs[i]);
            ImGui::SameLine();
            if (ImGui::Button("削除")) {
                data.branchComboIDs.erase(data.branchComboIDs.begin() + i);
                ImGui::PopID();
                break;
            }
            ImGui::PopID();
        }

        if (ImGui::Button("+ 分岐を追加")) {
            data.branchComboIDs.push_back(-1);
        }
    }
#endif
}

///-------------------------------------------/// 
/// プレビューコントロールの描画
///-------------------------------------------///
void AttackEditor::RenderPreviewControl() {
#ifdef USE_IMGUI
    ImGui::Begin("プレビューコントロール");

    if (ImGui::Button("再生")) {
        PlayPreview();
    }
    ImGui::SameLine();
    if (ImGui::Button("停止")) {
        StopPreview();
    }
    ImGui::SameLine();
    if (ImGui::Button("リセット")) {
        ResetPreview();
    }

    ImGui::Checkbox("自動リプレイ", &autoReplay_);

    ImGui::SeparatorText("プレビュー設定");
    ImGui::DragFloat3("武器位置", &previewWeaponPosition_.x, 0.1f);  // ← 変更

    // 実際の攻撃をプレビュー
    if (ImGui::Button("攻撃プレビュー実行") && previewWeapon_) {
        if (selectedAttackIndex_ >= 0 && selectedAttackIndex_ < static_cast<int>(attacks_.size())) {
            AttackData& currentAttack = attacks_[selectedAttackIndex_];

            // ベジェ曲線の開始点と終了点を取得
            if (currentAttack.trajectoryPoints.size() >= 2) {
                Vector3 startPoint = previewWeaponPosition_ + currentAttack.trajectoryPoints.front().position;
                Vector3 endPoint = previewWeaponPosition_ + currentAttack.trajectoryPoints.back().position;

                // 武器に攻撃を実行させる
                previewWeapon_->StartAttack(
                    startPoint,
                    endPoint,
                    currentAttack.activeDuration,
                    currentAttack.startRotation,
                    currentAttack.endRotation
                );
            }
        }
    }

    ImGui::End();
#endif
}

///-------------------------------------------/// 
/// 選択中の攻撃を削除
///-------------------------------------------///
void AttackEditor::DeleteSelectedAttack() {
    if (selectedAttackIndex_ >= 0 && selectedAttackIndex_ < static_cast<int>(attacks_.size())) {
        attacks_.erase(attacks_.begin() + selectedAttackIndex_);
        selectedAttackIndex_ = -1;
    }
}

///-------------------------------------------/// 
/// 選択中の攻撃を複製
///-------------------------------------------///
void AttackEditor::DuplicateSelectedAttack() {
    if (selectedAttackIndex_ >= 0 && selectedAttackIndex_ < static_cast<int>(attacks_.size())) {
        AttackData copy = attacks_[selectedAttackIndex_];
        copy.attackName += "_Copy";
        copy.attackID = static_cast<int>(attacks_.size());
        attacks_.push_back(copy);
    }
}

///-------------------------------------------/// 
/// 利用可能なプリセットファイルを更新
///-------------------------------------------///
void AttackEditor::UpdateAvailablePresets() {
    availablePresets_.clear();

    if (!std::filesystem::exists(kDefaultSavePath)) {
        std::filesystem::create_directories(kDefaultSavePath);
        return;
    }

    for (const auto& entry : std::filesystem::directory_iterator(kDefaultSavePath)) {
        if (entry.path().extension() == ".json") {
            availablePresets_.push_back(entry.path().filename().string());
        }
    }
}

///-------------------------------------------/// 
/// 軌道のプレビュー更新
///-------------------------------------------///
void AttackEditor::UpdateTrajectoryPreview(const float deltaTime) {
    if (selectedAttackIndex_ < 0 || selectedAttackIndex_ >= static_cast<int>(attacks_.size())) {
        return;
    }

    AttackData& currentAttack = attacks_[selectedAttackIndex_];

    // タイマーを進める
    previewTimer_ += deltaTime;

    // プレビュー用武器の更新
    if (previewWeapon_) {
        previewWeapon_->Update();
    }

    // 攻撃時間を超えたらリセット
    if (previewTimer_ >= currentAttack.activeDuration) {
        if (autoReplay_) {
            previewTimer_ = 0.0f;
        } else {
            isPlaying_ = false;
        }
    }
}