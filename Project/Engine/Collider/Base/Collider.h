#pragma once
/// ===Include=== ///
// map
#include <map>
#include <string>
// Object3D
#include "application/Drawing/3d/Object3d.h"
// Data
#include "Engine/DataInfo/ColliderData.h"
// ColliderCollision
#include "Engine/Collider/Collision/ColliderCollision.h"
// Line
#include "application/Drawing/3d/Line.h"

/// ===前方宣言=== ///
class GameCamera;

///=====================================================/// 
/// コライダー
///=====================================================///
class Collider {
public:

    Collider() = default;
    virtual ~Collider() = default;

    /// <summary>
    /// 初期化処理
    /// </summary>
    virtual void Initialize();

    /// <summary>
    /// 更新処理
    /// </summary>
    virtual void Update();

    /// <summary>
    /// 描画処理
    /// </summary>
    /// <param name="mode">描画に使用するブレンドモードを指定します。</param>
    virtual void Draw(BlendMode mode);

    /// <summary>
	/// ImGui情報の更新
    /// </summary>
    virtual void Information();

public: /// ===衝突=== ///
    /// <summary>
    /// 衝突処理
    /// </summary>
    /// <param name="collider">衝突した相手のコライダーへのポインタ。該当するコライダー情報を示すオブジェクトへの参照。</param>
    virtual void OnCollision(Collider* collider) = 0;

public: /// ===親子関係=== ///
	// 親の設定
    void SetParent(ModelCommon* parent);
	// 親の解除
	void ClearParent();
	// ModelCommonの取得
	ModelCommon* GetModelCommon();

public: /// ===Setter=== ///
    // Transform
    void SetTranslate(const Vector3& translate);
    void SetRotate(const Quaternion& rotate);
    void SetScale(const Vector3& scale);
    // color
    void SetColor(const Vector4& color);
    // Light
    void SetLight(LightType type);
    void SetLightData(LightInfo light);
    // IsCollision
    void SetIsCollisison(bool flag);

public: /// ===Getter=== ///
    // Type
    ColliderType GetColliderType();
    // CollsisionName
    ColliderName GetColliderName();
    // Transform
    QuaternionTransform GetTransform() const;
    // Color
    Vector4 GetColor() const;
    // IsCollision
    bool GetIsCollision() const;

protected:
    /// ===Collider=== ///
    // Type
    ColliderType type_;
    // ColliderName
    ColliderName name_;

    /// ===Object=== ///
    // Object3D
    std::unique_ptr<Object3d> object3d_;
    // Transform
    QuaternionTransform transform_;
    // color
    Vector4 color_;

    /// ===Collision=== ///
    // 衝突フラグ
    bool isCollision_ = false;

    /// ===Debug=== ///
#ifdef _DEBUG
    // Line
    std::unique_ptr<Line> line_;
    Vector4 lineColor_ = { 1.0f, 1.0f, 1.0f, 1.0f };
#endif // _DEBUG
};

