#pragma once
/// ===Icnlde=== ///
// Buffer
#include "Engine/Graphics/3d/base/VertexBuffer3D.h"
#include "ParticleCommon.h"
// Pipleine
#include "Engine/DataInfo/PipelineStateObjectType.h"
// c++
#include <memory>

/// ===前方宣言=== ///
class Camera;
class SRVManager;

/// ===選択肢=== ///
enum class shapeType {
	// 円
	kCircle,
	// Cylinder
	kCylinder,
	// 総数(これは最後にしなければいけない)
	kNone,
};

///=====================================================/// 
/// Particle共通描画設定
///=====================================================///
class ParticleSetUp {
public:

	ParticleSetUp();
	~ParticleSetUp();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="filename">初期化に使用するファイルのパスまたは名前（const std::string&）。</param>
	/// <param name="kNumMaxInstance">許可される最大インスタンス数（const uint32_t）。</param>
	/// <param name="type">初期化対象の形状の種類を示す shapeType 値。</param>
	void Initialze(const std::string& filename, const uint32_t kNumMaxInstance, shapeType type);

	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="instance">描画対象を示す uint32_t 型の識別子。関数内で変更されない（const）。</param>
	/// <param name="mode">描画時に使用するブレンドモードを指定する（BlendMode 型の値）。</param>
	void Darw(const uint32_t instance, BlendMode mode);

public: /// ===Setter=== ///

	// テクスチャ
	void SetTexture(const std::string& fileName);
	// Material
	void SetMatiarlData(const Vector4& color, const Matrix4x4& uvTransform);
	// Instancing
	void SetInstancingData(size_t index, const Vector4& color, const Matrix4x4& WVP, const Matrix4x4& World);

private:
	/// ===SRV=== ///
	struct ParticleSRV {
		SRVManager* srvManager_ = nullptr;
		uint32_t srvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};
	ParticleSRV srvData_;

	/// ===バッファリソース=== ///
	std::unique_ptr<VertexBuffer3D> vertex_;
	std::unique_ptr<ParticleCommon> common_;

	/// ===バッファリソース内のデータを指すポインタ=== ///
	VertexData3D* vertexData_ = nullptr;

	/// ===バッファビュー=== ///
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	
	/// ===モデルデータ=== ///
	ModelData modelData_;

	/// ===モデル情報=== ///
	uint32_t kNumMaxInstance_;

private:
	
	/// <summary>
	/// 円形の頂点バッファを設定
	/// </summary>
	void SetVertexBufferCircle();

	/// <summary>
	/// 円柱の頂点バッファを設定
	/// </summary>
	void SetVertexBufferCylinder();
};

