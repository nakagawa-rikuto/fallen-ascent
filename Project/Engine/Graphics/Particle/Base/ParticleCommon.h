#pragma once
/// ===Include=== ///
// Engine
#include "Engine/DataInfo/CData.h"
#include "Engine/DataInfo/ParticleData.h"
#include "Engine/Core/ComPtr.h"
#include "Engine/Graphics/3d/base/Material3D.h"
// c++
#include <memory>
#include <list>

/// ===前方宣言=== ///
class SRVManager;

///=====================================================/// 
/// ParticleSetUp
///=====================================================///
class ParticleCommon {
public:

	ParticleCommon();
	~ParticleCommon();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="device">初期化に使用する ID3D12Device へのポインタ。</param>
	/// <param name="kNumMaxInstance">初期化時に確保または許容する最大インスタンス数（uint32_t）。</param>
	void Initlize(ID3D12Device* device, const uint32_t kNumMaxInstance);

	/// <summary>
	/// 描画準備処理
	/// </summary>
	/// <param name="commandList">バインド先のID3D12GraphicsCommandListへのポインター。コマンドの記録に使用されます。</param>
	void Bind(ID3D12GraphicsCommandList* commandList);

public: /// ===Geter=== ///

	// Instancingの取得
	ID3D12Resource* GetInstancing();

public: /// ===Setter=== ///

	// Material
	void SetMatiarlData(const Vector4& color, const Matrix4x4& uvTransform);
	// Instancing
	void SetInstancingData(size_t index, const Vector4& color, const Matrix4x4& WVP, const Matrix4x4& World);

private:

	/// ===バッファリソース=== ///
	std::unique_ptr<Material3D> material_;
	ComPtr<ID3D12Resource> instancing_;
	
	/// ===バッファリソース内のデータを指すポインタ=== ///
	MaterialData3D* materialData_ = nullptr;
	ParticleForGPU* instancingData_ = nullptr;

private:

	/// <summary>
	/// Bufferの生成処理
	/// </summary>
	/// <param name="device">リソース作成に使用する ID3D12Device へのポインタ。</param>
	/// <param name="sizeInBytes">作成するリソースのサイズ（バイト単位）。</param>
	void Create(ID3D12Device* device, size_t sizeInBytes);
};

