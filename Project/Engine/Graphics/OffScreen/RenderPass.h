#pragma once
#include "Engine/Graphics/OffScreen/RenderTexture.h"
#include <memory>

///=====================================================/// 
/// RenderPass 
///=====================================================///
class RenderPass {
public:
	RenderPass() = default;
	virtual ~RenderPass() = default;

	/// <summary>
	/// 初期化処理、純粋仮想関数
	/// </summary>
	/// <param name="device">初期化に使用する Direct3D 12 デバイスへのポインタ（ID3D12Device*）。</param>
	/// <param name="RenderTexture">初期化対象のレンダーテクスチャを指す std::shared_ptr。レンダリング先や関連リソースを表します。</param>
	virtual void Initialize(ID3D12Device* device, std::shared_ptr<RenderTexture> RenderTexture) = 0;

	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="commandList">描画コマンドを記録するための ID3D12GraphicsCommandList へのポインタ。</param>
	/// <param name="dsvHandle">深度ステンシルビュー (DSV) の CPU 側ディスクリプタハンドル。深度ステンシル設定に使用されます。</param>
	virtual void PreDraw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle);

	/// <summary>
	/// 描画処理、純粋仮想関数
	/// </summary>
	/// <param name="commandList">Direct3D 12 の ID3D12GraphicsCommandList へのポインター。描画コマンドをこのコマンドリストに記録します。</param>
	virtual void Draw(ID3D12GraphicsCommandList* commandList) = 0;

	/// <summary>
	/// ImGui情報の表示
	/// </summary>
	virtual void ImGuiInfo() = 0;

protected:
	std::shared_ptr<RenderTexture> renderTexture_;
};

 