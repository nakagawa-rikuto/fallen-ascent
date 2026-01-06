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
	/// 初期化処理
	/// </summary>
	/// <param name="translate">発生位置</param>
	/// <param name="definition">パーティクル定義</param>
	void Initialze(const Vector3& translate, const ParticleDefinition& definition);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="mode">描画時の合成（ブレンド）モードを指定します。</param>
	void Draw(BlendMode mode);

	/// <summary>
	/// 停止処理
	/// </summary>
	void Stop();

	/// <summary>
	/// 生存しているかを判定する関数
	/// </summary>
	/// <returns>処理が完了している場合は true、そうでない場合は false を返します。</returns>
	bool IsFinish() const;

	/// <summary>
	/// 停止中かどうか
	/// </summary>
	/// <returns></returns>
	bool IsStopped() const;

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

	/// <summary>
	/// エミッタの位置を設定
	/// </summary>
	/// <param name="position">新しい位置</param>
	void SetEmitterPosition(const Vector3& position);

	/// <summary>
	/// エミッタの位置を移動(相対移動)
	/// </summary>
	/// <param name="offset">移動量</param>
	void MoveEmitterPosition(const Vector3& offset);

	/// <summary>
	/// エミッターの回転を設定します。
	/// </summary>
	/// <param name="rotate">設定する回転ベクトル。const Vector3& で参照渡しされます。</param>
	void SetEmitterRotate(const Vector3& rotate);

public: /// ===Getter=== ///

	/// <summary>
	/// 現在のパーティクル定義を取得
	/// </summary>
	const ParticleDefinition& GetDefinition() const;

	/// <summary>
	/// アクティブなパーティクル数を取得
	/// </summary>
	uint32_t GetActiveParticleCount() const;

	/// <summary>
	/// エミッタの現在位置を取得
	/// </summary>
	const Vector3& GetEmitterPosition() const;

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

		// 軌跡パーティクル用
		float trajectoryTimer = 0.0f;                // 軌跡タイマー
		Vector3 previouseEmitterPosition = { 0.0f, 0.0f, 0.0f }; // 前回のエミッタ位置
		Vector3 previouseEmitterRotation = { 0.0f, 0.0f, 0.0f }; // 前回のエミッタ回転
	};

	Group group_{};
	ParticleDefinition definition_;     // パーティクル定義
	float kDeltaTime_ = 1.0f / 60.0f;   // デルタタイム
	std::mt19937 randomEngine_;         // 乱数生成器

	bool isStopped_ = false;		// 停止フラグ

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

	/// <summary>
    /// 渦巻き運動を適用
    /// </summary>
    void ApplySwirlMotion(ParticleData& particle, float progress);

    /// <summary>
    /// エミッタの回転影響を適用
    /// </summary>
    void ApplyRotationInfluence(ParticleData& particle, float progress);
};