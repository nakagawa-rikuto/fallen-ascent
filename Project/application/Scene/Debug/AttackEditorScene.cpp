#include "AttackEditorScene.h"
// SceneManager
#include "Engine/System/Managers/SceneManager.h"
// ImGui
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
AttackEditorScene::~AttackEditorScene() {
    attackEditor_.reset();
    previewWeapon_.reset();
    line_.reset();
    // ISceneのデストラクタ
    IScene::~IScene();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void AttackEditorScene::Initialize() {
    // ISceneの初期化(デフォルトカメラとカメラマネージャ)
    IScene::Initialize();

    /// ===エディターの初期化=== ///
    attackEditor_ = std::make_unique<AttackEditor>();
    attackEditor_->Initialize();
    attackEditor_->SetVisible(true);

    /// ===Line=== ///
    line_ = std::make_unique<Line>();

    /// ===プレビュー用プレイヤーの初期化=== ///
    previewWeapon_ = std::make_unique<PlayerWeapon>();
    previewWeapon_->Initialize();

    // エディターにプレビュー用プレイヤーとカメラを設定
    attackEditor_->SetPreviewPlayer(previewWeapon_.get());

    // デフォルトカメラの位置を設定
    defaultCamera_->SetTranslate(cameraPosition_);
    defaultCamera_->SetRotate(cameraRotation_);
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void AttackEditorScene::Update() {
    /// ===エディターの更新=== ///
    if (attackEditor_) {
        attackEditor_->Update(deltaTime_);
    }

    /// ===プレビュー用プレイヤーの更新=== ///
    if (previewWeapon_) {
		previewWeapon_->Update();
    }

    /// ===ImGui描画=== ///
#ifdef USE_IMGUI
    RenderMenuBar();
    /// ===デバッグ時のカメラ=== ///
    defaultCamera_->ImGuiUpdate();
	defaultCamera_->DebugUpdate();

    // エディターメインウィンドウ
    attackEditor_->Render();

    // サブウィンドウ
    if (showHelp_) {
        RenderHelpWindow();
    }
    if (showStats_) {
        RenderStatsWindow();
    }
#endif // USE_IMGUI

    /// ===ISceneの更新=== ///
    IScene::Update();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void AttackEditorScene::Draw() {
    /// ===グリッドの描画=== ///
    if (showGrid_) {
        line_->DrawGrid({ 0.0f, 0.0f, 0.0f }, { 500.0f, 0.0f, 500.0f }, 100, { 1.0f, 1.0f, 1.0f, 1.0f });
    }

    /// ===プレビュー用プレイヤーの描画=== ///
    if (previewWeapon_) {
        previewWeapon_->Draw(BlendMode::KBlendModeNormal);
    }

    /// ===エディタープレビューの描画=== ///
    if (attackEditor_) {
        attackEditor_->DrawPreview();
    }

    /// ===ISceneの描画=== ///
    IScene::Draw();
}

///-------------------------------------------/// 
/// メニューバーの描画
///-------------------------------------------///
void AttackEditorScene::RenderMenuBar() {
#ifdef USE_IMGUI
    if (ImGui::BeginMainMenuBar()) {
        if (ImGui::BeginMenu("表示")) {
            ImGui::Checkbox("ヘルプ", &showHelp_);
            ImGui::Checkbox("統計情報", &showStats_);
            ImGui::Checkbox("グリッド", &showGrid_);
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("エディター")) {
            if (ImGui::MenuItem("攻撃エディターを表示/非表示")) {
                attackEditor_->ToggleVisibility();
            }
            ImGui::EndMenu();
        }
        ImGui::EndMainMenuBar();
    }
#endif
}

///-------------------------------------------/// 
/// ヘルプウィンドウの描画
///-------------------------------------------///
void AttackEditorScene::RenderHelpWindow() {
#ifdef USE_IMGUI
    ImGui::Begin("ヘルプ", &showHelp_);

    ImGui::Text("攻撃エディターの使い方:");
    ImGui::Separator();
    ImGui::BulletText("左側のリストから攻撃を選択");
    ImGui::BulletText("右側のタブで各設定を編集");
    ImGui::BulletText("「保存」ボタンでJSONに保存");
    ImGui::BulletText("プレビューで軌道を確認");

    ImGui::Separator();
    ImGui::Text("ショートカットキー:");
    ImGui::BulletText("Ctrl+N:  新規作成");
    ImGui::BulletText("Ctrl+S: 保存");
    ImGui::BulletText("Ctrl+O: 読み込み");

    ImGui::End();
#endif
}

///-------------------------------------------/// 
/// 統計情報ウィンドウの描画
///-------------------------------------------///
void AttackEditorScene::RenderStatsWindow() {
#ifdef USE_IMGUI
    ImGui::Begin("統計情報", &showStats_);

    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
    ImGui::Text("フレーム時間: %.3f ms", 1000.0f / ImGui::GetIO().Framerate);
    ImGui::Separator();
    ImGui::Text("デルタタイム: %.3f", deltaTime_);

    ImGui::End();
#endif
}