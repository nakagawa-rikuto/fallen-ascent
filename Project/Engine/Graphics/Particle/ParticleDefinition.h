#pragma once
/// ===Include=== ///
// Engine
#include "Engine/DataInfo/CData.h"
#include "Engine/DataInfo/ParticleData.h"
// c++
#include <string>
// JSON
#include <json.hpp>

namespace MiiEngine {
    ///=====================================================/// 
    /// パーティクル定義データ
    ///=====================================================///
    struct ParticleDefinition {
    public:
        /// ===基本設定=== ///
        std::string name = "NewParticle";
        std::string modelName = "plane";
        shapeType shape = shapeType::kNone;
        uint32_t maxInstance = 100;

        /// ===設定グループ=== ///
        ParticlePhysicsSettings physics;       // 物理設定
        ParticleAppearanceSettings appearance; // 見た目設定
        ParticleRotationSettings rotation;     // 回転設定
        ParticleEmissionSettings emission;     // 発生設定
        ParticleAdvancedSettings advanced;     // 高度設定


        /// ===JSON変換=== ///
        /// <summary>
        /// ParticleDefinitionをJSON形式に変換
        /// </summary>
        /// <returns>変換されたJSONオブジェクト</returns>
        nlohmann::json ToJson() const;

        /// <summary>
        /// JSONからParticleDefinitionを生成
        /// </summary>
        /// <param name="json">読み込むJSONオブジェクト</param>
        /// <returns>生成されたParticleDefinition</returns>
        static ParticleDefinition FromJson(const nlohmann::json& json);

    private:
        /// ===JSONヘルパー関数=== ///
        // Vector3のJSON変換
        static nlohmann::json Vector3ToJson(const Vector3& vec);
        static Vector3 JsonToVector3(const nlohmann::json& json);
        // Vector4のJSON変換
        static nlohmann::json Vector4ToJson(const Vector4& vec);
        static Vector4 JsonToVector4(const nlohmann::json& json);

        // 高度設定用のJson変換
        static nlohmann::json MotionSettingsToJson(const ParticleMotionSettings& motion);
        static ParticleMotionSettings JsonToMotionSettings(const nlohmann::json& json);
        static nlohmann::json EmissionPatternToJson(const ParticleEmissionPattern& pattern);
        static ParticleEmissionPattern JsonToEmissionPattern(const nlohmann::json& json);
        static nlohmann::json ColorGradientToJson(const ParticleColorGradient& gradient);
        static ParticleColorGradient JsonToColorGradient(const nlohmann::json& json);
        static nlohmann::json ScaleAnimationToJson(const ParticleScaleAnimation animation);
        static ParticleScaleAnimation JsonToScaleAnimation(const nlohmann::json& json);
    };
}

///=====================================================/// 
/// shapeTypeのJSON変換ヘルパー
///=====================================================///
namespace nlohmann {
    template <>
    struct adl_serializer<MiiEngine::shapeType> {
        static void to_json(json& j, const MiiEngine::shapeType& type) {
            switch (type) {
            case MiiEngine::shapeType::kCircle:   j = "Circle"; break;
            case MiiEngine::shapeType::kCylinder: j = "Cylinder"; break;
            case MiiEngine::shapeType::kNone:     j = "None"; break;
            default:                   j = "None"; break;
            }
        }

        static void from_json(const json& j, MiiEngine::shapeType& type) {
            std::string str = j.get<std::string>();
            if (str == "Circle") type = MiiEngine::shapeType::kCircle;
            else if (str == "Cylinder") type = MiiEngine::shapeType::kCylinder;
            else type = MiiEngine::shapeType::kNone;
        }
    };

    // EmissionPatternのJSON変換
    template <>
    struct adl_serializer<MiiEngine::ParticleEmissionPattern::Pattern> {
        static void to_json(json& j, const MiiEngine::ParticleEmissionPattern::Pattern& pattern) {
            switch (pattern) {
            case MiiEngine::ParticleEmissionPattern::Pattern::Point:  j = "Point"; break;
            case MiiEngine::ParticleEmissionPattern::Pattern::Sphere: j = "Sphere"; break;
            case MiiEngine::ParticleEmissionPattern::Pattern::Cone:   j = "Cone"; break;
            case MiiEngine::ParticleEmissionPattern::Pattern::Trail:  j = "Trail"; break;
            case MiiEngine::ParticleEmissionPattern::Pattern::Ring:   j = "Ring"; break;
            case MiiEngine::ParticleEmissionPattern::Pattern::Burst:  j = "Burst"; break;
            default: j = "Point"; break;
            }
        }

        static void from_json(const json& j, MiiEngine::ParticleEmissionPattern::Pattern& pattern) {
            std::string str = j.get<std::string>();
            if (str == "Point") pattern = MiiEngine::ParticleEmissionPattern::Pattern::Point;
            else if (str == "Sphere") pattern = MiiEngine::ParticleEmissionPattern::Pattern::Sphere;
            else if (str == "Cone") pattern = MiiEngine::ParticleEmissionPattern::Pattern::Cone;
            else if (str == "Trail") pattern = MiiEngine::ParticleEmissionPattern::Pattern::Trail;
            else if (str == "Ring") pattern = MiiEngine::ParticleEmissionPattern::Pattern::Ring;
            else if (str == "Burst") pattern = MiiEngine::ParticleEmissionPattern::Pattern::Burst;
            else pattern = MiiEngine::ParticleEmissionPattern::Pattern::Point;
        }
    };

    // ScaleAnimationTypeのJSON変換
    template <>
    struct adl_serializer<MiiEngine::ParticleScaleAnimation::AnimationType> {
        static void to_json(json& j, const MiiEngine::ParticleScaleAnimation::AnimationType& type) {
            switch (type) {
            case MiiEngine::ParticleScaleAnimation::AnimationType::None:   j = "None"; break;
            case MiiEngine::ParticleScaleAnimation::AnimationType::Linear: j = "Linear"; break;
            case MiiEngine::ParticleScaleAnimation::AnimationType::EaseIn: j = "EaseIn"; break;
            case MiiEngine::ParticleScaleAnimation::AnimationType::EaseOut: j = "EaseOut"; break;
            case MiiEngine::ParticleScaleAnimation::AnimationType::Bounce: j = "Bounce"; break;
            case MiiEngine::ParticleScaleAnimation::AnimationType::Pulse:  j = "Pulse"; break;
            default: j = "None"; break;
            }
        }

        static void from_json(const json& j, MiiEngine::ParticleScaleAnimation::AnimationType& type) {
            std::string str = j.get<std::string>();
            if (str == "None") type = MiiEngine::ParticleScaleAnimation::AnimationType::None;
            else if (str == "Linear") type = MiiEngine::ParticleScaleAnimation::AnimationType::Linear;
            else if (str == "EaseIn") type = MiiEngine::ParticleScaleAnimation::AnimationType::EaseIn;
            else if (str == "EaseOut") type = MiiEngine::ParticleScaleAnimation::AnimationType::EaseOut;
            else if (str == "Bounce") type = MiiEngine::ParticleScaleAnimation::AnimationType::Bounce;
            else if (str == "Pulse") type = MiiEngine::ParticleScaleAnimation::AnimationType::Pulse;
            else type = MiiEngine::ParticleScaleAnimation::AnimationType::None;
        }
    };
}