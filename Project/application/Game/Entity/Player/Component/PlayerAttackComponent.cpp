#include "PlayerAttackComponent.h"
// PlayerWeapon
#include "application/Game/Entity/Player/Weapon/PlayerWeapon.h"
#include "application/Game/Entity/Player/Editor/AttackDataSerializer.h"
// ImGui
#ifdef USE_IMGUI
#include <imgui.h>
#endif // USE_IMGUI


///-------------------------------------------/// 
/// 初期化処理
///-------------------------------------------///
void PlayerAttackComponent::Initialize() {

}

///-------------------------------------------/// 
/// 更新処理
///-------------------------------------------///
void PlayerAttackComponent::Update() {
    if (!state_.isActive) return;

    // 攻撃時間が終了したかチェック
    const AttackData* currentData = GetCurrentAttackData();
    if (currentData && state_.timer >= currentData->activeDuration) {
        state_.isActive = false;
        state_.currentAttackID = -1;

        // コンボ受付開始
        if (currentData->canComboToNext) {
            state_.canCombo = true;
            state_.comboTimer = 0.0f;
        }
    }

    // コンボ受付時間の管理
    if (state_.canCombo) {
        if (currentData && state_.comboTimer >= currentData->comboWindowTime) {
            state_.canCombo = false;
            state_.comboCount = 0;  // コンボリセット
        }
    }
}

///-------------------------------------------/// 
/// ImGui情報の表示
///-------------------------------------------///
void PlayerAttackComponent::Information() {
#ifdef USE_IMGUI
    ImGui::SeparatorText("PlayerAttackComponent");

    ImGui::Text("攻撃中:  %s", state_.isActive ? "true" : "false");
    ImGui::Text("現在の攻撃ID:  %d", state_.currentAttackID);
    ImGui::Text("コンボ可能:  %s", state_.canCombo ? "true" : "false");
    ImGui::Text("コンボ数: %d", state_.comboCount);
    ImGui::Text("攻撃タイマー: %.2f", state_.timer);
    ImGui::Text("コンボタイマー: %.2f", state_.comboTimer);
    ImGui::Text("攻撃進行度: %.2f%%", GetAttackProgress() * 100.0f);

    ImGui::Separator();
    ImGui::Text("登録されている攻撃数: %zu", attackDataMap_.size());
#endif
}

///-------------------------------------------/// 
/// 攻撃データをJsonから読み込み
///-------------------------------------------///
bool PlayerAttackComponent::LoadAttackData(int attackID, const std::string& filepath) {
    AttackData data;
    if (AttackDataSerializer::LoadFromJson(data, filepath)) {
        data.attackID = attackID;  // IDを確実に設定
        attackDataMap_[attackID] = data;
        return true;
    }
    return false;
}

///-------------------------------------------/// 
/// 攻撃を開始
///-------------------------------------------///
bool PlayerAttackComponent::StartAttack(int attackID, PlayerWeapon* weapon) {
    // 既に攻撃中の場合は失敗
    if (state_.isActive) {
        return false;
    }

    // 攻撃データを取得
    auto it = attackDataMap_.find(attackID);
    if (it == attackDataMap_.end()) {
        return false;
    }

    const AttackData& attackData = it->second;

    // 武器に攻撃を適用
    ApplyAttackToWeapon(attackData, weapon);

    // 状態を更新
    state_.isActive = true;
    state_.currentAttackID = attackID;
    state_.timer = 0.0f;
    state_.canCombo = false;
    state_.comboCount = 0;

    return true;
}

///-------------------------------------------/// 
/// コンボ攻撃を試行
///-------------------------------------------///
bool PlayerAttackComponent::TryCombo(PlayerWeapon* weapon) {
    // コンボ可能状態でない場合は失敗
    if (!state_.canCombo) {
        return false;
    }

    // 現在の攻撃データを取得
    const AttackData* currentData = GetCurrentAttackData();
    if (!currentData || !currentData->canComboToNext) {
        return false;
    }

    // 次のコンボIDを取得
    int nextAttackID = currentData->nextComboID;
    if (nextAttackID < 0) {
        return false;
    }

    // 次の攻撃を開始
    state_.canCombo = false;
    state_.comboCount++;

    return StartAttack(nextAttackID, weapon);
}

///-------------------------------------------/// 
/// 攻撃をキャンセル
///-------------------------------------------///
void PlayerAttackComponent::CancelAttack() {
    state_.isActive = false;
    state_.canCombo = false;
    state_.currentAttackID = -1;
    state_.comboCount = 0;
    state_.timer = 0.0f;
    state_.comboTimer = 0.0f;
}

///-------------------------------------------/// 
/// 現在の攻撃データを取得
///-------------------------------------------///
const AttackData* PlayerAttackComponent::GetCurrentAttackData() const {
    return GetAttackData(state_.currentAttackID);
}

///-------------------------------------------/// 
/// 攻撃データをIDで取得
///-------------------------------------------///
const AttackData* PlayerAttackComponent::GetAttackData(int attackID) const {
    auto it = attackDataMap_.find(attackID);
    if (it != attackDataMap_.end()) {
        return &it->second;
    }
    return nullptr;
}

///-------------------------------------------/// 
/// 現在の攻撃進行度を取得
///-------------------------------------------///
float PlayerAttackComponent::GetAttackProgress() const {
    const AttackData* data = GetCurrentAttackData();
    if (!data || !state_.isActive) {
        return 0.0f;
    }

    return (std::min)(state_.timer / data->activeDuration, 1.0f);
}

///-------------------------------------------/// 
/// 武器に攻撃軌道を設定
///-------------------------------------------///
void PlayerAttackComponent::ApplyAttackToWeapon(const AttackData& data, PlayerWeapon* weapon) {
    if (!weapon || data.trajectoryPoints.size() < 2) {
        return;
    }

    // ベジェ曲線の開始点と終了点を取得
    Vector3 localStart = data.trajectoryPoints.front().position;
    Vector3 localEnd = data.trajectoryPoints.back().position;

    //// プレイヤーの回転を適用してワールド座標に変換
    //Vector3 worldStart = basePosition + localStart;
    //Vector3 worldEnd = basePosition + localEnd;

    // 武器に攻撃を開始させる
    weapon->StartAttack(
        localStart,
        localEnd,
        data.activeDuration,
        data.startRotation,
        data.endRotation
    );
}

///-------------------------------------------/// 
/// タイマーの更新
///-------------------------------------------///
void PlayerAttackComponent::UpdateTimers(float deltaTime) {
    if (state_.isActive) {
        state_.timer += deltaTime;
    }

    if (state_.canCombo) {
        state_.comboTimer += deltaTime;
    }
}