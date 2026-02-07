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

	///=====================================================/// 
	/// RTVManager
	///=====================================================///
	class RTVManager {
	public:

		RTVManager() = default;
		~RTVManager();

		/// <summary>
		/// 初期化処理
		/// </summary>
		/// <param name="dxcommon">初期化に使用する DXCommon オブジェクトへのポインター。nullptr を渡した場合の動作は呼び出し側で確認してください。</param>
		void Initialize(DXCommon* dxcommon);

		/// <summary>
		/// RTV（RenderTargetView）の作成
		/// </summary>
		/// <param name="index">レンダーターゲットのスロットまたは位置を示すインデックス。</param>
		/// <param name="resource">レンダーターゲットとして使用する ID3D12Resource へのポインター。nullptr であってはなりません。</param>
		/// <param name="desc">ビューのフォーマット、ミップスライス、配列スライスなどを指定する D3D12_RENDER_TARGET_VIEW_DESC 構造体。</param>
		void CreateRenderTargetView(uint32_t index, ID3D12Resource* resource, const D3D12_RENDER_TARGET_VIEW_DESC& desc);

		/// <summary>
		/// RTV（RenderTargetView）のクリア
		/// </summary>
		/// <param name="commandList">クリア操作を記録する ID3D12GraphicsCommandList へのポインタ。</param>
		/// <param name="index">クリアするレンダーターゲットのインデックス（スロット番号）。</param>
		/// <param name="color">RGBA 順の 4 要素配列で指定するクリア色。各要素は通常 0.0 から 1.0 の浮動小数点値です。</param>
		void ClearRenderTargetView(ID3D12GraphicsCommandList* commandList, uint32_t index, const float color[4]);

		/// <summary>
		/// リソースの割り当てを行い、割り当てられた識別子を返す
		/// </summary>
		/// <returns>割り当てられたリソースの識別子を表す 32 ビット符号なし整数 (uint32_t)。</returns>
		uint32_t Allocate();

		/// <summary>
		/// メモリ割り当ての成否を検証
		/// </summary>
		/// <returns>アロケーションが成功した場合は true、失敗した場合は false を返します。</returns>
		bool AssertAllocate() const;

		/// <summary>
		/// スワップチェーン用のレンダーターゲットを作成
		/// </summary>
		void CreateSwapChainRenderTargetView();

	public: /// ===Getter=== ///
		// Heap
		ID3D12DescriptorHeap* GetDescriptorHeap() const;
		// CPU
		D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index) const;
		// GPU
		D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index) const;

	public:/// ===定数=== ///

		// RTVの数
		static const uint32_t kMaxRTVCount_ = 16;

	private:
		// DXCommon
		DXCommon* dxcommon_ = nullptr;
		// ヒープ
		ComPtr<ID3D12DescriptorHeap> descriptorHeap_;
		// デスクリプタハンドル
		D3D12_CPU_DESCRIPTOR_HANDLE descriptorHandles_[kMaxRTVCount_];
		// スクリプタサイズ
		uint32_t descriptorSize_ = 0;
		// 次に使用するSRVインデックス
		uint32_t useIndex_ = 0;
	};
}

