#pragma once
/// ===Include=== ///
#include "Engine/Graphics/Particle/Base/ParticleGroup.h"
// Math
#include "Engine/DataInfo/ColliderData.h"
// c++
#include <random>

///=====================================================/// 
/// 爆破エミッタ
///=====================================================///
class ExplosionParticle : public ParticleGroup {
public:

    ExplosionParticle(); // コンストラクタでの処理は行わない
    ~ExplosionParticle();

    /// <summary>
    /// 初期化処理
    /// </summary>
    /// <param name="translate">初期化に使用する平行移動を表す Vector3 型の const 参照。読み取り専用の 3D ベクトルを渡す。</param>
    void Initialze(const Vector3& translate)override;

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

private:
    // === コピー禁止 === ///
    ExplosionParticle(const ExplosionParticle&) = delete;
    ExplosionParticle& operator=(const ExplosionParticle&) = delete;

    // 爆発の設定
    float explosionRadius_; // 爆発の半径
    float maxLifetime_; // パーティクルの最大寿命
    bool hasExploded_; // パーティクルが発生したかのフラグ

private:
    /// <summary>
    /// ランダムなパーティクルの生成処理
    /// </summary>
    /// <param name="randomEngine">粒子のランダムな特性（例：速度、寿命、色のばらつき）を生成するための std::mt19937 の参照。呼び出しにより内部状態が更新される可能性がある。</param>
    /// <param name="translate">生成される粒子に適用する並進（位置）ベクトル。変更されないことを示す const 参照で渡される。</param>
    /// <returns>生成された ParticleData オブジェクト。新しい粒子の位置や速度、その他のプロパティを含む。</returns>
    ParticleData MakeParticle(std::mt19937& randomEngine, const Vector3& translate)override;

    /// <summary>
    /// パーティクルの発生処理
    /// </summary>
    /// <param name="group">パーティクル生成の対象となるグループへの定数参照。関数内では変更されません。</param>
    /// <param name="randomEngine">パーティクルのランダム性を生成するための std::mt19937 への参照。関数内で状態が更新される可能性があります。</param>
    /// <returns>生成された各パーティクルの情報を格納した std::list<ParticleData>。</returns>
    std::list<ParticleData> Emit(const Group& group, std::mt19937& randomEngine)override;
};