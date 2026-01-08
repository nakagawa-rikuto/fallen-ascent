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

public: /// ===Getter=== ///
    
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

#ifdef USE_IMGUI
private:

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
    /// 指定した敵タイプに対応する代表的な BaseEnemy インスタンスへのポインタを取得します。
    /// </summary>
    /// <param name="type">取得したい敵の種類を表す EnemyType 値。</param>
    /// <returns>指定したタイプに対応する BaseEnemy へのポインタ。該当する敵が存在しない場合は nullptr を返すことがあります。</returns>
    BaseEnemy* GetRepresentative(EnemyType type);

public:
    // === テンプレート関数の実装 === //
    /// <summary>
    /// 指定した敵タイプの代表的な敵オブジェクトに対して調整関数を実行し、その変更を同タイプ全体に反映します。
    /// </summary>
    /// <typeparam name="Func">代表インスタンス（BaseEnemy*）を受け取って設定を変更する呼び出し可能型。戻り値は通常不要（void でも可）。</typeparam>
    /// <param name="type">操作対象の敵タイプ。GetRepresentativeを使って代表インスタンスが取得されます。</param>
    /// <param name="adjustFunction">代表インスタンスを受け取って設定を変更する呼び出し可能オブジェクト（例: ラムダや関数オブジェクト）。</param>
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

    /// <summary>
    /// 指定した敵タイプの代表個体に対して調整関数を実行し、その設定を全ての敵に適用する。代表が見つからない場合は何もしない。
    /// </summary>
    /// <typeparam name="Func">代表個体を引数に取る呼び出し可能な型（例: void(BaseEnemy*)）。代表に対する変更・調整を行うための型。</typeparam>
    /// <param name="sourceType">設定の基準となる敵タイプ。代表個体がこのタイプから取得される。</param>
    /// <param name="adjustFunction">代表個体（BaseEnemy*）を受け取り、その設定を変更する呼び出し可能オブジェクト（関数、関数オブジェクト、ラムダなど）。戻り値は無視される。</param>
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
#endif // USE_IMGUI
};