#include "ParticleDefinition.h"

///-------------------------------------------/// 
/// JSON変換: ToJson
///-------------------------------------------///
nlohmann::json ParticleDefinition::ToJson() const {
    nlohmann::json j;

    // 基本設定
    j["name"] = name;
    j["modelName"] = modelName;
    j["shape"] = shape;
    j["maxInstance"] = maxInstance;

    // 物理設定
    j["physics"] = {
        {"velocityMin", Vector3ToJson(physics.velocityMin)},
        {"velocityMax", Vector3ToJson(physics.velocityMax)},
        {"acceleration", Vector3ToJson(physics.acceleration)},
        {"gravity", physics.gravity},
        {"explosionRadius", physics.explosionRadius},
        {"upwardForce", physics.upwardForce},
        {"useRandomVelocity", physics.useRandomVelocity}
    };

    // 見た目設定
    j["appearance"] = {
        {"startColor", Vector4ToJson(appearance.startColor)},
        {"endColor", Vector4ToJson(appearance.endColor)},
        {"startScaleMin", Vector3ToJson(appearance.startScaleMin)},
        {"startScaleMax", Vector3ToJson(appearance.startScaleMax)},
        {"endScale", Vector3ToJson(appearance.endScale)},
        {"texturePath", appearance.texturePath},
        {"useColorGradient", appearance.useColorGradient},
        {"useScaleAnimation", appearance.useScaleAnimation}
    };

    // 回転設定
    j["rotation"] = {
        {"rotationSpeedMin", Vector3ToJson(rotation.rotationSpeedMin)},
        {"rotationSpeedMax", Vector3ToJson(rotation.rotationSpeedMax)},
        {"randomRotation", rotation.randomRotation},
        {"enableRotation", rotation.enableRotation}
    };

    // 発生設定
    j["emission"] = {
        {"lifetimeMin", emission.lifetimeMin},
        {"lifetimeMax", emission.lifetimeMax},
        {"emissionRate", emission.emissionRate},
        {"burstCount", emission.burstCount},
        {"frequency", emission.frequency},
        {"isBurst", emission.isBurst}
    };

    return j;
}

///-------------------------------------------/// 
/// JSON変換: FromJson
///-------------------------------------------///
ParticleDefinition ParticleDefinition::FromJson(const nlohmann::json& json) {
    ParticleDefinition def;

    // 基本設定
    if (json.contains("name")) def.name = json["name"];
    if (json.contains("modelName")) def.modelName = json["modelName"];
    if (json.contains("shape")) def.shape = json["shape"];
    if (json.contains("maxInstance")) def.maxInstance = json["maxInstance"];

    // 物理設定
    if (json.contains("physics")) {
        const auto& p = json["physics"];
        if (p.contains("velocityMin")) def.physics.velocityMin = JsonToVector3(p["velocityMin"]);
        if (p.contains("velocityMax")) def.physics.velocityMax = JsonToVector3(p["velocityMax"]);
        if (p.contains("acceleration")) def.physics.acceleration = JsonToVector3(p["acceleration"]);
        if (p.contains("gravity")) def.physics.gravity = p["gravity"];
        if (p.contains("explosionRadius")) def.physics.explosionRadius = p["explosionRadius"];
        if (p.contains("upwardForce")) def.physics.upwardForce = p["upwardForce"];
        if (p.contains("useRandomVelocity")) def.physics.useRandomVelocity = p["useRandomVelocity"];
    }

    // 見た目設定
    if (json.contains("appearance")) {
        const auto& a = json["appearance"];
        if (a.contains("startColor")) def.appearance.startColor = JsonToVector4(a["startColor"]);
        if (a.contains("endColor")) def.appearance.endColor = JsonToVector4(a["endColor"]);
        if (a.contains("startScaleMin")) def.appearance.startScaleMin = JsonToVector3(a["startScaleMin"]);
        if (a.contains("startScaleMax")) def.appearance.startScaleMax = JsonToVector3(a["startScaleMax"]);
        if (a.contains("endScale")) def.appearance.endScale = JsonToVector3(a["endScale"]);
        if (a.contains("texturePath")) def.appearance.texturePath = a["texturePath"];
        if (a.contains("useColorGradient")) def.appearance.useColorGradient = a["useColorGradient"];
        if (a.contains("useScaleAnimation")) def.appearance.useScaleAnimation = a["useScaleAnimation"];
    }

    // 回転設定
    if (json.contains("rotation")) {
        const auto& r = json["rotation"];
        if (r.contains("rotationSpeedMin")) def.rotation.rotationSpeedMin = JsonToVector3(r["rotationSpeedMin"]);
        if (r.contains("rotationSpeedMax")) def.rotation.rotationSpeedMax = JsonToVector3(r["rotationSpeedMax"]);
        if (r.contains("randomRotation")) def.rotation.randomRotation = r["randomRotation"];
        if (r.contains("enableRotation")) def.rotation.enableRotation = r["enableRotation"];
    }

    // 発生設定
    if (json.contains("emission")) {
        const auto& e = json["emission"];
        if (e.contains("lifetimeMin")) def.emission.lifetimeMin = e["lifetimeMin"];
        if (e.contains("lifetimeMax")) def.emission.lifetimeMax = e["lifetimeMax"];
        if (e.contains("emissionRate")) def.emission.emissionRate = e["emissionRate"];
        if (e.contains("burstCount")) def.emission.burstCount = e["burstCount"];
        if (e.contains("frequency")) def.emission.frequency = e["frequency"];
        if (e.contains("isBurst")) def.emission.isBurst = e["isBurst"];
    }

    return def;
}

///-------------------------------------------/// 
/// JSONヘルパー: Vector3ToJson
///-------------------------------------------///
nlohmann::json ParticleDefinition::Vector3ToJson(const Vector3& vec) {
    return nlohmann::json::array({ vec.x, vec.y, vec.z });
}

///-------------------------------------------/// 
/// JSONヘルパー: JsonToVector3
///-------------------------------------------///
Vector3 ParticleDefinition::JsonToVector3(const nlohmann::json& json) {
    if (json.is_array() && json.size() >= 3) {
        return Vector3{ json[0], json[1], json[2] };
    }
    return Vector3{ 0.0f, 0.0f, 0.0f };
}

///-------------------------------------------/// 
/// JSONヘルパー: Vector4ToJson
///-------------------------------------------///
nlohmann::json ParticleDefinition::Vector4ToJson(const Vector4& vec) {
    return nlohmann::json::array({ vec.x, vec.y, vec.z, vec.w });
}

///-------------------------------------------/// 
/// JSONヘルパー: JsonToVector4
///-------------------------------------------///
Vector4 ParticleDefinition::JsonToVector4(const nlohmann::json& json) {
    if (json.is_array() && json.size() >= 4) {
        return Vector4{ json[0], json[1], json[2], json[3] };
    }
    return Vector4{ 1.0f, 1.0f, 1.0f, 1.0f };
}