#pragma once
/// ===incled=== ///
#include "ParticleSetUp.h"
#include <list>
#include <random>

/// ===Camera=== ///
class GameCamera;

///=====================================================/// 
/// パーティクルのグループ
///=====================================================///
class ParticleGroup {
public:

	ParticleGroup() = default;
	virtual ~ParticleGroup();

	/// <summary>
	/// 初期化処理、純粋仮想関数
	/// </summary>
	/// <param name="translate">適用する平行移動（オフセット）を表す 3 次元ベクトル。const 参照で渡され、呼び出し側の値は変更されないことを意図している。</param>
	virtual void Initialze(const Vector3& translate) = 0;

	/// <summary>
	/// 更新処理、純粋仮想関数
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画処理、純粋仮想関数
	/// </summary>
	/// <param name="mode">描画時の合成（ブレンド）モードを指定します。</param>
	virtual void Draw(BlendMode mode);
	
	/// <summary>
	/// モデルのインスタンシングに対する初期化処理
	/// </summary>
	/// <param name="modelName">初期化するモデルの名前または識別子。</param>
	/// <param name="translate">インスタンスの基準となる平行移動ベクトル（配置位置のオフセット）。</param>
	/// <param name="maxInstance">許可する最大インスタンス数。</param>
	/// <param name="type">インスタンスの形状タイプ。省略時は shapeType::kNone。</param>
	void InstancingInit(const std::string& modelName, const Vector3& translate, const uint32_t maxInstance, shapeType type = shapeType::kNone);

	/// <summary>
	/// モデルのインスタンシングに対する更新処理
	/// </summary>
	/// <param name="it">更新対象の ParticleData を指す std::list<ParticleData>::iterator。該当要素を参照して更新処理を行います。</param>
	void InstancingUpdate(std::list<ParticleData>::iterator it);

	/// <summary>
	/// 生存しているかを判定する関数
	/// </summary>
	/// <returns>処理が完了している場合は true、そうでない場合は false を返します。</returns>
	bool IsFinish();

public: /// ===Setter=== ///
	// Texture
	void SetTexture(const std::string& fileName);

protected:
	/// ===Emitter=== ///
	struct Group {
		std::unique_ptr<ParticleSetUp> particle; // パーティクルグループ
		std::list<ParticleData> particles; // パーティクルのリスト
		EulerTransform transform; // エミッタのTransform
		EulerTransform cameraTransform; // カメラのTransform
		uint32_t maxInstance; // パーティクルの最大数
		uint32_t numInstance; // インスタンス数
		uint32_t frequencyCount; // パーティクルの発生頻度のカウント
		float frequency; // パーティクルの発生頻度
		float frequencyTime; // パーティクルの発生頻度の時間
		GameCamera* camera = nullptr;
	};

	// エミッタ
	Group group_{};
	// 時間の進む速度
	const float kDeltaTime_ = 1.0f / 60.0f;

	/// ===プライベートフィールド=== ///
	std::mt19937 randomEngine_; // 乱数生成器

public:

	/// <summary>
	/// クローン処理、純粋仮想関数
	/// </summary>
	/// <returns>新しく作成された ParticleGroup オブジェクトへの所有権を表す std::unique_ptr<ParticleGroup> を返します。</returns>
	virtual std::unique_ptr<ParticleGroup> Clone() = 0;

	/// <summary>
	/// ランダムなパーティクルの生成処理, 純粋仮想関数
	/// </summary>
	/// <param name="randomEngine">粒子のランダムな特性（例：速度、寿命、色のばらつき）を生成するための std::mt19937 の参照。呼び出しにより内部状態が更新される可能性がある。</param>
	/// <param name="translate">生成される粒子に適用する並進（位置）ベクトル。変更されないことを示す const 参照で渡される。</param>
	/// <returns>生成された ParticleData オブジェクト。新しい粒子の位置や速度、その他のプロパティを含む。</returns>
	virtual ParticleData MakeParticle(std::mt19937& randomEngine, const Vector3& translate) = 0;
	
	/// <summary>
	/// パーティクルの発生処理, 純粋仮想関数
	/// </summary>
	/// <param name="group">パーティクル生成の対象となるグループへの定数参照。関数内では変更されません。</param>
	/// <param name="randomEngine">パーティクルのランダム性を生成するための std::mt19937 への参照。関数内で状態が更新される可能性があります。</param>
	/// <returns>生成された各パーティクルの情報を格納した std::list<ParticleData>。</returns>
	virtual std::list<ParticleData> Emit(const Group& group, std::mt19937& randomEngine) = 0;
};

