#include "BufferBase.h"
// C++
#include <cassert>

namespace MiiEngine {
	///-------------------------------------------/// 
	/// デストラクタ
	///-------------------------------------------///
	BufferBase::~BufferBase() { buffer_.Reset(); }

	///-------------------------------------------/// 
	/// リソースの生成
	///-------------------------------------------///
	void BufferBase::Create(ID3D12Device* device, size_t sizeInBytes, DXGI_FORMAT format, D3D12_RESOURCE_FLAGS flags) {
		buffer_ = CreateBufferResourceComPtr(device, sizeInBytes, format, flags);
	}

	///-------------------------------------------/// 
	/// Setter
	///-------------------------------------------///
	void BufferBase::SetBuffer(ID3D12Resource* buffer) { buffer_ = buffer; }

	///-------------------------------------------/// 
	/// Getter
	///-------------------------------------------///
	ID3D12Resource* BufferBase::GetBuffer() { return buffer_.Get(); }
}