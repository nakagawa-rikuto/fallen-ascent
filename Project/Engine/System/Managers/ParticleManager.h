#pragma once
/// ===include=== ///
// c++
#include <map>
#include <memory>
#include <string>
// PartlceGroup
#include "Engine/Graphics/Particle/Base/ParticleGroup.h"

/// ===前方宣言=== ///
class Camera;

///=====================================================/// 
/// ParticleManager
///=====================================================///
class ParticleManager {
public:
	ParticleManager() = default;
	~ParticleManager();

    /// <summary>
    /// パーティクルの追加処理
    /// </summary>
    /// <param name="name">追加するパーティクルの名前</param>
    /// <param name="particle">追加するパーティクル</param>
    void AddParticle(const std::string& name, std::unique_ptr<ParticleGroup> particle);

    /// <summary>
    /// パーティクルの発生処理
    /// </summary>
    /// <param name="name">発生させるパーティクルの名前</param>
    /// <param name="translate">発生させる場所</param>
    void Emit(const std::string& name, const Vector3& translate);

    /// <summary>
    /// パーティクルのテクスチャを設定
    /// </summary>
    /// <param name="name">テクスチャを設定するパーティクルの名前</param>
    /// <param name="textureName">設定するテクスチャの名前</param>
    void SetTexture(const std::string& name, const std::string& textureName);

    /// <summary>
    /// パーティクルの削除処理
    /// </summary>
    /// <param name="name">削除するパーティクルの名前</param>
    void RemoveParticle(const std::string& name);

    /// <summary>
    /// パーティクル全体の更新処理
    /// </summary>
    void Update();

    /// <summary>
    /// パーティクル全体の描画処理
    /// </summary>
    /// <param name="mode">ブレンドモード</param>
    void Draw(BlendMode mode = BlendMode::kBlendModeAdd);

private:
    // 登録されたテンプレート
    std::map<std::string, std::unique_ptr<ParticleGroup>> prototype_;
    // 実際に動いているパーティクル
    std::map<std::string, std::vector<std::unique_ptr<ParticleGroup>>> activeParticles_;
};

