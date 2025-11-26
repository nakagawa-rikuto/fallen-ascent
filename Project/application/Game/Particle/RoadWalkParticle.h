#pragma once
/// ===Include=== ///
// Engine
#include "Engine/Graphics/Particle/Base/ParticleGroup.h"
// C++
#include <random>

///=====================================================/// 
/// 道を歩く時のパーティクル
///=====================================================///
class RoadWalkParticle : public ParticleGroup {
public:

	RoadWalkParticle();
	~RoadWalkParticle();

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

private: 
    // === コピー禁止 === ///
    RoadWalkParticle(const RoadWalkParticle&) = delete;
    RoadWalkParticle& operator=(const RoadWalkParticle&) = delete;

    // 
};

