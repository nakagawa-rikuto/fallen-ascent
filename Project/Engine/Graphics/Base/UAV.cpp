#include "UAV.h"

///-------------------------------------------/// 
/// UAVの生成処理
///-------------------------------------------///
void UAV::Create(ID3D12Device* device, ID3D12Resource* targetResorce, UINT numElements, UINT structureByteStride, D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle) {

	// リソースを保存
	resource_ = targetResorce;

	// デスクの設定
	desc_.Format = DXGI_FORMAT_UNKNOWN;
	desc_.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	desc_.Buffer.FirstElement = 0;
	desc_.Buffer.NumElements = numElements;
	desc_.Buffer.CounterOffsetInBytes = 0;
	desc_.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	desc_.Buffer.StructureByteStride = structureByteStride;

	device->CreateUnorderedAccessView(resource_, nullptr, &desc_, cpuHandle);
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
D3D12_UNORDERED_ACCESS_VIEW_DESC UAV::GetUAVDesc() const { return desc_; }
ID3D12Resource* UAV::GetResource() const { return resource_; }
