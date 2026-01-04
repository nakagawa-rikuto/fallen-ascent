#pragma once
/// ===Include=== ///
// Math
#include "Math/Vector3.h"
#include "Math/Quaternion.h"
#include "Engine/DataInfo/LineObjectData.h"
// C++
#include <vector>
#include <string>

///-------------------------------------------/// 
/// 攻撃データ構造体
///-------------------------------------------///
struct AttackData {
    // ===基本情報=== //
    std::string attackName;              // 攻撃名
    std::string description;             // 説明
    int attackID;                        // 攻撃ID（ユニーク）

    // ===タイミング設定=== //
    float activeDuration;                // 攻撃のアクティブ時間（秒）
    float comboWindowTime;               // コンボ受付時間（秒）
    float cooldownTime;                  // クールダウン時間（秒）

    // ===軌道設定（ベジェ曲線）=== //
    std::vector<BezierControlPointData> trajectoryPoints;  // ベジェ曲線の制御点リスト
    int curveSegments;                   // 曲線の分割数（滑らかさ）
    float weaponLength;                  // 武器の距離

    // ===回転設定=== //
    Quaternion startRotation;            // 開始回転
    Quaternion endRotation;              // 終了回転
    bool useRotationCurve;               // 回転もベジェ曲線で補間するか

    // ===コンボ設定=== //
    bool canComboToNext;                 // 次のコンボに繋げるか
    int nextComboID;                     // 次のコンボのID（-1で無し）
    std::vector<int> branchComboIDs;     // 分岐可能なコンボID（複数選択肢）

    // ===エフェクト設定=== //
    std::string particleEffectName;      // パーティクルエフェクト名
    Vector3 particleOffset;              // パーティクルのオフセット
    float cameraShakeIntensity;          // カメラシェイクの強度
    float cameraShakeDuration;           // カメラシェイクの持続時間

    // ===サウンド設定=== //
    std::string swingSoundName;          // 振りサウンド名
    std::string hitSoundName;            // ヒット時サウンド名

    // ===プレイヤーの動き=== //
    float moveSpeedMultiplier;           // 攻撃中の移動速度倍率（0.0～1.0）
    Vector3 rootMotion;                  // ルートモーション（前進など）

    // ===デバッグ/プレビュー用=== //
    bool showTrajectory;                 // 軌道を表示するか
    Vector3 trajectoryColor;             // 軌道の色（RGB）

    /// デフォルトコンストラクタ
    AttackData()
        : attackName("NewAttack")
        , description("")
        , attackID(0)
        , activeDuration(0.4f)
        , comboWindowTime(1.0f)
        , cooldownTime(0.3f)
        , curveSegments(20)
        , weaponLength(8.0f)
        , startRotation(Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f })
        , endRotation(Quaternion{ 0.0f, 0.0f, 0.0f, 1.0f })
        , useRotationCurve(false)
        , canComboToNext(false)
        , nextComboID(-1)
        , particleEffectName("WeaponAttack")
        , particleOffset(Vector3{ 0.0f, 0.0f, 0.0f })
        , cameraShakeIntensity(0.0f)
        , cameraShakeDuration(0.0f)
        , swingSoundName("")
        , hitSoundName("")
        , moveSpeedMultiplier(0.4f)
        , rootMotion(Vector3{ 0.0f, 0.0f, 0.0f })
        , showTrajectory(true)
        , trajectoryColor(Vector3{ 1.0f, 0.0f, 0.0f })
    {
        // デフォルトで3点のベジェ曲線を設定（左→中央→右）
        trajectoryPoints.push_back({ Vector3{-8.0f, 0.0f, 0.0f}, 0.0f });
        trajectoryPoints.push_back({ Vector3{0.0f, 0.0f, 8.0f}, 0.5f });
        trajectoryPoints.push_back({ Vector3{8.0f, 0.0f, 0.0f}, 1.0f });
    }
};