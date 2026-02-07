#include "BoxFilter5x5Effect.h"
// Service
#include "Engine/System/Service/Render.h"

namespace MiiEngine {
	///-------------------------------------------/// 
	/// 初期化
	///-------------------------------------------///
	void BoxFilter5x5Effect::Initialize(ID3D12Device* device, std::shared_ptr<RenderTexture> RenderTexture) {
		// RenderTextureを取得
		outputTexture_ = RenderTexture;

		// Deviceの初期化
		ID3D12Device* devicePtr = device;
		devicePtr;
	}

	///-------------------------------------------/// 
	/// 描画前処理
	///-------------------------------------------///
	void BoxFilter5x5Effect::PreDraw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) {
		RenderPass::PreDraw(commandList, dsvHandle);
	}

	///-------------------------------------------/// 
	/// ImGuiの描画
	///-------------------------------------------///
	void BoxFilter5x5Effect::Draw(ID3D12GraphicsCommandList* commandList) {
		// コピーイメージ
		Service::Render::SetPSO(commandList, PipelineType::BoxFilter5x5, BlendMode::kBlendModeNone);

		commandList->SetGraphicsRootDescriptorTable(0, inputTexture_->GetSRVHandle());
		// 頂点3つを描画
		commandList->DrawInstanced(3, 1, 0, 0);
	}

	///-------------------------------------------/// 
	/// ImGui情報
	///-------------------------------------------///
	void BoxFilter5x5Effect::ImGuiInfo() {

	}
}