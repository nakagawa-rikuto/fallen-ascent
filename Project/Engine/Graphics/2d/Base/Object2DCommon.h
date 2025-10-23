#pragma once
/// ===include=== ///
// Engine
#include "Material2D.h"
#include "Transform2D.h"
//c++
#include <memory>

///=====================================================/// 
/// Object2D共通部
///=====================================================///
class Object2DCommon {
public:

	Object2DCommon();
	~Object2DCommon();

	/// <summary>
	/// マテリアルの初期化処理
	/// </summary>
	/// <param name="device">初期化に使用する Direct3D 12 デバイスへの ID3D12Device ポインタ。</param>
	/// <param name="Size">初期化で使用するサイズ（バイト数や要素数など、実装に依存します）。</param>
	void MaterialInitialize(ID3D12Device* device, uint32_t Size);

	/// <summary>
	/// WVP（ワールド・ビュー・プロジェクション）行列に関する内部リソースの初期化処理
	/// </summary>
	/// <param name="device">WVP 行列の初期化に使用する Direct3D 12 デバイスへのポインタ。</param>
	void WVPMatrixInitialize(ID3D12Device* device);

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="commandList">バインド対象のID3D12GraphicsCommandListへのポインタ。バインド処理に使用されます。</param>
	void Bind(ID3D12GraphicsCommandList* commandList);

public:/// ===Setter=== ///
	// Mateial
	void SetMateiralData(const Vector4& color, const Matrix4x4& uvTransform);
	// WVP
	void SetWVPData(const Matrix4x4& WVP);

private:/// ===Variables(変数)=== ///

	// バッファリソース
	std::unique_ptr<Material2D> material_;
	std::unique_ptr<Transform2D> wvp_;

	// バッファリソース内のデータを指すポインタ
	MaterialData2D* materialData_ = nullptr;
	TransformationMatrix2D* wvpMatrixData_ = nullptr;
};


