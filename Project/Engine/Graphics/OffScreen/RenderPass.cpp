#include "RenderPass.h"

///-------------------------------------------/// 
/// 描画
///-------------------------------------------///
void RenderPass::PreDraw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) {

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandel = outputTexture_->GetRTVHandle();
	commandList->OMSetRenderTargets(1, &rtvHandel, false, &dsvHandle);

	// RenderTextureのクリア
	outputTexture_->Clear(commandList);
}

///-------------------------------------------/// 
/// Setter
///-------------------------------------------///
void RenderPass::SetInputTexture(std::shared_ptr<RenderTexture> inputTexture) { inputTexture_ = inputTexture; }
