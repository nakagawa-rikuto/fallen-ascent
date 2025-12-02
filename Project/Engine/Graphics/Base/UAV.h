/// ===Include=== ///
// Engien
#include "Engine/Core/ComPtr.h"
// directX
#include <d3d12.h>

///=====================================================/// 
/// UAV
///=====================================================///
class UAV {
public:
	UAV() = default;
	~UAV() = default;

	/// <summary>
	/// UAV（Unordered Access View）を作成します。
	/// </summary>
	/// <param name="dxCommon">DirectX共通管理クラスへのポインタ</param>
	/// <param name="targetResource">UAVとして使用するリソース（既に作成済み）</param>
	/// <param name="numElements">バッファの要素数</param>
	/// <param name="structureByteStride">1要素あたりのバイトサイズ</param>
	/// <param name="cpuHandle">DescriptorHeapのCPUハンドル（UAVを登録する場所）</param>
	void Create(ID3D12Device* device, ID3D12Resource* targetResorce, UINT numElements, UINT structureByteStride, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle);

	/// <summary>
	/// UAVの取得
	/// </summary>
	/// <returns></returns>
	D3D12_UNORDERED_ACCESS_VIEW_DESC GetUAVDesc() const;

	/// <summary>
	/// 内部で保持している ID3D12Resource の生ポインタを取得します。
	/// </summary>
	/// <returns>管理下にある ID3D12Resource への生ポインタ。所有権は移動しません。リソースが存在しない場合は nullptr を返すことがあります。</returns>
	ID3D12Resource* GetResource() const;

private:
	// resource
	ID3D12Resource* resource_ = nullptr;
	// Desc
	D3D12_UNORDERED_ACCESS_VIEW_DESC desc_{};
};