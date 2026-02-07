#include "FunctionData.h"
// C++
#include <cassert>

namespace MiiEngine {
	///-------------------------------------------/// 
	/// CreateBufferResource
	///-------------------------------------------///
	ID3D12Resource* CreateBufferResource(
		ID3D12Device* device,
		size_t sizeInBytes,
		DXGI_FORMAT format,
		D3D12_RESOURCE_FLAGS flags) {

		HRESULT hr;

		// リソース用のヒープの設定
		D3D12_HEAP_PROPERTIES uploadHeapProperties{};
		uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;  // UploadHeapを使う

		// リソースの設定
		D3D12_RESOURCE_DESC resourceDesc{};

		// バッファリソース。テクスチャの場合はまた別の設定をする
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Width = sizeInBytes; // リソースのサイズ。今回はVector4を3頂点分

		// バッファの場合はこれらは1にする決まり
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.SampleDesc.Count = 1;

		// Formatの設定
		resourceDesc.Format = format;
		// Flagsの設定
		resourceDesc.Flags = flags;

		// バッファの場合はこれにする決まり
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 実際に頂点リソースを作る
		ID3D12Resource* bufferResource = nullptr;
		hr = device->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&bufferResource)
		);
		assert(SUCCEEDED(hr));

		return bufferResource;
	}

	///-------------------------------------------/// 
	/// CreateBufferResource(ComPtr)
	///-------------------------------------------///
	ComPtr<ID3D12Resource> CreateBufferResourceComPtr(
		ID3D12Device* device,
		size_t sizeInBytes,
		DXGI_FORMAT format,
		D3D12_RESOURCE_FLAGS flags) {

		HRESULT hr;

		// リソース用のヒープの設定
		D3D12_HEAP_PROPERTIES uploadHeapProperties{};
		uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;  // UploadHeapを使う

		// リソースの設定
		D3D12_RESOURCE_DESC resourceDesc{};

		// バッファリソース。テクスチャの場合はまた別の設定をする
		resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resourceDesc.Width = sizeInBytes; // リソースのサイズ。今回はVector4を3頂点分

		// バッファの場合はこれらは1にする決まり
		resourceDesc.Height = 1;
		resourceDesc.DepthOrArraySize = 1;
		resourceDesc.MipLevels = 1;
		resourceDesc.SampleDesc.Count = 1;

		// Formatの設定
		resourceDesc.Format = format;
		// Flagsの設定
		resourceDesc.Flags = flags;

		// バッファの場合はこれにする決まり
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		// 実際に頂点リソースを作る
		ComPtr<ID3D12Resource> bufferResource = nullptr;
		hr = device->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&bufferResource)
		);
		assert(SUCCEEDED(hr));

		return bufferResource;
	}
}