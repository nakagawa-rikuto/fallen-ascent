#pragma once
/// ===Include=== ///
#include "ParticleDefinition.h"
#include "Engine/DataInfo/ParticleData.h"
// c++
#include <random>
#include <list>

///=====================================================/// 
/// パーティクルファクトリー
/// ParticleDefinitionから動的にパーティクルを生成
///=====================================================///
class ParticleFactory {
public:

    /// <summary>
    /// 単一のパーティクルデータを生成
    /// </summary>
    /// <param name="definition">パーティクルの定義データ</param>
    /// <param name="randomEngine">乱数生成器</param>
    /// <param name="position">発生位置</param>
    /// <returns>生成されたパーティクルデータ</returns>
    static ParticleData CreateParticle(
        const ParticleDefinition& definition,
        std::mt19937& randomEngine,
        const Vector3& position);

    /// <summary>
    /// パーティクルのバッチ生成（バーストモード用）
    /// </summary>
    /// <param name="definition">パーティクルの定義データ</param>
    /// <param name="randomEngine">乱数生成器</param>
    /// <param name="position">発生位置</param>
    /// <param name="count">生成数</param>
    /// <returns>生成されたパーティクルのリスト</returns>
    static std::list<ParticleData> CreateParticleBurst(
        const ParticleDefinition& definition,
        std::mt19937& randomEngine,
        const Vector3& position,
        uint32_t count);

    /// <summary>
    /// パーティクルの更新処理（汎用ロジック）
    /// </summary>
    /// <param name="particle">更新するパーティクルデータ</param>
    /// <param name="definition">パーティクルの定義データ</param>
    /// <param name="deltaTime">デルタタイム</param>
    /// <param name="randomEngine">乱数生成器（回転用）</param>
    static void UpdateParticle(
        ParticleData& particle,
        const ParticleDefinition& definition,
        float deltaTime,
        std::mt19937& randomEngine);

private:

    /// <summary>
    /// ランダムな速度を生成
    /// </summary>
    static Vector3 GenerateRandomVelocity(
        const ParticleDefinition& definition,
        std::mt19937& randomEngine,
        const Vector3& position);

    /// <summary>
    /// ランダムなスケールを生成
    /// </summary>
    static Vector3 GenerateRandomScale(
        const ParticleDefinition& definition,
        std::mt19937& randomEngine);

    /// <summary>
    /// ランダムな回転速度を生成
    /// </summary>
    static Vector3 GenerateRandomRotationSpeed(
        const ParticleDefinition& definition,
        std::mt19937& randomEngine);

    /// <summary>
    /// ランダムな寿命を生成
    /// </summary>
    static float GenerateRandomLifetime(
        const ParticleDefinition& definition,
        std::mt19937& randomEngine);

    /// <summary>
    /// 時間経過による色の補間
    /// </summary>
    static Vector4 LerpColor(
        const Vector4& start,
        const Vector4& end,
        float t);

    /// <summary>
    /// 時間経過によるスケールの補間
    /// </summary>
    static Vector3 LerpScale(
        const Vector3& start,
        const Vector3& end,
        float t);
};