#pragma once
/// ===Include=== ///
#include "Engine/Graphics/OffScreen/RenderPass.h"

///=====================================================/// 
/// OutLineEffect
///=====================================================///
class OutLineEffect : public RenderPass {
public:
	OutLineEffect() = default;
	~OutLineEffect() = default;
	
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="device">初期化に使用する D3D12 デバイスへのポインタ。GPU リソースの作成やコマンド発行に使用されます。</param>
	/// <param name="RenderTexture">初期化で使用するレンダーテクスチャへの std::shared_ptr。レンダリングの出力先や関連リソースを表します。</param>
	void Initialize(ID3D12Device* device, std::shared_ptr<RenderTexture> RenderTexture) override;
	
	/// <summary>
	/// 描画前処理
	/// </summary>
	/// <param name="commandList">描画コマンドを記録するための ID3D12GraphicsCommandList へのポインター。GPU コマンドの発行に使用されます。</param>
	/// <param name="dsvHandle">深度ステンシルビュー (DSV) を指す D3D12_CPU_DESCRIPTOR_HANDLE。描画前に DSV をバインドまたは設定するために使用されます。</param>
	void PreDraw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) override;
	
	/// <summary>
	/// 描画処理
	/// </summary>
	/// <param name="commandList">描画コマンドを記録する ID3D12GraphicsCommandList へのポインター。nullptr を渡さないでください。</param>
	void Draw(ID3D12GraphicsCommandList* commandList) override;
	
	/// <summary>
	/// ImGui情報の表示
	/// </summary>
	void ImGuiInfo() override;
};