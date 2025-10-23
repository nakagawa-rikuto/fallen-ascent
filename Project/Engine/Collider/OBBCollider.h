#pragma once
/// ===Includ=== ///
#include "Engine/Collider/Base/Collider.h"

///-------------------------------------------/// 
/// OBBCollider
///-------------------------------------------///
class OBBCollider : public Collider {
public:

	OBBCollider() = default;
	~OBBCollider() = default;

    /// <summary>
    /// 初期化処理
    /// </summary>
    virtual void Initialize() override;

    /// <summary>
    /// 更新処理
    /// </summary>
    virtual void Update() override;

    /// <summary>
    /// 描画処理
    /// </summary>
    /// <param name="mode">描画モードを指定するBlendMode値。描画の合成やブレンド動作を決定します。</param>
    virtual void Draw(BlendMode mode) override;

    /// <summary>
	/// ImGui情報の更新
    /// </summary>
    virtual void Information()override;

public: /// ===衝突=== ///
    /// <summary>
    /// 衝突処理
    /// </summary>
    /// <param name="collider">衝突した相手の Collider へのポインタ。衝突対象に関する情報を表します。</param>
    virtual void OnCollision(Collider* collider) override = 0;

public: /// ===Getter=== ///
	// OBBの取得
    OBB GetOBB() const;

protected:
    OBB obb_;

private:
    /// <summary>
    /// OBBのAxisをQuaternionから設定
    /// </summary>
    /// <param name="obb">軸を設定する OBB への参照。関数内でこの OBB の軸が更新されます。</param>
    /// <param name="rotate">適用する回転を表すクォータニオン。OBB の軸計算に使用されます。</param>
    void SetOBBAxisFromQuaternion(OBB& obb, Quaternion rotate);
};

