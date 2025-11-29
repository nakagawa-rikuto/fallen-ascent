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
}