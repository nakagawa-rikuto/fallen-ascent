#pragma once
/// ===Include=== ///
#include "Data/AttackData.h"
#include "application/Drawing/3d/Line.h"
// Serializer
#include "AttackDataSerializer.h"
// c++
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

/// ===前方宣言=== ///
class PlayerWeapon;

///=====================================================/// 
/// プレイヤー攻撃エディター
/// ImGuiを使用した攻撃編集UI
///=====================================================///
class AttackEditor {
public:

    AttackEditor() = default;
    ~AttackEditor();

    /// <summary>
    /// 初期化処理
    /// </summary>
    void Initialize();

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update(const float deltaTime);

    /// <summary>
    /// ImGUI描画処理
    /// </summary>
    void Render();

    /// <summary>
    /// プレビュー描画処理（ゲーム画面に表示）
    /// </summary>
    void DrawPreview();

    /// <summary>
    /// エディターの表示/非表示切り替え
    /// </summary>
    void ToggleVisibility();

    /// <summary>
    /// エディターの表示状態を設定
    /// </summary>
    void SetVisible(bool visible);

    /// <summary>
    /// エディターが表示されているか
    /// </summary>
    bool IsVisible() const { return isVisible_; }

    /// <summary>
    /// プレビュー用プレイヤーの設定
    /// </summary>
    void SetPreviewPlayer(PlayerWeapon* player) { previewWeapon_ = player; }

private:
    /// ===エディター状態=== ///
    bool isVisible_ = false;                      // 表示フラグ
    std::vector<AttackData> attacks_;             // 攻撃データリスト
    int selectedAttackIndex_ = -1;                // 選択中の攻撃インデックス

    /// ===ファイル管理=== ///
    static constexpr const char* kDefaultSavePath = "Resource/Json/Attacks";
    char filePathBuffer_[256];                    // ファイルパス入力バッファ
    std::vector<std::string> availablePresets_;   // 利用可能なプリセットリスト
    std::unordered_map<int, std::string> attackFilePaths_; // 

    /// ===プレビュー=== ///
    bool isPlaying_ = false;                      // プレビュー再生中フラグ
    bool autoReplay_ = false;                     // 自動リプレイフラグ
    float previewTimer_ = 0.0f;                   // プレビュータイマー
    Vector3 previewWeaponPosition_{ 0.0f, 0.0f, 0.0f };  // プレビュー用プレイヤー位置

    PlayerWeapon* previewWeapon_ = nullptr;             // プレビュー用プレイヤー

    /// ===Line描画=== ///
    std::unique_ptr<Line> line_;                  // 線描画オブジェクト
    
    /// ===Serializer=== ///
	std::unique_ptr<AttackDataSerializer> serializer_; // シリアライザ

    /// ===回転編集用（オイラー角）=== ///  
    struct RotationEditData {
        Vector3 startEuler = { 0.0f, 0.0f, 0.0f };  // 開始回転（度数法）
        Vector3 endEuler = { 0.0f, 0.0f, 0.0f };    // 終了回転（度数法）
    };
    std::unordered_map<int, RotationEditData> rotationEditMap_;  

private:

    /// ===ファイル操作=== ///

    /// <summary>
    /// 選択中の攻撃を上書き保存
    /// </summary>
    void SaveCurrent();

    /// <summary>
    /// 選択中の攻撃を名前を付けて保存
    /// </summary>
    void SaveCurrentAs();

    /// <summary>
    /// JSONファイルに保存
    /// </summary>
    void SaveToJson();

    /// <summary>
    /// JSONファイルから読み込み
    /// </summary>
    void LoadFromJson();

    /// <summary>
    /// 新規作成
    /// </summary>
    void CreateNew();

    /// <summary>
    /// 全ての攻撃を保存
    /// </summary>
    void SaveAllAttacks();

    /// <summary>
    /// プリセットフォルダから全JSONを読み込み
    /// </summary>
    void LoadAllAttacks();

    /// ===プレビュー制御=== ///

    /// <summary>
    /// プレビュー再生開始
    /// </summary>
    void PlayPreview();

    /// <summary>
    /// プレビュー停止
    /// </summary>
    void StopPreview();

    /// <summary>
    /// プレビューリセット
    /// </summary>
    void ResetPreview();

    /// ===ImGui描画関数群=== ///

    /// <summary>
    /// メニューバーの描画
    /// </summary>
    void RenderMenuBar();

    /// <summary>
    /// 攻撃リストの描画
    /// </summary>
    void RenderAttackList();

    /// <summary>
    /// 基本設定UIの描画
    /// </summary>
    void RenderBasicSettings(AttackData& data);

    /// <summary>
    /// タイミング設定UIの描画
    /// </summary>
    void RenderTimingSettings(AttackData& data);

    /// <summary>
    /// 軌道設定UIの描画
    /// </summary>
    void RenderTrajectorySettings(AttackData& data);

    /// <summary>
    /// エフェクト設定UIの描画
    /// </summary>
    void RenderEffectSettings(AttackData& data);

    /// <summary>
    /// コンボ設定UIの描画
    /// </summary>
    void RenderComboSettings(AttackData& data);

    /// <summary>
    /// プレビューコントロールの描画
    /// </summary>
    void RenderPreviewControl();

    /// ===ヘルパー関数=== ///

    /// <summary>
    /// 選択中の攻撃を削除
    /// </summary>
    void DeleteSelectedAttack();

    /// <summary>
    /// 選択中の攻撃を複製
    /// </summary>
    void DuplicateSelectedAttack();

    /// <summary>
    /// 利用可能なプリセットファイルを更新
    /// </summary>
    void UpdateAvailablePresets();

    /// <summary>
    /// 軌道のプレビュー更新
    /// </summary>
    void UpdateTrajectoryPreview(const float deltaTime);
};