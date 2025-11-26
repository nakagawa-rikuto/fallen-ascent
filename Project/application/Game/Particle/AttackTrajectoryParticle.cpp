#include "AttackTrajectoryParticle.h"
// Service
#include "Engine/System/Service/DeltaTimeSevice.h"
// Math
#include "Math/sMath.h"
// c++
#include <numbers>

///-------------------------------------------/// 
/// コンストラクタ・デストラクタ
///-------------------------------------------///
AttackTrajectoryParticle::AttackTrajectoryParticle() {}
AttackTrajectoryParticle::~AttackTrajectoryParticle() {
    group_.particles.clear();
    group_.particle.reset();
    particleRotations_.clear();
    particleInitialVelocities_.clear();
}

///-------------------------------------------/// 
/// 初期化
///-------------------------------------------///
void AttackTrajectoryParticle::Initialze(const Vector3& translate) {

    /// ===初期化=== ///
    ParticleGroup::InstancingInit("plane", translate, 200);

    /// ===フラグと設定の初期化=== ///
    isEmitting_ = false;
    emitTimer_ = 0.0f;
    emitInterval_ = 0.008f;
    currentPosition_ = translate;
    previousPosition_ = translate;
    currentRotation_ = { 0.0f, 0.0f, 0.0f, 1.0f };
    previousRotation_ = { 0.0f, 0.0f, 0.0f, 1.0f };

    // パーティクル設定
    particleSpread_ = 0.25f;
    fadeSpeed_ = 3.5f;
    initialScale_ = 0.3f;

    // 軌跡の色（鮮やかなエネルギー感）
    trajectoryColor_ = { 0.2f, 0.8f, 1.0f, 1.0f }; // シアン系
    secondaryColor_ = { 1.0f, 0.9f, 0.3f, 1.0f };  // 黄色系（アクセント）

    // 動きの設定
    swirlingSpeed_ = 3.0f;
    expansionRate_ = 0.8f;

    // 回転速度の影響係数
    rotationInfluence_ = 1.2f;
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void AttackTrajectoryParticle::Update() {
    // 武器の回転速度を計算（前フレームからの回転変化）
    Quaternion rotationDelta = Multiply(currentRotation_, Math::Conjugate(previousRotation_));

    // パーティクル発生処理
    if (isEmitting_) {
        emitTimer_ += kDeltaTime_;

        // 一定間隔でパーティクルを発生
        if (emitTimer_ >= emitInterval_) {
            group_.particles.splice(group_.particles.end(), Emit(group_, randomEngine_));
            emitTimer_ = 0.0f;
        }
    }

    // パーティクルの更新
    group_.numInstance = 0;
    auto itRotation = particleRotations_.begin();
    auto itVelocity = particleInitialVelocities_.begin();

    for (auto it = group_.particles.begin(); it != group_.particles.end();) {
        if (it->currentTime >= it->lifeTime) {
            it = group_.particles.erase(it);
            if (itRotation != particleRotations_.end()) {
                itRotation = particleRotations_.erase(itRotation);
            }
            if (itVelocity != particleInitialVelocities_.end()) {
                itVelocity = particleInitialVelocities_.erase(itVelocity);
            }
            continue;
        }

        // 時間の更新
        it->currentTime += kDeltaTime_;
        float progress = it->currentTime / it->lifeTime;

        // スムーズなフェードアウト（イージング適用）
        float alpha = 1.0f - (progress * progress);
        it->color.w = alpha;

        // スケールアニメーション：最初膨らんでから縮む
        float scaleProgress = progress * 2.0f;
        float scale;
        if (scaleProgress < 1.0f) {
            scale = 1.0f + (scaleProgress * 0.5f);
        } else {
            scale = 1.5f - ((scaleProgress - 1.0f) * 1.5f);
        }
        it->transform.scale = {
            initialScale_ * scale,
            initialScale_ * scale,
            initialScale_ * scale
        };

        // 渦巻き運動（武器の回転を考慮）
        float swirl = it->currentTime * swirlingSpeed_;

        // 武器の向きに基づいたローカル空間での渦巻きオフセット
        Vector3 localSwirlingOffset = {
            std::cos(swirl) * expansionRate_ * progress,
            std::sin(swirl * 1.3f) * expansionRate_ * progress * 0.5f,
            std::sin(swirl) * expansionRate_ * progress
        };

        // 武器の回転を適用して、ワールド空間に変換
        Quaternion initialRot = (itRotation != particleRotations_.end()) ? *itRotation : currentRotation_;
        Vector3 worldSwirlingOffset = Math::RotateVector(localSwirlingOffset, initialRot);

        // 回転による遠心力的な速度を追加
        Vector3 initialVel = (itVelocity != particleInitialVelocities_.end()) ? *itVelocity : it->velocity;
        Vector3 rotationalVelocity = Math::RotateVector(initialVel * rotationInfluence_, rotationDelta);

        // 位置の更新：元の位置 + 速度 + 渦巻き + 回転による遠心力
        it->transform.translate = it->transform.translate + (it->velocity * kDeltaTime_) + (worldSwirlingOffset * kDeltaTime_) + (rotationalVelocity * kDeltaTime_ * 0.3f);

        // 速度の減衰
        it->velocity = it->velocity * 0.95f;

        // 回転アニメーション（武器の回転も反映）
        Quaternion particleRotation = Math::SLerp(
            initialRot,
            Multiply(initialRot, rotationDelta),
            progress * 0.5f
        );

        // ビルボード効果用のZ軸回転を追加
        float zRotation = (3.0f + swirl * 0.5f) * it->currentTime;
        Quaternion zRotationQuat = Math::MakeRotateAxisAngle({ 0.0f, 0.0f, 1.0f }, zRotation);
        it->transform.rotate = Math::QuaternionToEuler(Multiply(particleRotation, zRotationQuat));

        // 色の変化（2色の間でブレンド）
        float colorBlend = std::sin(progress * std::numbers::pi_v<float>) * 0.5f + 0.5f;
        it->color.x = Math::Lerp(trajectoryColor_.x, secondaryColor_.x, colorBlend);
        it->color.y = Math::Lerp(trajectoryColor_.y, secondaryColor_.y, colorBlend);
        it->color.z = Math::Lerp(trajectoryColor_.z, secondaryColor_.z, colorBlend);

        /// ===ParticleEmitterの更新=== ///
        ParticleGroup::InstancingUpdate(it);
        ++it;
        if (itRotation != particleRotations_.end()) ++itRotation;
        if (itVelocity != particleInitialVelocities_.end()) ++itVelocity;
    }

    // 前回の位置と回転を更新
    previousPosition_ = currentPosition_;
    previousRotation_ = currentRotation_;
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void AttackTrajectoryParticle::Draw(BlendMode mode) {
    if (group_.numInstance > 0) {
        ParticleGroup::Draw(mode);
    }
}

///-------------------------------------------/// 
/// クローン
///-------------------------------------------///
std::unique_ptr<ParticleGroup> AttackTrajectoryParticle::Clone() {
    std::unique_ptr<AttackTrajectoryParticle> clone = std::make_unique<AttackTrajectoryParticle>();

    clone->emitInterval_ = this->emitInterval_;
    clone->particleSpread_ = this->particleSpread_;
    clone->fadeSpeed_ = this->fadeSpeed_;
    clone->trajectoryColor_ = this->trajectoryColor_;
    clone->secondaryColor_ = this->secondaryColor_;
    clone->initialScale_ = this->initialScale_;
    clone->swirlingSpeed_ = this->swirlingSpeed_;
    clone->expansionRate_ = this->expansionRate_;
    clone->rotationInfluence_ = this->rotationInfluence_;

    return clone;
}

///-------------------------------------------/// 
/// 軌跡の位置と回転を設定
///-------------------------------------------///
void AttackTrajectoryParticle::SetTrajectoryTransform(const Vector3& position, const Quaternion& rotation) {
    currentPosition_ = position;
    currentRotation_ = rotation;
    group_.transform.translate = position;
}

///-------------------------------------------/// 
/// 軌跡の位置を設定（後方互換性のため残す）
///-------------------------------------------///
void AttackTrajectoryParticle::SetTrajectoryPosition(const Vector3& position) {
    currentPosition_ = position;
    group_.transform.translate = position;
}

///-------------------------------------------/// 
/// 軌跡パーティクルの発生を開始
///-------------------------------------------///
void AttackTrajectoryParticle::StartEmission() {
    isEmitting_ = true;
    emitTimer_ = 0.0f;
}

///-------------------------------------------/// 
/// 軌跡パーティクルの発生を停止
///-------------------------------------------///
void AttackTrajectoryParticle::StopEmission() {
    isEmitting_ = false;
	// パーティクルをクリア
	ClearParticle();
}

///-------------------------------------------/// 
/// パーティクルをクリア
///-------------------------------------------///
void AttackTrajectoryParticle::ClearParticle() {
    // パーティクルリストをクリア
    group_.particles.clear();

    // 追加データもクリア
    particleRotations_.clear();
    particleInitialVelocities_.clear();

    // インスタンス数をリセット
    group_.numInstance = 0;

    // タイマーもリセット
    emitTimer_ = 0.0f;

    // 位置と回転の履歴をリセット
    previousPosition_ = currentPosition_;
    previousRotation_ = currentRotation_;
}

///-------------------------------------------/// 
/// 軌跡パーティクルの生成
///-------------------------------------------///
ParticleData AttackTrajectoryParticle::MakeParticle(std::mt19937& randomEngine, const Vector3& center) {
    std::uniform_real_distribution<float> distSpread(-particleSpread_, particleSpread_);
    std::uniform_real_distribution<float> distLifetime(0.4f, 0.8f);
    std::uniform_real_distribution<float> distRotation(0.0f, 2.0f * std::numbers::pi_v<float>);
    std::uniform_real_distribution<float> distVelocity(-0.3f, 0.3f);

    ParticleData particle;
    particle.transform.scale = { initialScale_, initialScale_, initialScale_ };

    // 武器の回転を考慮した初期回転を別リストに保存
    particle.transform.rotate = Math::QuaternionToEuler(currentRotation_);

    // ランダムなオフセット（球状に分布）を武器のローカル空間で生成
    float theta = distRotation(randomEngine);
    float phi = distRotation(randomEngine);
    float radius = std::abs(distSpread(randomEngine));

    Vector3 localOffset = {
        radius * std::sin(phi) * std::cos(theta),
        radius * std::sin(phi) * std::sin(theta),
        radius * std::cos(phi)
    };

    // 武器の回転を適用してワールド空間に変換
    Vector3 worldOffset = Math::RotateVector(localOffset, currentRotation_);

    particle.transform.translate = center + worldOffset;

    // ランダムな初速度（武器の向きを考慮）
    Vector3 localDirection = Normalize(localOffset);
    float speed = std::abs(distVelocity(randomEngine));
    Vector3 localVelocity = localDirection * speed;

    // 武器の回転を適用して初速度を設定
    particle.velocity = Math::RotateVector(localVelocity, currentRotation_);

    // 色の設定（ランダムで2色のどちらかに近く）
    std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
    float colorChoice = distColor(randomEngine);
    if (colorChoice < 0.7f) {
        particle.color = trajectoryColor_;
    } else {
        particle.color = secondaryColor_;
    }

    // 寿命を設定
    particle.lifeTime = distLifetime(randomEngine);
    particle.currentTime = 0.0f;

    return particle;
}

///-------------------------------------------/// 
/// パーティクルの発生処理
///-------------------------------------------///
std::list<ParticleData> AttackTrajectoryParticle::Emit(const Group& group, std::mt19937& randomEngine) {
    std::list<ParticleData> particles;

    // 一度に5-7個のパーティクルを発生（密度を上げる）
    std::uniform_int_distribution<int> distCount(5, 7);
    int numParticles = distCount(randomEngine);

    for (int i = 0; i < numParticles; ++i) {
        ParticleData particle = MakeParticle(randomEngine, group.transform.translate);

        // 初期回転と初速度を別リストに保存
        particleRotations_.push_back(currentRotation_);
        particleInitialVelocities_.push_back(particle.velocity);

        particles.push_back(particle);
    }

    return particles;
}