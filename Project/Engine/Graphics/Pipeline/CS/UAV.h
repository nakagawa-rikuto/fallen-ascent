/// ===Include=== ///
// Engien
#include "Engine/Core/ComPtr.h"
// PipelineType
#include "Engine/DataInfo/PipelineStateObjectType.h"
// directX
#include <d3d12.h>
#include <unordered_map>

/// ===前方宣言=== ///
class DXCommon;

///=====================================================/// 
/// UAV
///=====================================================///
class UAV {
public:
	UAV() = default;
	~UAV() = default;

	/// <summary>
	/// UAV（Unordered Access View）用のバッファリソースとビューを作成します。
	/// </summary>
	/// <param name="dxCommon">DirectX共通管理クラスへのポインタ。リソース作成やデバイス取得に使用します。</param>
	/// <param name="Type">パイプラインの種類（PipelineType）。用途に応じたUAVの設定に利用します。</param>
	/// <param name="numElements">UAVバッファの要素数。格納するデータの個数を指定します。</param>
	/// <param name="structureByteStride">1要素あたりのバイトサイズ。構造体バッファの場合は構造体のサイズを指定します。</param>
	void Create(DXCommon* dxCommon, PipelineType Type, UINT numElements, UINT structureByteStride);

	/// <summary>
	/// UAVの取得
	/// </summary>
	/// <returns></returns>
	D3D12_UNORDERED_ACCESS_VIEW_DESC GetUAVDesc() const;

private:
	// resource
	ComPtr<ID3D12Resource> resource_;
	// Desc
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc_{};
};