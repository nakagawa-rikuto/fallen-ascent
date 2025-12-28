#include "ParticleParameter.h"
#include <unordered_map>

///-------------------------------------------/// 
/// enum classを文字列に変換
///-------------------------------------------///
std::string ParticleParameterUtils::ToString(ParticleParameter param) {
    switch (param) {
        // 物理パラメータ
    case ParticleParameter::Gravity:           return "gravity";
    case ParticleParameter::ExplosionRadiusX:  return "explosionRadiusX";
    case ParticleParameter::ExplosionRadiusY:  return "explosionRadiusY";
    case ParticleParameter::ExplosionRadiusZ:  return "explosionRadiusZ";
    case ParticleParameter::UpwardForce:       return "upwardForce";
    case ParticleParameter::AccelerationX:     return "accelerationX";
    case ParticleParameter::AccelerationY:     return "accelerationY";
    case ParticleParameter::AccelerationZ:     return "accelerationZ";

        // 発生パラメータ
    case ParticleParameter::EmissionRate:      return "emissionRate";
    case ParticleParameter::Frequency:         return "frequency";
    case ParticleParameter::LifetimeMin:       return "lifetimeMin";
    case ParticleParameter::LifetimeMax:       return "lifetimeMax";
    case ParticleParameter::BurstCount:        return "burstCount";

        // 見た目パラメータ
    case ParticleParameter::StartColorR:       return "startColorR";
    case ParticleParameter::StartColorG:       return "startColorG";
    case ParticleParameter::StartColorB:       return "startColorB";
    case ParticleParameter::StartColorA:       return "startColorA";
    case ParticleParameter::EndColorR:         return "endColorR";
    case ParticleParameter::EndColorG:         return "endColorG";
    case ParticleParameter::EndColorB:         return "endColorB";
    case ParticleParameter::EndColorA:         return "endColorA";

        // 回転パラメータ
    case ParticleParameter::RotationSpeedX:    return "rotationSpeedX";
    case ParticleParameter::RotationSpeedY:    return "rotationSpeedY";
    case ParticleParameter::RotationSpeedZ:    return "rotationSpeedZ";

        // 速度パラメータ
    case ParticleParameter::VelocityMinX:      return "velocityMinX";
    case ParticleParameter::VelocityMinY:      return "velocityMinY";
    case ParticleParameter::VelocityMinZ:      return "velocityMinZ";
    case ParticleParameter::VelocityMaxX:      return "velocityMaxX";
    case ParticleParameter::VelocityMaxY:      return "velocityMaxY";
    case ParticleParameter::VelocityMaxZ:      return "velocityMaxZ";

    default: return "unknown";
    }
}

///-------------------------------------------/// 
/// 文字列からenum classに変換
///-------------------------------------------///
bool ParticleParameterUtils::FromString(const std::string& str, ParticleParameter& outParam) {
    static const std::unordered_map<std::string, ParticleParameter> stringToEnum = {
        // 物理パラメータ
        {"gravity", ParticleParameter::Gravity},
        {"explosionRadiuX", ParticleParameter::ExplosionRadiusX},
        {"explosionRadiuY", ParticleParameter::ExplosionRadiusY},
        {"explosionRadiuZ", ParticleParameter::ExplosionRadiusZ},
        {"upwardForce", ParticleParameter::UpwardForce},
        {"accelerationX", ParticleParameter::AccelerationX},
        {"accelerationY", ParticleParameter::AccelerationY},
        {"accelerationZ", ParticleParameter::AccelerationZ},

        // 発生パラメータ
        {"emissionRate", ParticleParameter::EmissionRate},
        {"frequency", ParticleParameter::Frequency},
        {"lifetimeMin", ParticleParameter::LifetimeMin},
        {"lifetimeMax", ParticleParameter::LifetimeMax},
        {"burstCount", ParticleParameter::BurstCount},

        // 見た目パラメータ
        {"startColorR", ParticleParameter::StartColorR},
        {"startColorG", ParticleParameter::StartColorG},
        {"startColorB", ParticleParameter::StartColorB},
        {"startColorA", ParticleParameter::StartColorA},
        {"endColorR", ParticleParameter::EndColorR},
        {"endColorG", ParticleParameter::EndColorG},
        {"endColorB", ParticleParameter::EndColorB},
        {"endColorA", ParticleParameter::EndColorA},

        // 回転パラメータ
        {"rotationSpeedX", ParticleParameter::RotationSpeedX},
        {"rotationSpeedY", ParticleParameter::RotationSpeedY},
        {"rotationSpeedZ", ParticleParameter::RotationSpeedZ},

        // 速度パラメータ
        {"velocityMinX", ParticleParameter::VelocityMinX},
        {"velocityMinY", ParticleParameter::VelocityMinY},
        {"velocityMinZ", ParticleParameter::VelocityMinZ},
        {"velocityMaxX", ParticleParameter::VelocityMaxX},
        {"velocityMaxY", ParticleParameter::VelocityMaxY},
        {"velocityMaxZ", ParticleParameter::VelocityMaxZ},
    };

    auto it = stringToEnum.find(str);
    if (it != stringToEnum.end()) {
        outParam = it->second;
        return true;
    }
    return false;
}

///-------------------------------------------/// 
/// パラメータの説明を取得
///-------------------------------------------///
std::string ParticleParameterUtils::GetDescription(ParticleParameter param) {
    switch (param) {
        // 物理パラメータ
    case ParticleParameter::Gravity:           return "重力加速度";
    case ParticleParameter::ExplosionRadiusX:  return "X軸の発生半径";
    case ParticleParameter::ExplosionRadiusY:  return "Y軸の発生半径";
    case ParticleParameter::ExplosionRadiusZ:  return "Z軸の発生半径";
    case ParticleParameter::UpwardForce:       return "上方向の初期加速";
    case ParticleParameter::AccelerationX:     return "X軸加速度";
    case ParticleParameter::AccelerationY:     return "Y軸加速度";
    case ParticleParameter::AccelerationZ:     return "Z軸加速度";

        // 発生パラメータ
    case ParticleParameter::EmissionRate:      return "発生レート（個/秒）";
    case ParticleParameter::Frequency:         return "発生頻度（秒）";
    case ParticleParameter::LifetimeMin:       return "最小寿命（秒）";
    case ParticleParameter::LifetimeMax:       return "最大寿命（秒）";
    case ParticleParameter::BurstCount:        return "バースト数";

        // 見た目パラメータ
    case ParticleParameter::StartColorR:       return "開始色（赤）";
    case ParticleParameter::StartColorG:       return "開始色（緑）";
    case ParticleParameter::StartColorB:       return "開始色（青）";
    case ParticleParameter::StartColorA:       return "開始色（透明度）";
    case ParticleParameter::EndColorR:         return "終了色（赤）";
    case ParticleParameter::EndColorG:         return "終了色（緑）";
    case ParticleParameter::EndColorB:         return "終了色（青）";
    case ParticleParameter::EndColorA:         return "終了色（透明度）";

        // 回転パラメータ
    case ParticleParameter::RotationSpeedX:    return "X軸回転速度";
    case ParticleParameter::RotationSpeedY:    return "Y軸回転速度";
    case ParticleParameter::RotationSpeedZ:    return "Z軸回転速度";

        // 速度パラメータ
    case ParticleParameter::VelocityMinX:      return "最小速度X";
    case ParticleParameter::VelocityMinY:      return "最小速度Y";
    case ParticleParameter::VelocityMinZ:      return "最小速度Z";
    case ParticleParameter::VelocityMaxX:      return "最大速度X";
    case ParticleParameter::VelocityMaxY:      return "最大速度Y";
    case ParticleParameter::VelocityMaxZ:      return "最大速度Z";

    default: return "不明なパラメータ";
    }
}