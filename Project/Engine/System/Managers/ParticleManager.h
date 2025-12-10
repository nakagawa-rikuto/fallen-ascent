#pragma once
/// ===include=== ///
// c++
#include <map>
#include <memory>
#include <string>
#include <vector>
// ParticleGroup
#include "Engine/Graphics/Particle/ParticleGroup.h"
#include "Engine/Graphics/Particle/ParticleDefinition.h"
#include "Engine/Graphics/Particle/ParticleParameter.h"

/// ===前方宣言=== ///
class GameCamera;

///=====================================================/// 
/// ParticleManager（新方式のみ）
/// 定義ベースの管理システム
///=====================================================///
class ParticleManager {
public:
	ParticleManager() = default;
	~ParticleManager();

	/// ===定義の管理=== ///

	/// <summary>
	/// パーティクル定義の追加（JSONファイルから）
	/// </summary>
	/// <param name="jsonPath">JSONファイルのパス</param>
	/// <returns>読み込みに成功したかどうか</returns>
	void LoadParticleDefinition(const std::string& jsonPath);

	/// <summary>
	/// パーティクル定義の追加（直接指定）
	/// </summary>
	/// <param name="name">パーティクルの名前</param>
	/// <param name="definition">パーティクル定義</param>
	void AddParticleDefinition(const std::string& name, const ParticleDefinition& definition);

	/// <summary>
    /// パーティクルの発生処理
    /// </summary>
    /// <param name="name">発生させるパーティクルの名前</param>
    /// <param name="translate">発生させる場所</param>
    /// <returns>発生に成功したかどうか</returns>
	void Emit(const std::string& name, const Vector3& translate);

	/// <summary>
	/// パーティクル全体の更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// パーティクル全体の描画処理
	/// </summary>
	/// <param name="mode">ブレンドモード</param>
	void Draw(BlendMode mode = BlendMode::kBlendModeAdd);

	/// <summary>
	/// 指定した名前のアクティブなパーティクルをすべて停止
	/// </summary>
	/// <param name="name">停止するパーティクルの名前</param>
	void StopParticle(const std::string& name);

	/// <summary>
	/// パーティクル定義の削除
	/// </summary>
	/// <param name="name">削除するパーティクルの名前</param>
	void RemoveParticleDefinition(const std::string& name);

	/// <summary>
	/// すべてのアクティブなパーティクルを停止
	/// </summary>
	void RemoveAllParticles();

public: /// ===設定=== ///

	/// <summary>
	/// パーティクルのテクスチャを設定
	/// </summary>
	/// <param name="name">テクスチャを設定するパーティクルの名前</param>
	/// <param name="textureName">設定するテクスチャの名前</param>
	void SetTexture(const std::string& name, const std::string& textureName);

	/// <summary>
	/// 実行時パラメータ変更（型安全版）
	/// </summary>
	/// <param name="name">対象パーティクルの名前</param>
	/// <param name="param">変更するパラメータ</param>
	/// <param name="value">設定する値</param>
	void SetParameter(const std::string& name, ParticleParameter param, float value);

	/// <summary>
	/// アクティブなパーティクルのエミッタ位置を設定
	/// </summary>
	/// <param name="name">対象パーティクルの名前</param>
	/// <param name="position">新しい位置</param>
	/// <param name="groupIndex">グループのインデックス(-1で全グループ)</param>
	void SetEmitterPosition(const std::string& name, const Vector3& position, int groupIndex = -1);

	/// <summary>
	/// アクティブなパーティクルのエミッタ位置を移動
	/// </summary>
	/// <param name="name">対象パーティクルの名前</param>
	/// <param name="offset">移動量</param>
	/// <param name="groupIndex">グループのインデックス(-1で全グループ)</param>
	void MoveEmitterPosition(const std::string& name, const Vector3& offset, int groupIndex = -1);

	/// <summary>
	/// 指定された名前のエミッターの回転を設定します。
	/// </summary>
	/// <param name="name">設定対象のエミッターを識別する名前。</param>
	/// <param name="rotation">エミッターに適用する回転（Vector3で表現）。</param>
	/// <param name="groupIndex">適用先のグループインデックス。省略時は -1（グループ未指定または既定動作を示す）。</param>
	void SetEmitterRotation(const std::string& name, const Vector3& rotation, int groupIndex = -1);

public: /// ===情報取得=== ///

	/// <summary>
	/// パーティクル定義を取得
	/// </summary>
	/// <param name="name">取得するパーティクルの名前</param>
	/// <returns>パーティクル定義へのポインタ（存在しない場合はnullptr）</returns>
	const ParticleDefinition* GetDefinition(const std::string& name) const;

	/// <summary>
	/// パーティクル定義が登録されているか確認
	/// </summary>
	/// <param name="name">確認するパーティクルの名前</param>
	/// <returns>登録されている場合はtrue</returns>
	bool HasDefinition(const std::string& name) const;

	/// <summary>
	/// アクティブなパーティクル数を取得
	/// </summary>
	/// <param name="name">対象パーティクルの名前</param>
	/// <returns>アクティブなパーティクルの総数</returns>
	uint32_t GetActiveParticleCount(const std::string& name) const;

	/// <summary>
	/// 登録されているパーティクル定義の一覧を取得
	/// </summary>
	/// <returns>定義名のリスト</returns>
	std::vector<std::string> GetDefinitionNames() const;

	/// <summary>
	/// アクティブなパーティクルグループ数を取得
	/// </summary>
	/// <param name="name">対象パーティクルの名前</param>
	/// <returns>アクティブなグループ数</returns>
	size_t GetActiveGroupCount(const std::string& name) const;

	/// <summary>
	/// 特定のグループのエミッタ位置を取得
	/// </summary>
	/// <param name="name">対象パーティクルの名前</param>
	/// <param name="groupIndex">グループのインデックス(0-based)</param>
	/// <returns>エミッタ位置(グループが存在しない場合はゼロベクトル)</returns>
	Vector3 GetEmitterPosition(const std::string& name, size_t groupIndex = 0) const;

private:
	// 定義ベース
	std::map<std::string, ParticleDefinition> definitions_;

	// アクティブなパーティクル
	std::map<std::string, std::vector<std::unique_ptr<ParticleGroup>>> activeParticles_;
};