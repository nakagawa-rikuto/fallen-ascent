#include "DustCloudParticle.h"
// Service
#include "Engine/System/Service/DeltaTimeSevice.h"
// Math
#include "Math/sMath.h"
// c++
#include <numbers>


///-------------------------------------------/// 
/// コンストラクタ・デストラクタ
///-------------------------------------------///
DustCloudParticle::DustCloudParticle() {}
DustCloudParticle::~DustCloudParticle() {
    group_.particles.clear();
    group_.particle.reset();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void DustCloudParticle::Initialze(const Vector3& translate) {
    /// ===初期化=== ///
    ParticleGroup::InstancingInit("plane", translate, 50);

    /// ===フラグと設定の初期化=== ///
    hasEmitted_ = false;
    spreadRadius_ = 0.8f;    // 地面での広がり範囲
    upwardSpeed_ = 3.0f;     // 上昇速度
    gravity_ = -3.0f;        // 重力
    maxLifetime_ = 1.2f;     // パーティクルの寿命
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void DustCloudParticle::Update() {
    if (!hasEmitted_) {
        // 土煙のパーティクルを生成
        group_.particles.splice(group_.particles.end(), Emit(group_, randomEngine_));
        hasEmitted_ = true;
    }

    // パーティクルの更新
    group_.numInstance = 0; // インスタンス数をリセット
    for (auto it = group_.particles.begin(); it != group_.particles.end();) {
        if (it->currentTime >= it->lifeTime) {
            it = group_.particles.erase(it); // 寿命が尽きたパーティクルを削除
            continue;
        }

        // 重力を適用
        it->velocity.y += gravity_ * kDeltaTime_;

        // 位置の更新
        it->transform.translate += Vector3{
            it->velocity.x * kDeltaTime_,
            it->velocity.y * kDeltaTime_,
            it->velocity.z * kDeltaTime_
        };

        // 時間比率の計算
        float timeRatio = it->currentTime / it->lifeTime;

        // 横方向の速度減衰
        it->velocity.x *= (1.0f - kDeltaTime_ * 2.0f);
        it->velocity.z *= (1.0f - kDeltaTime_ * 2.0f);

        // スケールの変化(時間経過で大きくなる)
        float scaleGrowth = 1.0f + (timeRatio * 2.5f); // 最大3.5倍
        it->transform.scale = { 0.12f * scaleGrowth, 0.12f * scaleGrowth, 0.12f * scaleGrowth };

        // 透明度の減少(時間経過でフェードアウト)
        it->color.w = 1.0f - timeRatio;

        // ゆっくりとした回転
        it->transform.rotate.y += 1.0f * kDeltaTime_;

        it->currentTime += kDeltaTime_;

        /// ===ParticleEmitterの更新=== ///
        ParticleGroup::InstancingUpdate(it);
        ++it;
    }
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void DustCloudParticle::Draw(BlendMode mode) {
    if (group_.numInstance > 0) {
        ParticleGroup::Draw(mode);
    }
}

///-------------------------------------------/// 
/// クローン
///-------------------------------------------///
std::unique_ptr<ParticleGroup> DustCloudParticle::Clone() {
    // 新しいインスタンスを作成
    std::unique_ptr<DustCloudParticle> clone = std::make_unique<DustCloudParticle>();

    // 状態のコピー
    clone->spreadRadius_ = this->spreadRadius_;
    clone->upwardSpeed_ = this->upwardSpeed_;
    clone->gravity_ = this->gravity_;
    clone->maxLifetime_ = this->maxLifetime_;

    return clone;
}

///-------------------------------------------/// 
/// 土煙パーティクルの生成
///-------------------------------------------///
ParticleData DustCloudParticle::MakeParticle(std::mt19937& randomEngine, const Vector3& center) {
    std::uniform_real_distribution<float> distAngle(0.0f, 2.0f * std::numbers::pi_v<float>);
    std::uniform_real_distribution<float> distRadius(0.0f, spreadRadius_);
    std::uniform_real_distribution<float> distLifetime(0.6f, maxLifetime_);
    std::uniform_real_distribution<float> distUpward(0.8f, 1.2f); // 上昇速度のばらつき

    ParticleData particle;
    particle.transform.scale = { 0.12f, 0.12f, 0.12f }; // 初期スケール
    particle.transform.rotate = { 0.0f, 0.0f, 0.0f };

    // 地面での放射状の広がりを計算
    float angle = distAngle(randomEngine);
    float radius = distRadius(randomEngine);

    Vector3 offset = {
        radius * std::cos(angle),
        0.0f, // 地面から発生
        radius * std::sin(angle)
    };

    particle.transform.translate = center + offset;

    // 速度の設定(主に上方向 + わずかに外側)
    particle.velocity = {
        offset.x * 0.5f, // 外側への速度(小さめ)
        upwardSpeed_ * distUpward(randomEngine), // 上昇速度
        offset.z * 0.5f  // 外側への速度(小さめ)
    };

    // 土煙らしい色(茶色がかった灰色)
    std::uniform_real_distribution<float> distBrightness(0.4f, 0.6f);
    float brightness = distBrightness(randomEngine);
    particle.color = {
        brightness + 0.2f,  // R: 少し明るめ
        brightness + 0.15f, // G: 茶色がかった色
        brightness,         // B: 基準値
        0.7f                // A: 初期アルファ値
    };

    particle.lifeTime = distLifetime(randomEngine);
    particle.currentTime = 0.0f;

    return particle;
}

///-------------------------------------------/// 
/// パーティクルの発生処理
///-------------------------------------------///
std::list<ParticleData> DustCloudParticle::Emit(const Group& group, std::mt19937& randomEngine) {
    std::list<ParticleData> particles;
    for (uint32_t i = 0; i < group.maxInstance; ++i) {
        particles.push_back(MakeParticle(randomEngine, group.transform.translate));
    }
    return particles;
}