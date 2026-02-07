#pragma once
/// ===Include=== ///
// DirectX
#include <d3d12.h>
// c++
#include <cstdint>
#include <cassert>
// Engine
#include "Engine/Core/ComPtr.h"

namespace MiiEngine {
	/// ===前方宣言=== ///
	class DXCommon;
	class WinApp;

	///=====================================================/// 
	/// DSVManager
	///=====================================================///
	class DSVManager {
	public:

		DSVManager();
		~DSVManager();

		/// <summary>
		/// 初期化処理
		/// </summary>
		/// <param name="dxcommon">Class DXCommon</param>
		void Initialize(DXCommon* dxcommon, WinApp* winApp);

		/// <summary>
		/// 深度バッファの生成処理
		/// </summary>
		/// <param name="index">インデックス</param>
		void CreateDepthBufferView(uint32_t index);

		/// <summary>
		/// クリア処理
		/// </summary>
		/// <param name="commandList">コマンドリスト</param>
		void ClearDepthBufferView(ID3D12GraphicsCommandList* commandList);

		/// <summary>
		/// リソースの割り当てを行い、割り当てられた識別子を返す
		/// </summary>
		/// <returns>割り当てられたリソースの識別子を表す 32 ビット符号なし整数 (uint32_t)。</returns>
		uint32_t Allocate();

		/// <summary>
		/// メモリ割り当ての成否を検証
		/// </summary>
		/// <returns>アロケーションが成功した場合は true、失敗した場合は false を返します。</returns>
		bool AssertAllocate();

	public: /// ===Getter=== ///
		// Heap
		ID3D12DescriptorHeap* GetDescriptorHeap() const;
		// CPU
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index) const;
		// GPU
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index) const;

	public:/// ===定数=== ///

		// DSVの数
		static const uint32_t kMaxDSVCount_ = 16;

	private:
		// DXCommon
		DXCommon* dxcommon_ = nullptr;
		// WinApp
		WinApp* winApp_ = nullptr;
		// depthStencilResource
		ComPtr<ID3D12Resource> resource_[kMaxDSVCount_];
		// ヒープ
		ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
		// スクリプタサイズ
		uint32_t descriptorSize_ = 0;
		// 次に使用するSRVインデックス
		uint32_t useIndex_ = 0;
	};
}

