#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Graphics/Particle/Base/ParticleGroup.h"
// Math
#include "Engine/DataInfo/ColliderData.h"
// c++
#include <random>
#include <list>

///=====================================================/// 
/// 攻撃軌跡パーティクル
///=====================================================///
class AttackTrajectoryParticle : public ParticleGroup {
public:

    AttackTrajectoryParticle();
    ~AttackTrajectoryParticle();

    /// <summary>
    /// 初期化処理
    /// </summary>
    /// <param name="translate">初期化に使用する平行移動を表す Vector3 型の const 参照。読み取り専用の 3D ベクトルを渡す。</param>
    void Initialze(const Vector3& translate) override;

    /// <summary>
    /// 更新処理
    /// </summary>
    void Update() override;

    /// <summary>
    /// 描画処理
    /// </summary>
    /// <param name="mode">描画に使用するブレンドモード。省略した場合は BlendMode::KBlendModeNormal が使用されます。</param>
    void Draw(BlendMode mode = BlendMode::KBlendModeNormal) override;

    /// <summary>
    /// クローン処理
    /// </summary>
    /// <returns>複製された ParticleGroup の所有権を持つ std::unique_ptr。複製に失敗した場合は nullptr を返すことがある（実装依存）。</returns>
    std::unique_ptr<ParticleGroup> Clone() override;

    /// <summary>
    /// 軌跡の位置と回転を設定（外部から呼び出す）
    /// </summary>
    /// <param name="position">現在の武器の位置</param>
    /// <param name="rotation">現在の武器の回転（クォータニオン）</param>
    void SetTrajectoryTransform(const Vector3& position, const Quaternion& rotation);

    /// <summary>
    /// 軌跡の位置を設定（後方互換性のため）
    /// </summary>
    /// <param name="position">現在の武器の位置</param>
    void SetTrajectoryPosition(const Vector3& position);

    /// <summary>
    /// 軌跡パーティクルの発生を開始
    /// </summary>
    void StartEmission();

    /// <summary>
    /// 軌跡パーティクルの発生を停止
    /// </summary>
    void StopEmission();

	/// <summary>
	/// パーティクルをクリア
	/// </summary>
	void ClearParticle();

private:
    // === コピー禁止 === ///
    AttackTrajectoryParticle(const AttackTrajectoryParticle&) = delete;
    AttackTrajectoryParticle& operator=(const AttackTrajectoryParticle&) = delete;

    // 軌跡設定
    bool isEmitting_;           // パーティクル発生中フラグ
    float emitTimer_;           // パーティクル発生タイマー
    float emitInterval_;        // パーティクル発生間隔
    Vector3 currentPosition_;   // 現在の位置
    Vector3 previousPosition_;  // 前回の位置
    Quaternion currentRotation_;    // 現在の回転
    Quaternion previousRotation_;   // 前回の回転

    // パーティクルパラメータ
    float particleSpread_;      // パーティクルの広がり
    float fadeSpeed_;           // フェードアウト速度
    float initialScale_;        // 初期スケール

    // 色設定
    Vector4 trajectoryColor_;   // 主要な軌跡の色
    Vector4 secondaryColor_;    // セカンダリカラー（アクセント用）

    // 動きのパラメータ
    float swirlingSpeed_;       // 渦巻きの速度
    float expansionRate_;       // 拡散速度
    float rotationInfluence_;   // 回転速度の影響係数

    // パーティクルごとの追加データ（ParticleDataを拡張できない場合）
    std::list<Quaternion> particleRotations_;       // 各パーティクルの初期回転
    std::list<Vector3> particleInitialVelocities_;  // 各パーティクルの初速度

private:

    /// <summary>
    /// ランダムなパーティクルの生成処理
    /// </summary>
    /// <param name="randomEngine">粒子のランダムな特性（例：速度、寿命、色のばらつき）を生成するための std::mt19937 の参照。呼び出しにより内部状態が更新される可能性がある。</param>
    /// <param name="translate">生成される粒子に適用する並進（位置）ベクトル。変更されないことを示す const 参照で渡される。</param>
    /// <returns>生成された ParticleData オブジェクト。新しい粒子の位置や速度、その他のプロパティを含む。</returns>
    ParticleData MakeParticle(std::mt19937& randomEngine, const Vector3& translate) override;

    /// <summary>
    /// パーティクルの発生処理
    /// </summary>
    /// <param name="group">パーティクル生成の対象となるグループへの定数参照。関数内では変更されません。</param>
    /// <param name="randomEngine">パーティクルのランダム性を生成するための std::mt19937 への参照。関数内で状態が更新される可能性があります。</param>
    /// <returns>生成された各パーティクルの情報を格納した std::list<ParticleData>。</returns>
    std::list<ParticleData> Emit(const Group& group, std::mt19937& randomEngine) override;
};