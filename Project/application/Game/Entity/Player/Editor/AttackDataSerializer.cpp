#include "AttackDataSerializer.h"
#include <fstream>
#include <json.hpp>

using json = nlohmann::json;

///-------------------------------------------/// 
/// 攻撃データのJson保存・読み込み
///-------------------------------------------///
bool AttackDataSerializer::SaveToJson(const AttackData& data, const std::string& filepath) {
    try {
        json j;

        // 基本情報
        j["attackName"] = data.attackName;
        j["description"] = data.description;
        j["attackID"] = data.attackID;

        // タイミング設定
        j["activeDuration"] = data.activeDuration;
        j["comboWindowTime"] = data.comboWindowTime;
        j["cooldownTime"] = data.cooldownTime;

        // 軌道設定
        j["curveSegments"] = data.curveSegments;
        j["weaponLength"] = data.weaponLength;

        // ベジェ曲線の制御点
        json trajectoryArray = json::array();
        for (const auto& point : data.trajectoryPoints) {
            json pointJson;
            pointJson["position"] = { point.position.x, point.position.y, point.position.z };
            pointJson["time"] = point.time;
            trajectoryArray.push_back(pointJson);
        }
        j["trajectoryPoints"] = trajectoryArray;

        // 回転設定
        j["startRotation"] = { data.startRotation.x, data.startRotation.y, data.startRotation.z, data.startRotation.w };
        j["endRotation"] = { data.endRotation.x, data.endRotation.y, data.endRotation.z, data.endRotation.w };
        j["useRotationCurve"] = data.useRotationCurve;

        // コンボ設定
        j["canComboToNext"] = data.canComboToNext;
        j["nextComboID"] = data.nextComboID;
        j["branchComboIDs"] = data.branchComboIDs;

        // エフェクト設定
        j["particleEffectName"] = data.particleEffectName;
        j["particleOffset"] = { data.particleOffset.x, data.particleOffset.y, data.particleOffset.z };
        j["cameraShakeIntensity"] = data.cameraShakeIntensity;
        j["cameraShakeDuration"] = data.cameraShakeDuration;

        // サウンド設定
        j["swingSoundName"] = data.swingSoundName;
        j["hitSoundName"] = data.hitSoundName;

        // プレイヤーの動き
        j["moveSpeedMultiplier"] = data.moveSpeedMultiplier;
        j["rootMotion"] = { data.rootMotion.x, data.rootMotion.y, data.rootMotion.z };

        // デバッグ
        j["showTrajectory"] = data.showTrajectory;
        j["trajectoryColor"] = { data.trajectoryColor.x, data.trajectoryColor.y, data.trajectoryColor.z };

        // ファイルに書き込み
        std::ofstream file(filepath);
        if (!file.is_open()) {
            return false;
        }
        file << j.dump(4); // インデント4でフォーマット
        file.close();

        return true;

    } catch (const std::exception&) {
        return false;
    }
}

///-------------------------------------------/// 
/// JSONファイルから攻撃データを読み込み
///-------------------------------------------///
bool AttackDataSerializer::LoadFromJson(AttackData& data, const std::string& filepath) {
    try {
        std::ifstream file(filepath);
        if (!file.is_open()) {
            return false;
        }

        json j;
        file >> j;
        file.close();

        // 基本情報
        data.attackName = j.value("attackName", "Unknown");
        data.description = j.value("description", "");
        data.attackID = j.value("attackID", 0);

        // タイミング設定
        data.activeDuration = j.value("activeDuration", 0.4f);
        data.comboWindowTime = j.value("comboWindowTime", 1.0f);
        data.cooldownTime = j.value("cooldownTime", 0.3f);

        // 軌道設定
        data.curveSegments = j.value("curveSegments", 20);
        data.weaponLength = j.value("weaponLength", 8.0f);

        // ベジェ曲線の制御点
        data.trajectoryPoints.clear();
        if (j.contains("trajectoryPoints")) {
            for (const auto& pointJson : j["trajectoryPoints"]) {
                BezierControlPointData point;
                point.position.x = pointJson["position"][0];
                point.position.y = pointJson["position"][1];
                point.position.z = pointJson["position"][2];
                point.time = pointJson["time"];
                data.trajectoryPoints.push_back(point);
            }
        }

        // 回転設定
        if (j.contains("startRotation")) {
            data.startRotation.x = j["startRotation"][0];
            data.startRotation.y = j["startRotation"][1];
            data.startRotation.z = j["startRotation"][2];
            data.startRotation.w = j["startRotation"][3];
        }
        if (j.contains("endRotation")) {
            data.endRotation.x = j["endRotation"][0];
            data.endRotation.y = j["endRotation"][1];
            data.endRotation.z = j["endRotation"][2];
            data.endRotation.w = j["endRotation"][3];
        }
        data.useRotationCurve = j.value("useRotationCurve", false);

        // コンボ設定
        data.canComboToNext = j.value("canComboToNext", false);
        data.nextComboID = j.value("nextComboID", -1);
        if (j.contains("branchComboIDs")) {
            data.branchComboIDs = j["branchComboIDs"].get<std::vector<int>>();
        }

        // エフェクト設定
        data.particleEffectName = j.value("particleEffectName", "");
        if (j.contains("particleOffset")) {
            data.particleOffset.x = j["particleOffset"][0];
            data.particleOffset.y = j["particleOffset"][1];
            data.particleOffset.z = j["particleOffset"][2];
        }
        data.cameraShakeIntensity = j.value("cameraShakeIntensity", 0.0f);
        data.cameraShakeDuration = j.value("cameraShakeDuration", 0.0f);

        // サウンド設定
        data.swingSoundName = j.value("swingSoundName", "");
        data.hitSoundName = j.value("hitSoundName", "");

        // プレイヤーの動き
        data.moveSpeedMultiplier = j.value("moveSpeedMultiplier", 0.4f);
        if (j.contains("rootMotion")) {
            data.rootMotion.x = j["rootMotion"][0];
            data.rootMotion.y = j["rootMotion"][1];
            data.rootMotion.z = j["rootMotion"][2];
        }

        // デバッグ
        data.showTrajectory = j.value("showTrajectory", true);
        if (j.contains("trajectoryColor")) {
            data.trajectoryColor.x = j["trajectoryColor"][0];
            data.trajectoryColor.y = j["trajectoryColor"][1];
            data.trajectoryColor.z = j["trajectoryColor"][2];
        }

        return true;

    } catch (const std::exception&) {
        return false;
    }
}
