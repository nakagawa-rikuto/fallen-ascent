#pragma once
/// ===Include=== ///
#include "Engine/Graphics/3d/Model/Model.h"
#include "Engine/Graphics/3d/Model/AnimationModel.h"

/// ===オブジェクトのタイプ=== ///
enum class ObjectType {
	Model,
	AnimationModel
};

///=====================================================/// 
/// Object3D
///=====================================================///
class Object3d {
public:
	Object3d() = default;
	~Object3d();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="type">初期化するオブジェクトの種類を指定します。</param>
	/// <param name="modelName">使用するモデルの名前（識別またはロードに使われる文字列）。</param>
	/// <param name="light">照明の種類を指定します。省略時は LightType::None（照明なし）が使用されます。</param>
	void Init(ObjectType type, const std::string& modelName, MiiEngine::LightType light = MiiEngine::LightType::None);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="mode">描画に使用するブレンドモード。省略した場合は BlendMode::KBlendModeNormal が使用される。</param>
	void Draw(MiiEngine::BlendMode mode = MiiEngine::BlendMode::KBlendModeNormal);

public: /// ===親子関係=== ///
	/// <summary>
	/// ModelCommon 型の親オブジェクトを設定
	/// </summary>
	/// <param name="parent">設定する親オブジェクトへのポインタ。</param>
	void SetParent(MiiEngine::ModelCommon* parent);

	/// <summary>
	/// 親オブジェクトへの参照をクリア
	/// </summary>
	void ClearParent();

	// 親からのオフセットを設定
	void SetParentOffset(const Vector3& offset);
	// 親からのオフセットを取得
	const Vector3& GetParentOffset() const;

public: /// ===Getter=== ///
	// Transform（位置、回転、拡縮）を取得
	const Vector3& GetWorldTranslate() const;
	const Quaternion& GetWorldRotate() const;
	const Vector3& GetWorldScale() const;
	const MiiEngine::QuaternionTransform& GetWorldTransform() const;
	// Color（色）を取得
	const Vector4& GetColor() const;
	// ModelCommonオブジェクトへのポインターを取得
	MiiEngine::ModelCommon* GetModelCommon();

public: /// ===Setter=== ///
	// モデルTransform
	void SetTranslate(const Vector3& translate);
	void SetRotate(const Quaternion& rotate);
	void SetScale(const Vector3& scale);
	// モデルColor
	void SetColor(const Vector4& color);
	// Light
	void SetLight(MiiEngine::LightType type);
	// LightData
	void SetLightData(MiiEngine::LightInfo light);
	// 環境マップ
	void SetEnvironmentMapData(bool flag, float string);
	// Animation
	void SetAnimation(const std::string& animationName, bool isLoop = true);

private: /// ===Variables(変数)=== ///

	/// ===クラス=== ///
	std::unique_ptr<MiiEngine::Model> model_;
	std::unique_ptr<MiiEngine::AnimationModel> animationModel_;

	/// ===モデルかアニメーションモデルかを判断するフラグ=== ///
	ObjectType type_ = ObjectType::Model;
};

