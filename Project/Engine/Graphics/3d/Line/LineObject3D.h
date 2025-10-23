#pragma once
/// ===Include=== ///
// Data
#include "Engine/DataInfo/LineObjectData.h"
#include "Engine/DataInfo/PipelineStateObjectType.h"
// Buffer
#include "Engine/Graphics/3d/Base/VertexBuffer3D.h"
#include "Engine/Graphics/3d/Base/Transform3D.h"
// C++
#include <memory>
#include <vector>

/// ===前方宣言=== ///
class GameCamera;

///=====================================================/// 
/// LineObject3D
///=====================================================///
class LineObject3D {
public:
	LineObject3D() = default;
	~LineObject3D();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="device">初期化に使用する Direct3D 12 デバイスへのポインタ。リソース作成やデバイス機能へのアクセスに使用される。</param>
	void Initialize(ID3D12Device* device);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 開始点から終点までの線の生成処理
	/// </summary>
	/// <param name="start">線の開始点。3次元座標を表す Vector3 型の参照。</param>
	/// <param name="end">線の終点。3次元座標を表す Vector3 型の参照。</param>
	/// <param name="color">線の色。4成分のベクトル（例: RGBA）を表す Vector4 型の参照。</param>
	void CreateLine(const Vector3& start, const Vector3& end, const Vector4& color);

	/// <summary>
	/// オブジェクトまたは状態を既定の（初期）状態にリセットします。
	/// </summary>
	void Reset();

public: /// ===Getter=== ///
	// ワールドTransformの取得
	const std::vector<Vector3> GetSphereData();

private:

	/// ===バッファリソース=== ///
	std::unique_ptr<VertexBuffer3D> vertex_;
	std::unique_ptr<Transform3D> wvp_;

	/// ===バッファビュー=== ///
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};

	/// ===バッファリソース内のデータを指すポインタ=== ///
	LineVertexData3D* vertexData_ = nullptr;
	LineTransformMatrixData3D* wvpData_ = nullptr;

	/// ===WorldTransform=== ///
	EulerTransform worldTransform_;
	EulerTransform cameraTransform_;

	/// ===Camera=== ///
	GameCamera* camera_ = nullptr;

	/// ===LineInfo=== ///
	uint32_t lineIndex_ = 0;
	const uint32_t kMaxLineCount_ = 100000;
	const uint32_t kLineVertexCount_ = 2;
	// 
	std::vector<Vector3> spheres_;

private:

	/// <summary>
	/// 球の頂点データを生成または初期化処理
	/// </summary>
	void SphereVertexData();
};

