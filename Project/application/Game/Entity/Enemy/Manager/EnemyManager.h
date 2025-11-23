#pragma once
/// ===Include=== ///
#include "application/Game/Entity/Enemy/Base/BaseEnemy.h"
#include <memory>
#include <vector>

// 前方宣言
class Player;

/// ===enum=== ///
enum class EnemyType {
    CloseRange,
    LongRange
};

///=====================================================/// 
/// EnemyManager
///=====================================================///
class EnemyManager {
public:
    EnemyManager() = default;
    ~EnemyManager() = default;

    /// <summary>
    /// プレイヤーを設定
    /// </summary>
    /// <param name="player">設定する Player オブジェクトへのポインタ。</param>
    void SetPlayer(Player* player);

    /// <summary>
    /// 指定したタイプ・位置・回転で敵を生成します。
    /// </summary>
    /// <param name="type">生成する敵の種類。EnemyType 型。</param>
    /// <param name="pos">生成位置。Vector3 の const リファレンス。</param>
    /// <param name="rot">生成時の回転。Quaternion の const リファレンス。</param>
    /// <returns>生成された敵インスタンスへのポインタ (BaseEnemy*)。</returns>
    BaseEnemy* Spawn(EnemyType type, const Vector3& pos, const Quaternion& rot, const Vector3& halfSize);

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update();

	/// <summary>
	/// アニメーション時の更新処理
	/// </summary>
	void UpdateAnimation();

    /// <summary>
    /// 描画処理
    /// </summary>
    /// <param name="mode">描画に使用するブレンドモード。省略した場合は BlendMode::KBlendModeNormal が使用されます。</param>
    void Draw(BlendMode mode = BlendMode::KBlendModeNormal);

    /// <summary>
	/// ImGui情報の表示
    /// </summary>
    void UpdateImGui();

    /// <summary>
    /// オブジェクトの内部状態や保持しているデータをクリア
    /// </summary>
    void Clear();

    /// <summary>
    /// 指定した敵の種類にソース敵の設定を適用
    /// </summary>
    /// <param name="type">設定を適用する対象の敵の種類を表す値。</param>
    /// <param name="sourceEnemy">設定を取得する元となる敵を指す const ポインタ（このオブジェクトから設定がコピーされます）。</param>
    void ApplySettingsToType(EnemyType type, const BaseEnemy* sourceEnemy);

    /// <summary>
    /// 現存する全ての敵にソース敵の設定を適用
    /// </summary>
    /// <param name="sourceEnemy">設定を取得する元となる敵を指す const ポインタ</param>
    void ApplySettingsToAll(const BaseEnemy* sourceEnemy);

    /// <summary>
    /// 指定した EnemyType に代表的な設定を適用
    /// </summary>
    /// <param name="type">適用対象の敵の種類。EnemyType（列挙型など）で指定します。</param>
    void ApplyRepresentativeSettingsToType(EnemyType type);

    /// <summary>
    /// 指定した敵タイプの代表的な設定を、すべての対象に適用
    /// </summary>
    /// <param name="sourceType">代表設定のソースとなる敵の種類。ここで指定した敵タイプの設定が他のすべての対象に適用されます。</param>
    void ApplyRepresentativeSettingsToAll(EnemyType sourceType);

public: /// ===Getter=== ///
    //指定タイプの代表敵を取得
    BaseEnemy* GetRepresentative(EnemyType type);

    // 指定タイプの敵数を取得
    int GetEnemyCount(EnemyType type) const;

    // 全敵数を取得
    int GetTotalEnemyCount() const;

private:
    // Player
    Player* player_ = nullptr;

	// BaseEnemyの配列
    std::vector<std::unique_ptr<BaseEnemy>> enemies_;

    // タイプ判定
    EnemyType GetEnemyType(BaseEnemy* enemy) const;

public:
    // === テンプレート関数の実装 === //

    /// 指定タイプの代表敵を取得して調整用の関数を実行後、同タイプ全体に適用
    template<typename Func>
    void ModifyAndApplyToType(EnemyType type, Func adjustFunction) {
        BaseEnemy* representative = GetRepresentative(type);
        if (representative) {
            // 調整関数を実行
            adjustFunction(representative);
            // 同タイプ全体に適用
            ApplySettingsToType(type, representative);
        }
    }

    /// 指定タイプの代表敵を取得して調整用の関数を実行後、全敵に適用
    template<typename Func>
    void ModifyAndApplyToAll(EnemyType sourceType, Func adjustFunction) {
        BaseEnemy* representative = GetRepresentative(sourceType);
        if (representative) {
            // 調整関数を実行
            adjustFunction(representative);
            // 全敵に適用
            ApplySettingsToAll(representative);
        }
    }
};