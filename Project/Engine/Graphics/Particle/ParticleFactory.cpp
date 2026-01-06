#include "ParticleFactory.h"
// Math
#include "Math/sMath.h"
// c++
#include <numbers>

///-------------------------------------------/// 
/// 単一パーティクル生成
///-------------------------------------------///
ParticleData ParticleFactory::CreateParticle(
    const ParticleDefinition& definition,
    std::mt19937& randomEngine,
    const Vector3& position) {

    ParticleData particle;

    // スケールの設定
    particle.transform.scale = GenerateRandomScale(definition, randomEngine);

    // 初期回転の設定
    particle.transform.rotate = GenerateRandomInitialRotation(definition, randomEngine);

    // 位置の設定（爆発範囲を考慮）
    Vector3 offset = { 0.0f, 0.0f, 0.0f };
    if (definition.physics.explosionRange.x > 0.0f ||
        definition.physics.explosionRange.y > 0.0f ||
        definition.physics.explosionRange.z > 0.0f) {

        std::uniform_real_distribution<float> distX(-definition.physics.explosionRange.x, definition.physics.explosionRange.x);
        std::uniform_real_distribution<float> distY(-definition.physics.explosionRange.y, definition.physics.explosionRange.y);
        std::uniform_real_distribution<float> distZ(-definition.physics.explosionRange.z, definition.physics.explosionRange.z);

        offset = {
            distX(randomEngine),
            distY(randomEngine),
            distZ(randomEngine)
        };
    }
    particle.transform.translate = position + offset;

    // 速度の設定
    particle.velocity = GenerateRandomVelocity(definition, randomEngine, offset);

    // 色の設定
    particle.color = definition.appearance.startColor;

    // 寿命の設定
    particle.lifeTime = GenerateRandomLifetime(definition, randomEngine);
    particle.currentTime = 0.0f;

    // 回転速度の保存（カスタムデータとして）
    // 注: ParticleDataに rotationSpeed メンバーがない場合は追加が必要

    return particle;
}

///-------------------------------------------/// 
/// バースト生成
///-------------------------------------------///
std::list<ParticleData> ParticleFactory::CreateParticleBurst(
    const ParticleDefinition& definition,
    std::mt19937& randomEngine,
    const Vector3& position,
    uint32_t count) {

    std::list<ParticleData> particles;

    for (uint32_t i = 0; i < count; ++i) {
        particles.push_back(CreateParticle(definition, randomEngine, position));
    }

    return particles;
}

///-------------------------------------------/// 
/// パーティクル更新
///-------------------------------------------///
void ParticleFactory::UpdateParticle(
    ParticleData& particle,
    const ParticleDefinition& definition,
    float deltaTime,
    std::mt19937& randomEngine) {

    // 重力の適用
    if (definition.physics.gravity != 0.0f) {
        particle.velocity.y += definition.physics.gravity * deltaTime;
    }

    // 加速度の適用
    particle.velocity += definition.physics.acceleration * deltaTime;

    // 位置の更新
    particle.transform.translate += particle.velocity * deltaTime;

    // 回転の更新
    if (definition.rotation.enableRotation) {
        Vector3 rotationSpeed = GenerateRandomRotationSpeed(definition, randomEngine);
        if (definition.rotation.randomRotation) {
            particle.transform.rotate += rotationSpeed * deltaTime;
        } else {
            // 固定速度で回転
            particle.transform.rotate += definition.rotation.rotationSpeedMin * deltaTime;
        }
    }

    // 時間の更新
    particle.currentTime += deltaTime;

    // 寿命に基づく補間率
    float t = particle.currentTime / particle.lifeTime;

    // 色のグラデーション
    if (definition.appearance.useColorGradient) {
        particle.color = LerpColor(
            definition.appearance.startColor,
            definition.appearance.endColor,
            t);
    }

    // スケールアニメーション
    if (definition.appearance.useScaleAnimation) {
        Vector3 startScale = (definition.appearance.startScaleMin + definition.appearance.startScaleMax) * 0.5f;
        particle.transform.scale = LerpScale(
            startScale,
            definition.appearance.endScale,
            t);
    }
}

///-------------------------------------------/// 
/// ランダム速度生成
///-------------------------------------------///
Vector3 ParticleFactory::GenerateRandomVelocity(
    const ParticleDefinition& definition,
    std::mt19937& randomEngine,
    const Vector3& position) {

    Vector3 velocity;

    if (definition.physics.useRandomVelocity) {
        // ランダム範囲内で速度を生成
        std::uniform_real_distribution<float> distX(
            definition.physics.velocityMin.x,
            definition.physics.velocityMax.x);
        std::uniform_real_distribution<float> distY(
            definition.physics.velocityMin.y,
            definition.physics.velocityMax.y);
        std::uniform_real_distribution<float> distZ(
            definition.physics.velocityMin.z,
            definition.physics.velocityMax.z);

        velocity = {
            distX(randomEngine),
            distY(randomEngine),
            distZ(randomEngine)
        };
    } else {
        // 爆発方向に基づく速度
        float rangeSum = definition.physics.explosionRange.x +
            definition.physics.explosionRange.y +
            definition.physics.explosionRange.z;

        if (rangeSum > 0.0f) {
            // 正規化されたオフセット位置から放射状の速度を計算
            Vector3 normalizedOffset = Normalize(position);

            // 速度の最大値の平均を使用
            float averageMaxVelocity = (definition.physics.velocityMax.x +
                definition.physics.velocityMax.y +
                definition.physics.velocityMax.z) / 3.0f;

            velocity = normalizedOffset * averageMaxVelocity;
        } else {
            velocity = definition.physics.velocityMin;
        }
    }

    // 上方向の力を追加
    velocity.y += definition.physics.upwardForce;

    return velocity;
}

///-------------------------------------------/// 
/// ランダムスケール生成
///-------------------------------------------///
Vector3 ParticleFactory::GenerateRandomScale(
    const ParticleDefinition& definition,
    std::mt19937& randomEngine) {

    std::uniform_real_distribution<float> distX(
        definition.appearance.startScaleMin.x,
        definition.appearance.startScaleMax.x);
    std::uniform_real_distribution<float> distY(
        definition.appearance.startScaleMin.y,
        definition.appearance.startScaleMax.y);
    std::uniform_real_distribution<float> distZ(
        definition.appearance.startScaleMin.z,
        definition.appearance.startScaleMax.z);

    return {
        distX(randomEngine),
        distY(randomEngine),
        distZ(randomEngine)
    };
}

///-------------------------------------------/// 
/// ランダム回転速度生成
///-------------------------------------------///
Vector3 ParticleFactory::GenerateRandomRotationSpeed(
    const ParticleDefinition& definition,
    std::mt19937& randomEngine) {

    std::uniform_real_distribution<float> distX(
        definition.rotation.rotationSpeedMin.x,
        definition.rotation.rotationSpeedMax.x);
    std::uniform_real_distribution<float> distY(
        definition.rotation.rotationSpeedMin.y,
        definition.rotation.rotationSpeedMax.y);
    std::uniform_real_distribution<float> distZ(
        definition.rotation.rotationSpeedMin.z,
        definition.rotation.rotationSpeedMax.z);

    return {
        distX(randomEngine),
        distY(randomEngine),
        distZ(randomEngine)
    };
}

///-------------------------------------------/// 
/// ランダム初期回転角度生成
///-------------------------------------------///
Vector3 ParticleFactory::GenerateRandomInitialRotation(
    const ParticleDefinition& definition,
    std::mt19937& randomEngine) {

    if (definition.rotation.randomInitialRotation) {
        // ランダム初期回転の場合、最小〜最大の範囲でランダム生成
        std::uniform_real_distribution<float> distX(
            definition.rotation.initialRotationMin.x,
            definition.rotation.initialRotationMax.x);
        std::uniform_real_distribution<float> distY(
            definition.rotation.initialRotationMin.y,
            definition.rotation.initialRotationMax.y);
        std::uniform_real_distribution<float> distZ(
            definition.rotation.initialRotationMin.z,
            definition.rotation.initialRotationMax.z);

        return {
            distX(randomEngine),
            distY(randomEngine),
            distZ(randomEngine)
        };
    } else {
        // 固定初期回転の場合、最小値を使用
        return definition.rotation.initialRotationMin;
    }
}

///-------------------------------------------/// 
/// ランダム寿命生成
///-------------------------------------------///
float ParticleFactory::GenerateRandomLifetime(
    const ParticleDefinition& definition,
    std::mt19937& randomEngine) {

    std::uniform_real_distribution<float> dist(
        definition.emission.lifetimeMin,
        definition.emission.lifetimeMax);

    return dist(randomEngine);
}

///-------------------------------------------/// 
/// 色の補間
///-------------------------------------------///
Vector4 ParticleFactory::LerpColor(const Vector4& start, const Vector4& end, float t) {
    t = std::clamp(t, 0.0f, 1.0f);
    return {
        start.x + (end.x - start.x) * t,
        start.y + (end.y - start.y) * t,
        start.z + (end.z - start.z) * t,
        start.w + (end.w - start.w) * t
    };
}

///-------------------------------------------/// 
/// スケールの補間
///-------------------------------------------///
Vector3 ParticleFactory::LerpScale(const Vector3& start, const Vector3& end, float t) {
    t = std::clamp(t, 0.0f, 1.0f);
    return {
        start.x + (end.x - start.x) * t,
        start.y + (end.y - start.y) * t,
        start.z + (end.z - start.z) * t
    };
}