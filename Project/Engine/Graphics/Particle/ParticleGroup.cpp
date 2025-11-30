#include "ParticleGroup.h"
#include "ParticleFactory.h"
// Camera
#include "application/Game/Camera/GameCamera.h"
// Service
#include "Engine/System/Service/GraphicsResourceGetter.h"
#include "Engine/System/Service/CameraService.h"
#include "Engine/System/Service/DeltaTimeSevice.h"
// Math
#include "Math/sMath.h"
#include "Math/MatrixMath.h"
// c++
#include <numbers>

///-------------------------------------------/// 
/// デストラクタ
///-------------------------------------------///
ParticleGroup::~ParticleGroup() {
    group_.particles.clear();
    group_.particle.reset();
}

///-------------------------------------------/// 
/// 初期化（パラメータ駆動型）
///-------------------------------------------///
void ParticleGroup::Initialze(const Vector3& translate, const ParticleDefinition& definition) {
    /// ===定義を保存=== ///
    definition_ = definition;

    /// ===乱数生成器の初期化=== ///
    std::random_device seedGenerator;
    randomEngine_.seed(seedGenerator());

    /// ===最大パーティクル数の設定=== ///
    group_.maxInstance = definition_.maxInstance;
    group_.numInstance = 0;

    /// ===トランスフォームの初期化=== ///
    group_.transform = { {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}, translate };
    group_.cameraTransform = {
       {1.0f, 1.0f, 1.0f},
       {std::numbers::pi_v<float> / 3.0f, std::numbers::pi_v<float>, 0.0f},
       {0.0f, 23.0f, 10.0f}
    };

    /// ===パーティクルグループの初期化=== ///
    InstancingInit(definition_.modelName, translate, group_.maxInstance, definition_.shape);

    /// ===Cameraの設定=== ///
    group_.camera = CameraService::GetActiveCamera().get();

    /// ===発生タイマーの初期化=== ///
    group_.frequencyTimer = 0.0f;
    group_.hasEmitted = false;

    // デルタタイムの取得
    kDeltaTime_ = DeltaTimeSevice::GetDeltaTime();

    // テクスチャの設定
    if (!definition_.appearance.texturePath.empty()) {
        SetTexture(definition_.appearance.texturePath);
    }
}

///-------------------------------------------/// 
/// 更新
///-------------------------------------------///
void ParticleGroup::Update() {
    // デルタタイムの取得
    kDeltaTime_ = DeltaTimeSevice::GetDeltaTime();

    // パーティクルの発生処理
    Emit();

    // パーティクルの更新処理
    UpdateParticles();
}

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void ParticleGroup::Draw(BlendMode mode) {
    if (group_.numInstance > 0) {
        group_.particle->Darw(group_.numInstance, mode);
    }
}

///-------------------------------------------/// 
/// 終了判定
///-------------------------------------------///
bool ParticleGroup::IsFinish() const {
    // バーストモードの場合は全パーティクルが消えたらtrue
    if (definition_.emission.isBurst) {
        return group_.particles.empty() && group_.hasEmitted;
    }

    // 連続発生モードの場合は常にfalse（手動で停止する必要がある）
    return false;
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
// テクスチャ設定
void ParticleGroup::SetTexture(const std::string& fileName) {
    if (group_.particle) {
        group_.particle->SetTexture(fileName);
    }
}
// パーティクル定義設定
void ParticleGroup::SetDefinition(const ParticleDefinition& definition) {
    definition_ = definition;

    // テクスチャも更新
    if (!definition_.appearance.texturePath.empty()) {
        SetTexture(definition_.appearance.texturePath);
    }
}
// パラメーター変更
void ParticleGroup::SetParameter(ParticleParameter param, float value) {
    // 実行時にパラメータを変更
    switch (param) {
        // 物理パラメータ
    case ParticleParameter::Gravity:
        definition_.physics.gravity = value;
        break;
    case ParticleParameter::ExplosionRadius:
        definition_.physics.explosionRadius = value;
        break;
    case ParticleParameter::UpwardForce:
        definition_.physics.upwardForce = value;
        break;
    case ParticleParameter::AccelerationX:
        definition_.physics.acceleration.x = value;
        break;
    case ParticleParameter::AccelerationY:
        definition_.physics.acceleration.y = value;
        break;
    case ParticleParameter::AccelerationZ:
        definition_.physics.acceleration.z = value;
        break;

        // 発生パラメータ
    case ParticleParameter::EmissionRate:
        definition_.emission.emissionRate = value;
        break;
    case ParticleParameter::Frequency:
        definition_.emission.frequency = value;
        break;
    case ParticleParameter::LifetimeMin:
        definition_.emission.lifetimeMin = value;
        break;
    case ParticleParameter::LifetimeMax:
        definition_.emission.lifetimeMax = value;
        break;
    case ParticleParameter::BurstCount:
        definition_.emission.burstCount = static_cast<uint32_t>(value);
        break;

        // 見た目パラメータ
    case ParticleParameter::StartColorR:
        definition_.appearance.startColor.x = value;
        break;
    case ParticleParameter::StartColorG:
        definition_.appearance.startColor.y = value;
        break;
    case ParticleParameter::StartColorB:
        definition_.appearance.startColor.z = value;
        break;
    case ParticleParameter::StartColorA:
        definition_.appearance.startColor.w = value;
        break;
    case ParticleParameter::EndColorR:
        definition_.appearance.endColor.x = value;
        break;
    case ParticleParameter::EndColorG:
        definition_.appearance.endColor.y = value;
        break;
    case ParticleParameter::EndColorB:
        definition_.appearance.endColor.z = value;
        break;
    case ParticleParameter::EndColorA:
        definition_.appearance.endColor.w = value;
        break;

        // 回転パラメータ
    case ParticleParameter::RotationSpeedX:
        definition_.rotation.rotationSpeedMin.x = value;
        break;
    case ParticleParameter::RotationSpeedY:
        definition_.rotation.rotationSpeedMin.y = value;
        break;
    case ParticleParameter::RotationSpeedZ:
        definition_.rotation.rotationSpeedMin.z = value;
        break;

        // 速度パラメータ
    case ParticleParameter::VelocityMinX:
        definition_.physics.velocityMin.x = value;
        break;
    case ParticleParameter::VelocityMinY:
        definition_.physics.velocityMin.y = value;
        break;
    case ParticleParameter::VelocityMinZ:
        definition_.physics.velocityMin.z = value;
        break;
    case ParticleParameter::VelocityMaxX:
        definition_.physics.velocityMax.x = value;
        break;
    case ParticleParameter::VelocityMaxY:
        definition_.physics.velocityMax.y = value;
        break;
    case ParticleParameter::VelocityMaxZ:
        definition_.physics.velocityMax.z = value;
        break;
    }
}

void ParticleGroup::SetEmitterPosition(const Vector3& position) { group_.transform.translate = position; }

void ParticleGroup::MoveEmitterPosition(const Vector3& offset) { group_.transform.translate += offset; }

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
const ParticleDefinition& ParticleGroup::GetDefinition() const {return definition_;}
uint32_t ParticleGroup::GetActiveParticleCount() const {return static_cast<uint32_t>(group_.particles.size());}
const Vector3& ParticleGroup::GetEmitterPosition() const {return group_.transform.translate;}

///-------------------------------------------/// 
/// インスタンシング初期化
///-------------------------------------------///
void ParticleGroup::InstancingInit(const std::string& modelName, const Vector3& translate, const uint32_t maxInstance, shapeType type) {
    /// ===パーティクルグループの初期化=== ///
    group_.particle = std::make_unique<ParticleSetUp>();
    group_.particle->Initialze(modelName, maxInstance, type);
    translate;
}

///-------------------------------------------/// 
/// インスタンシング更新
///-------------------------------------------///
void ParticleGroup::InstancingUpdate(std::list<ParticleData>::iterator it) {
    // ワールド行列とWVP行列の計算
    Matrix4x4 worldMatrix = Math::MakeAffineEulerMatrix(
        it->transform.scale,
        it->transform.rotate,
        it->transform.translate);
    Matrix4x4 wvpMatrix;

    // カメラが存在する場合はWVP行列を計算
    if (group_.camera) {
        const Matrix4x4& viewProjectionMatrix = group_.camera->GetViewProjectionMatrix();
        wvpMatrix = Multiply(worldMatrix, viewProjectionMatrix);
    } else {
        Matrix4x4 viewMatrix = Math::Inverse4x4(
            Math::MakeAffineEulerMatrix(
                group_.cameraTransform.scale,
                group_.cameraTransform.rotate,
                group_.cameraTransform.translate));
        Matrix4x4 projectionMatrix = Math::MakePerspectiveFovMatrix(
            0.45f,
            static_cast<float>(GraphicsResourceGetter::GetWindowWidth()) /
            static_cast<float>(GraphicsResourceGetter::GetWindowHeight()),
            0.1f, 100.0f);
        wvpMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
    }

    // インスタンシングデータを設定
    group_.particle->SetInstancingData(group_.numInstance, it->color, wvpMatrix, worldMatrix);
    ++group_.numInstance;
}

///-------------------------------------------/// 
/// パーティクル生成（パラメータ駆動型）
///-------------------------------------------///
ParticleData ParticleGroup::MakeParticle(const Vector3& translate) {
    return ParticleFactory::CreateParticle(definition_, randomEngine_, translate);
}

///-------------------------------------------/// 
/// パーティクル発生処理
///-------------------------------------------///
void ParticleGroup::Emit() {
    // バーストモード
    if (definition_.emission.isBurst) {
        if (!group_.hasEmitted) {
            // 一度だけ大量発生
            std::list<ParticleData> burstParticles = ParticleFactory::CreateParticleBurst(
                definition_,
                randomEngine_,
                group_.transform.translate,
                definition_.emission.burstCount);

            group_.particles.splice(group_.particles.end(), burstParticles);
            group_.hasEmitted = true;
        }
        return;
    }

    // 連続発生モード
    group_.frequencyTimer += kDeltaTime_;

    if (group_.frequencyTimer >= definition_.emission.frequency) {
        // 発生レートに基づいてパーティクルを生成
        uint32_t emitCount = static_cast<uint32_t>(
            definition_.emission.emissionRate * definition_.emission.frequency);

        for (uint32_t i = 0; i < emitCount; ++i) {
            if (group_.particles.size() < group_.maxInstance) {
                group_.particles.push_back(MakeParticle(group_.transform.translate));
            }
        }

        group_.frequencyTimer = 0.0f;
    }
}

///-------------------------------------------/// 
/// パーティクル更新処理
///-------------------------------------------///
void ParticleGroup::UpdateParticles() {
    group_.numInstance = 0; // インスタンス数をリセット

    for (auto it = group_.particles.begin(); it != group_.particles.end();) {
        // 寿命チェック
        if (it->currentTime >= it->lifeTime) {
            it = group_.particles.erase(it);
            continue;
        }

        // Factoryの更新ロジックを使用
        ParticleFactory::UpdateParticle(*it, definition_, kDeltaTime_, randomEngine_);

        // インスタンシング更新
        InstancingUpdate(it);
        ++it;
    }
}