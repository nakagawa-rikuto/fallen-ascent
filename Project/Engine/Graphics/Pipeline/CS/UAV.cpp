#include "UAV.h"
// Engine
#include "Engine/Core/DXCommon.h"

///-------------------------------------------/// 
/// UAVの生成処理
///-------------------------------------------///
void UAV::Create(DXCommon* dxCommon, PipelineType Type, UINT numElements, UINT structureByteStride) {

	// デスクの設定
	desc_.Format = DXGI_FORMAT_UNKNOWN;
	desc_.ViewDimension = D3D12_UAV_DIMENSION_BUFFER;
	desc_.Buffer.FirstElement = 0;
	desc_.Buffer.NumElements = numElements;
	desc_.Buffer.CounterOffsetInBytes = 0;
	desc_.Buffer.Flags = D3D12_BUFFER_UAV_FLAG_NONE;
	desc_.Buffer.StructureByteStride = structureByteStride;

	dxCommon->GetDevice()->CreateUnorderedAccessView(resource_.Get(), nullptr, &desc_, );
}

///-------------------------------------------/// 
/// Getter
///-------------------------------------------///
D3D12_UNORDERED_ACCESS_VIEW_DESC UAV::GetUAVDesc() const { return desc_; }