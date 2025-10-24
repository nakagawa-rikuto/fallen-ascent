#pragma once
/// ===include=== ///
// c++
#include <memory>
#include <string>
// Math
#include "Math/Vector3.h"

/// ===前方宣言=== ///
class ParticleGroup;
class Camera;

///=====================================================/// 
/// ParticleService
///=====================================================///
class ParticleService {
public:

    /// <summary>
    /// 指定した名前で粒子グループを追加します。渡された std::unique_ptr の所有権は関数に移動します。
    /// </summary>
    /// <param name="name">追加する粒子グループの名前。</param>
    /// <param name="particle">追加する ParticleGroup オブジェクトへの std::unique_ptr。呼び出し時に所有権が関数に移ります。</param>
    static void AddParticle(const std::string& name, std::unique_ptr<ParticleGroup> particle);

    /// <summary>
    /// 指定された名前のパーティクルを削除します。
    /// </summary>
    /// <param name="name">削除対象のパーティクルの名前。</param>
    static void RemoveParticle(const std::string& name);

    /// <summary>
    /// 指定された名前と3次元ベクトルに基づいてエミット処理を行う静的関数。
    /// </summary>
    /// <param name="name">エミット対象の名前を表す文字列参照。</param>
    /// <param name="translate">エミット時の位置または移動を表す3次元ベクトル（Vector3）。</param>
    static void Emit(const std::string& name, const Vector3& translate);

    /// <summary>
    /// 指定された名前に対してテクスチャを設定する静的関数。
    /// </summary>
    /// <param name="name">テクスチャを設定する対象の名前。</param>
    /// <param name="textureName">適用するテクスチャの名前。</param>
    static void SetTexture(const std::string& name, const std::string& textureName);
};
