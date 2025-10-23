#pragma once
/// ===include=== ///
#include "Engine/DataInfo/FunctionData.h"

///=====================================================/// 
/// Light
///=====================================================///
class BufferBase {
public:
	BufferBase() = default;
	~BufferBase();

	/// <summary>
	/// バイト単位で指定したサイズのリソースの生成処理
	/// </summary>
	/// <param name="device">リソースの作成に使用する ID3D12Device へのポインタ。</param>
	/// <param name="sizeInBytes">作成するリソースのサイズ（バイト単位）。</param>
	void Create(ID3D12Device* device, size_t sizeInBytes);

public:/// ===Getter=== ///

	// バッファリソースの取得
	ID3D12Resource* GetBuffer();

private:/// ===Variables(変数)=== ///

	ComPtr<ID3D12Resource> buffer_;
};
