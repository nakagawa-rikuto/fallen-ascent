#pragma once
/// ===Include=== ///
#include "Engine/Scene/IScene.h"
// Editor
#include "application/Game/Entity/Player/Editor/AttackEditor.h"
// Line 
#include "application/Drawing/3d/Line.h"
// Player
#include "application/Game/Entity/Player/Weapon/PlayerWeapon.h"
// c++
#include <memory>

///=====================================================/// 
/// 攻撃エディターシーン
///=====================================================///
class AttackEditorScene : public IScene {
public:

    AttackEditorScene() = default;
    ~AttackEditorScene();

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Initialize() override;

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画処理
    /// </summary>
    void Draw() override;

private:
    /// ===Line=== ///
    std::unique_ptr<Line> line_;
    /// ===エディター=== ///
    std::unique_ptr<AttackEditor> attackEditor_;
    /// ===プレビュー用プレイヤー=== ///
    std::unique_ptr<PlayerWeapon> previewWeapon_;

    /// ===エディター設定=== ///
    bool showHelp_ = false;              // ヘルプ表示フラグ
    bool showStats_ = true;              // 統計情報表示フラグ
    bool showGrid_ = true;               // グリッド表示フラグ

	float deltaTime_ = 1.0f / 60.0f;    // デルタタイム

private:

    /// <summary>
    /// メニューバーの描画
    /// </summary>
    void RenderMenuBar();

    /// <summary>
    /// ヘルプウィンドウの描画
    /// </summary>
    void RenderHelpWindow();

    /// <summary>
    /// 統計情報ウィンドウの描画
    /// </summary>
    void RenderStatsWindow();
};