#pragma once
/// ===Include=== ///
// Engine
#include "Engine/DataInfo/CData.h"
#include "Engine/DataInfo/ParticleData.h"
// c++
#include <string>
// JSON
#include <json.hpp>

///=====================================================/// 
/// パーティクル定義データ
/// エディタとJSON間のデータ橋渡し役
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

///=====================================================/// 
/// shapeTypeのJSON変換ヘルパー
///=====================================================///
namespace nlohmann {
    template <>
    struct adl_serializer<shapeType> {
        static void to_json(json& j, const shapeType& type) {
            switch (type) {
            case shapeType::kCircle:   j = "Circle"; break;
            case shapeType::kCylinder: j = "Cylinder"; break;
            case shapeType::kNone:     j = "None"; break;
            default:                   j = "None"; break;
            }
        }

        static void from_json(const json& j, shapeType& type) {
            std::string str = j.get<std::string>();
            if (str == "Circle") type = shapeType::kCircle;
            else if (str == "Cylinder") type = shapeType::kCylinder;
            else type = shapeType::kNone;
        }
    };

    // EmissionPatternのJSON変換
    template <>
    struct adl_serializer<ParticleEmissionPattern::Pattern> {
        static void to_json(json& j, const ParticleEmissionPattern::Pattern& pattern) {
            switch (pattern) {
            case ParticleEmissionPattern::Pattern::Point:  j = "Point"; break;
            case ParticleEmissionPattern::Pattern::Sphere: j = "Sphere"; break;
            case ParticleEmissionPattern::Pattern::Cone:   j = "Cone"; break;
            case ParticleEmissionPattern::Pattern::Trail:  j = "Trail"; break;
            case ParticleEmissionPattern::Pattern::Ring:   j = "Ring"; break;
            case ParticleEmissionPattern::Pattern::Burst:  j = "Burst"; break;
            default: j = "Point"; break;
            }
        }

        static void from_json(const json& j, ParticleEmissionPattern::Pattern& pattern) {
            std::string str = j.get<std::string>();
            if (str == "Point") pattern = ParticleEmissionPattern::Pattern::Point;
            else if (str == "Sphere") pattern = ParticleEmissionPattern::Pattern::Sphere;
            else if (str == "Cone") pattern = ParticleEmissionPattern::Pattern::Cone;
            else if (str == "Trail") pattern = ParticleEmissionPattern::Pattern::Trail;
            else if (str == "Ring") pattern = ParticleEmissionPattern::Pattern::Ring;
            else if (str == "Burst") pattern = ParticleEmissionPattern::Pattern::Burst;
            else pattern = ParticleEmissionPattern::Pattern::Point;
        }
    };

    // ScaleAnimationTypeのJSON変換
    template <>
    struct adl_serializer<ParticleScaleAnimation::AnimationType> {
        static void to_json(json& j, const ParticleScaleAnimation::AnimationType& type) {
            switch (type) {
            case ParticleScaleAnimation::AnimationType::None:   j = "None"; break;
            case ParticleScaleAnimation::AnimationType::Linear: j = "Linear"; break;
            case ParticleScaleAnimation::AnimationType::EaseIn: j = "EaseIn"; break;
            case ParticleScaleAnimation::AnimationType::EaseOut: j = "EaseOut"; break;
            case ParticleScaleAnimation::AnimationType::Bounce: j = "Bounce"; break;
            case ParticleScaleAnimation::AnimationType::Pulse:  j = "Pulse"; break;
            default: j = "None"; break;
            }
        }

        static void from_json(const json& j, ParticleScaleAnimation::AnimationType& type) {
            std::string str = j.get<std::string>();
            if (str == "None") type = ParticleScaleAnimation::AnimationType::None;
            else if (str == "Linear") type = ParticleScaleAnimation::AnimationType::Linear;
            else if (str == "EaseIn") type = ParticleScaleAnimation::AnimationType::EaseIn;
            else if (str == "EaseOut") type = ParticleScaleAnimation::AnimationType::EaseOut;
            else if (str == "Bounce") type = ParticleScaleAnimation::AnimationType::Bounce;
            else if (str == "Pulse") type = ParticleScaleAnimation::AnimationType::Pulse;
            else type = ParticleScaleAnimation::AnimationType::None;
        }
    };
}