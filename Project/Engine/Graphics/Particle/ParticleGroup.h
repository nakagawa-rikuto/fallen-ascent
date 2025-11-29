#pragma once
/// ===Include=== ///
#include "ParticleSetUp.h"
#include "ParticleDefinition.h"
#include "ParticleParameter.h"
#include "Engine/DataInfo/ParticleData.h"
// c++
#include <list>
#include <random>
#include <memory>

/// ===前方宣言=== ///
class GameCamera;

///=====================================================/// 
/// パーティクルのグループ
///=====================================================///
class ParticleGroup {
public:

	ParticleGroup() = default;
	~ParticleGroup();

	/// <summary>
	/// 初期化処理（パラメータ駆動型）
	/// </summary>
	/// <param name="translate">発生位置</param>
	/// <param name="definition">パーティクル定義</param>
	void Initialze(const Vector3& translate, const ParticleDefinition& definition);

	/// <summary>
	/// 更新処理（汎用ロジック）
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="mode">描画時の合成（ブレンド）モードを指定します。</param>
	void Draw(BlendMode mode);

	/// <summary>
	/// 生存しているかを判定する関数
	/// </summary>
	/// <returns>処理が完了している場合は true、そうでない場合は false を返します。</returns>
	bool IsFinish() const;

public: /// ===Setter=== ///

	/// <summary>
	/// テクスチャ設定
	/// </summary>
	void SetTexture(const std::string& fileName);

	/// <summary>
	/// パーティクル定義を設定（実行時変更用）
	/// </summary>
	void SetDefinition(const ParticleDefinition& definition);

	/// <summary>
	/// 実行時パラメータ変更（型安全版）
	/// </summary>
	/// <param name="param">変更するパラメータ</param>
	/// <param name="value">設定する値</param>
	void SetParameter(ParticleParameter param, float value);

public: /// ===Getter=== ///

	/// <summary>
	/// 現在のパーティクル定義を取得
	/// </summary>
	const ParticleDefinition& GetDefinition() const;

	/// <summary>
	/// アクティブなパーティクル数を取得
	/// </summary>
	uint32_t GetActiveParticleCount() const;

private:

	/// ===内部構造体=== ///
	struct Group {
		std::unique_ptr<ParticleSetUp> particle;     // パーティクル描画セットアップ
		std::list<ParticleData> particles;           // パーティクルのリスト
		EulerTransform transform;                    // エミッタのTransform
		EulerTransform cameraTransform;              // カメラのTransform（フォールバック用）
		uint32_t maxInstance;                        // パーティクルの最大数
		uint32_t numInstance;                        // 現在のインスタンス数
		float frequencyTimer;                        // 発生頻度タイマー
		GameCamera* camera = nullptr;
		bool hasEmitted;                             // 初回発生済みフラグ（バースト用）
	};

	Group group_{};
	ParticleDefinition definition_;                  // パーティクル定義
	float kDeltaTime_ = 1.0f / 60.0f;               // デルタタイム
	std::mt19937 randomEngine_;                      // 乱数生成器

private:

	/// <summary>
	/// モデルのインスタンシングに対する初期化処理
	/// </summary>
	void InstancingInit(const std::string& modelName, const Vector3& translate, const uint32_t maxInstance, shapeType type);

	/// <summary>
	/// モデルのインスタンシングに対する更新処理
	/// </summary>
	void InstancingUpdate(std::list<ParticleData>::iterator it);

	/// <summary>
	/// パーティクル生成（パラメータ駆動型）
	/// </summary>
	ParticleData MakeParticle(const Vector3& translate);

	/// <summary>
	/// パーティクル発生処理
	/// </summary>
	void Emit();

	/// <summary>
	/// パーティクルの更新処理
	/// </summary>
	void UpdateParticles();
};