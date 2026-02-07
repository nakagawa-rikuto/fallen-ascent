#pragma once
#include <string>

namespace MiiEngine {
    ///=====================================================/// 
    /// パーティクルパラメータ列挙型
    /// SetParameter用の型安全なパラメータ指定
    ///=====================================================///
    enum class ParticleParameter {
        // 物理パラメータ
        Gravity,               // 重力
        ExplosionRadiusX,      // 発生範囲
        ExplosionRadiusY,      // 発生範囲
        ExplosionRadiusZ,      // 発生範囲
        UpwardForce,           // 上方向の力
        AccelerationX,         // 加速度X
        AccelerationY,         // 加速度Y
        AccelerationZ,         // 加速度Z

        // 発生パラメータ
        EmissionRate,          // 発生レート
        Frequency,             // 発生頻度
        LifetimeMin,           // 最小寿命
        LifetimeMax,           // 最大寿命
        BurstCount,            // バースト数

        // 見た目パラメータ
        StartColorR,           // 開始色R
        StartColorG,           // 開始色G
        StartColorB,           // 開始色B
        StartColorA,           // 開始色A
        EndColorR,             // 終了色R
        EndColorG,             // 終了色G
        EndColorB,             // 終了色B
        EndColorA,             // 終了色A

        // 回転パラメータ
        RotationSpeedX,        // 回転速度X
        RotationSpeedY,        // 回転速度Y
        RotationSpeedZ,        // 回転速度Z

        // 速度パラメータ
        VelocityMinX,          // 最小速度X
        VelocityMinY,          // 最小速度Y
        VelocityMinZ,          // 最小速度Z
        VelocityMaxX,          // 最大速度X
        VelocityMaxY,          // 最大速度Y
        VelocityMaxZ,          // 最大速度Z
    };

    ///=====================================================/// 
    /// パーティクルパラメータユーティリティ
    ///=====================================================///
    class ParticleParameterUtils {
    public:

        /// <summary>
        /// enum classを文字列に変換（内部処理用）
        /// </summary>
        /// <param name="param">変換するパラメータ</param>
        /// <returns>パラメータ名の文字列</returns>
        static std::string ToString(ParticleParameter param);

        /// <summary>
        /// 文字列からenum classに変換（デバッグ用）
        /// </summary>
        /// <param name="str">変換する文字列</param>
        /// <param name="outParam">変換結果の出力先</param>
        /// <returns>変換に成功した場合はtrue</returns>
        static bool FromString(const std::string& str, ParticleParameter& outParam);

        /// <summary>
        /// パラメータの説明を取得（UI表示用）
        /// </summary>
        /// <param name="param">説明を取得するパラメータ</param>
        /// <returns>パラメータの説明文</returns>
        static std::string GetDescription(ParticleParameter param);
    };
}