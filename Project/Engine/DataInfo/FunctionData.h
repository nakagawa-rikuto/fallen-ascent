#pragma once
#include "Engine/Core/ComPtr.h"
#include "Engine/DataInfo/CData.h"

/// <summary>
/// 指定したデバイス上でバイト単位のサイズとオプションのフォーマット／フラグを使用してバッファ用の ID3D12Resource を作成します。
/// </summary>
/// <param name="device">リソースを作成する Direct3D 12 デバイスへのポインタ。</param>
/// <param name="sizeInBytes">作成するバッファのサイズ（バイト単位）。</param>
/// <param name="format">リソースの DXGI フォーマット。既定は DXGI_FORMAT_UNKNOWN。型付きビューが必要な場合などに指定します。</param>
/// <param name="flags">リソース作成時の D3D12_RESOURCE_FLAGS（例: UAV 許可など）。既定は D3D12_RESOURCE_FLAG_NONE。</param>
/// <returns>作成された ID3D12Resource へのポインタ。作成に失敗した場合は nullptr を返す可能性があります。返されたリソースの参照解放（Release）は呼び出し側の責任です。</returns>
ID3D12Resource* CreateBufferResource(
	ID3D12Device* device, 
	size_t sizeInBytes, 
	DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, 
	D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);

/// <summary>
/// 指定したサイズとオプションのフォーマット／フラグでバッファ用の ID3D12Resource を作成し、ComPtr<ID3D12Resource> として返します。
/// </summary>
/// <param name="device">リソースを作成するために使用する ID3D12Device のポインタ。</param>
/// <param name="sizeInBytes">作成するバッファのサイズ（バイト単位）。</param>
/// <param name="format">リソースのフォーマットを指定します。既定値は DXGI_FORMAT_UNKNOWN（未指定）。</param>
/// <param name="flags">リソースの作成に使用する D3D12_RESOURCE_FLAGS。既定値は D3D12_RESOURCE_FLAG_NONE。</param>
/// <returns>作成された ID3D12Resource を所有する ComPtr。作成に失敗した場合は nullptr になることがあります。</returns>
ComPtr<ID3D12Resource> CreateBufferResourceComPtr(
	ID3D12Device* device, 
	size_t sizeInBytes, 
	DXGI_FORMAT format = DXGI_FORMAT_UNKNOWN, 
	D3D12_RESOURCE_FLAGS flags = D3D12_RESOURCE_FLAG_NONE);

