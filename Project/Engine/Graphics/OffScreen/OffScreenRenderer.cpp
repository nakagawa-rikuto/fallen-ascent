#include "OffScreenRenderer.h"
// Service
#include "Engine/System/Service/Render.h"
// ImGui
#ifdef USE_IMGUI
#include <imgui.h>
#endif

namespace MiiEngine {
	///-------------------------------------------/// 
	/// デストラクタ
	///-------------------------------------------///
	OffScreenRenderer::~OffScreenRenderer() {
		copyImage_.reset();
		grayscale_.reset();
		vignette_.reset();
		boxFilter3x3_.reset();
		boxFilter5x5_.reset();
		radiusBlur_.reset();
		outLine_.reset();
		dissolve_.reset();
		shatterGlass_.reset();
		sceneTexture_.reset();
		effectTexture_.reset();
	}

	///-------------------------------------------/// 
	/// 初期化
	///-------------------------------------------///
	void OffScreenRenderer::Initialize(
		ID3D12Device* device,
		SRVManager* srv, RTVManager* rtv,
		uint32_t width, uint32_t height, const Vector4& clearColor) {

		// シーン描画用のRenderTextureを初期化
		sceneTexture_ = std::make_shared<RenderTexture>();
		sceneTexture_->Initialize(srv, rtv, width, height, clearColor, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
		sceneTexture_->CreateRenderTexture(device);

		// エフェクト適用後のRenderTextureを初期化
		effectTexture_ = std::make_shared<RenderTexture>();
		effectTexture_->Initialize(srv, rtv, width, height, clearColor, DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
		effectTexture_->CreateRenderTexture(device);

		/// ===RenderPassの登録=== ///
		// SceneRenderPass
		copyImage_ = std::make_shared<CopyImageEffect>();
		copyImage_->Initialize(device, effectTexture_);
		copyImage_->SetInputTexture(sceneTexture_);

		// グレースケールレンダーパス
		grayscale_ = std::make_shared<GrayscaleEffect>();
		grayscale_->Initialize(device, effectTexture_);
		grayscale_->SetInputTexture(sceneTexture_);

		// ビネットレンダーパス
		vignette_ = std::make_shared<VignetteEffect>();
		vignette_->Initialize(device, effectTexture_);
		vignette_->SetInputTexture(sceneTexture_);

		// Dissolveエフェクト
		dissolve_ = std::make_shared<DissolveEffect>();
		dissolve_->Initialize(device, effectTexture_);
		dissolve_->SetInputTexture(sceneTexture_);

		// 3x3ボックスフィルタレンダーパス
		boxFilter3x3_ = std::make_shared<BoxFilter3x3Effect>();
		boxFilter3x3_->Initialize(device, effectTexture_);
		boxFilter3x3_->SetInputTexture(sceneTexture_);

		// 5x5ボックスフィルタレンダーパス
		boxFilter5x5_ = std::make_shared<BoxFilter5x5Effect>();
		boxFilter5x5_->Initialize(device, effectTexture_);
		boxFilter5x5_->SetInputTexture(sceneTexture_);

		// 半径ブラーエフェクト
		radiusBlur_ = std::make_shared<RadiusBlurEffect>();
		radiusBlur_->Initialize(device, effectTexture_);
		radiusBlur_->SetInputTexture(sceneTexture_);

		// OutLineエフェクト
		outLine_ = std::make_shared<OutLineEffect>();
		outLine_->Initialize(device, effectTexture_);
		outLine_->SetInputTexture(sceneTexture_);

		// ShatterGlassエフェクト
		shatterGlass_ = std::make_shared<ShatterGlassEffect>();
		shatterGlass_->Initialize(device, effectTexture_);
		shatterGlass_->SetInputTexture(sceneTexture_);
	}

	///-------------------------------------------/// 
	/// 描画前処理
	///-------------------------------------------///
	void OffScreenRenderer::PreDraw(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle) {

		// シーン描画はsceneTexture_へ
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandel = sceneTexture_->GetRTVHandle();
		commandList->OMSetRenderTargets(1, &rtvHandel, false, &dsvHandle);
		// RenderTextureのクリア
		sceneTexture_->Clear(commandList);

		/*switch (type_) {
		case OffScreenType::CopyImage:
			copyImage_->PreDraw(commandList, dsvHandle);
			break;
		case OffScreenType::Grayscale:
			grayscale_->PreDraw(commandList, dsvHandle);
			break;
		case OffScreenType::Vignette:
			vignette_->PreDraw(commandList, dsvHandle);
			break;
		case OffScreenType::BoxFilter3x3:
			boxFilter3x3_->PreDraw(commandList, dsvHandle);
			break;
		case OffScreenType::BoxFilter5x5:
			boxFilter5x5_->PreDraw(commandList, dsvHandle);
			break;
		case OffScreenType::RadiusBlur:
			radiusBlur_->PreDraw(commandList, dsvHandle);
			break;
		case OffScreenType::OutLine:
			outLine_->PreDraw(commandList, dsvHandle);
			break;
		case OffScreenType::Dissolve:
			dissolve_->PreDraw(commandList, dsvHandle);
			break;
		case OffScreenType::ShatterGlass:
			shatterGlass_->PreDraw(commandList, dsvHandle);
			break;
		}*/
	}

	///-------------------------------------------/// 
	/// 描画処理
	///-------------------------------------------///
	void OffScreenRenderer::Draw(ID3D12GraphicsCommandList* commandList) {
		// effectTexture_ への描画先を設定
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = effectTexture_->GetRTVHandle();
		commandList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);

		// effectTexture_ をクリア
		effectTexture_->Clear(commandList);

		switch (type_) {
		case OffScreenType::CopyImage:
			copyImage_->Draw(commandList);
			break;
		case OffScreenType::Grayscale:
			grayscale_->Draw(commandList);
			break;
		case OffScreenType::Vignette:
			vignette_->Draw(commandList);
			break;
		case OffScreenType::BoxFilter3x3:
			boxFilter3x3_->Draw(commandList);
			break;
		case OffScreenType::BoxFilter5x5:
			boxFilter5x5_->Draw(commandList);
			break;
		case OffScreenType::RadiusBlur:
			radiusBlur_->Draw(commandList);
			break;
		case OffScreenType::OutLine:
			outLine_->Draw(commandList);
			break;
		case OffScreenType::Dissolve:
			dissolve_->Draw(commandList);
			break;
		case OffScreenType::ShatterGlass:
			shatterGlass_->Draw(commandList);
			break;
		}
	}

	///-------------------------------------------/// 
	/// 描画後処理
	///-------------------------------------------///
	void OffScreenRenderer::PostDraw(ID3D12GraphicsCommandList* commandList) {
		commandList;
	}

	///-------------------------------------------/// 
	/// effectTexture を SwapChain にコピー
	///-------------------------------------------///
	void OffScreenRenderer::CopyToSwapChain(ID3D12GraphicsCommandList* commandList, D3D12_CPU_DESCRIPTOR_HANDLE swapChainRTV) {
		// SwapChain の RTV を描画先として設定
		commandList->OMSetRenderTargets(1, &swapChainRTV, false, nullptr);

		// CopyImage エフェクトを使用して effectTexture_ を SwapChain にコピー
		Service::Render::SetPSO(commandList, PipelineType::OffScreen, BlendMode::kBlendModeNone);
		commandList->SetGraphicsRootDescriptorTable(0, effectTexture_->GetSRVHandle());
		commandList->DrawInstanced(3, 1, 0, 0);
	}

	///-------------------------------------------/// 
	/// ImGuiの描画
	///-------------------------------------------///
#ifdef USE_IMGUI
	void OffScreenRenderer::DrawImGui() {
		const char* typeNames[] = {
			"CopyImage",
			"Grayscale",
			"Vignette",
			"BoxFilter3x3",
			"BoxFilter5x5",
			"RadiusBlur",
			"OutLine",
			"Dissolve",
			"ShatterGlass",
		};

		int current = static_cast<int>(type_);
		if (ImGui::Begin("OffScreen Effect")) {
			ImGui::Text("Current Effect: %s", typeNames[current]);  // ← ★ ここで名前表示

			if (ImGui::Combo("Effect Type", &current, typeNames, IM_ARRAYSIZE(typeNames))) {
				type_ = static_cast<OffScreenType>(current);
			}
		}

		switch (type_) {
		case OffScreenType::CopyImage:
			copyImage_->ImGuiInfo();
			break;
		case OffScreenType::Grayscale:
			grayscale_->ImGuiInfo();
			break;
		case OffScreenType::Vignette:
			vignette_->ImGuiInfo();
			break;
		case OffScreenType::BoxFilter3x3:
			boxFilter3x3_->ImGuiInfo();
			break;
		case OffScreenType::BoxFilter5x5:
			boxFilter5x5_->ImGuiInfo();
			break;
		case OffScreenType::RadiusBlur:
			radiusBlur_->ImGuiInfo();
			break;
		case OffScreenType::OutLine:
			outLine_->ImGuiInfo();
			break;
		case OffScreenType::Dissolve:
			dissolve_->ImGuiInfo();
			break;
		case OffScreenType::ShatterGlass:
			shatterGlass_->ImGuiInfo();
			break;
		}

		ImGui::End();
	}
#endif // USE_IMGUI

	///-------------------------------------------/// 
	/// Getter
	///-------------------------------------------///
	// RTV
	D3D12_CPU_DESCRIPTOR_HANDLE OffScreenRenderer::GetResultRTV() const { return effectTexture_->GetRTVHandle(); }
	// SRV
	D3D12_GPU_DESCRIPTOR_HANDLE OffScreenRenderer::GetResultSRV() const { return effectTexture_->GetSRVHandle(); }
	// RTVIndex
	uint32_t OffScreenRenderer::GetRTVHandleIndex() const { return effectTexture_->GetRTVHandleIndex(); }
	// SRVIndex
	uint32_t OffScreenRenderer::GetSRVHandleIndex() const { return effectTexture_->GetSRVHandleIndex(); }
	// Resource
	ID3D12Resource* OffScreenRenderer::GetSceneBuffer() const { return sceneTexture_->GetBuffer(); }
	ID3D12Resource* OffScreenRenderer::GetEffectBuffer() const { return effectTexture_->GetBuffer(); }

	/// ===Effect=== ///
	CopyImageEffect* OffScreenRenderer::GetCopyImage() { return copyImage_.get(); }
	GrayscaleEffect* OffScreenRenderer::GetGrayscale() { return grayscale_.get(); }
	VignetteEffect* OffScreenRenderer::GetVignette() { return vignette_.get(); }
	OutLineEffect* OffScreenRenderer::GetOutLine() { return outLine_.get(); }
	BoxFilter3x3Effect* OffScreenRenderer::GetBoxFilter3x3() { return boxFilter3x3_.get(); }
	BoxFilter5x5Effect* OffScreenRenderer::GetBoxFilter5x5() { return boxFilter5x5_.get(); }
	RadiusBlurEffect* OffScreenRenderer::GetRadiusBlur() { return radiusBlur_.get(); }
	DissolveEffect* OffScreenRenderer::GetDissolve() { return dissolve_.get(); }
	ShatterGlassEffect* OffScreenRenderer::GetShatterGlass() { return shatterGlass_.get(); }

	/// ===Type=== ///
	OffScreenType OffScreenRenderer::GetType() { return type_; }

	///-------------------------------------------/// 
	/// Setter
	///-------------------------------------------///
	// Typeの設定
	void OffScreenRenderer::SetType(OffScreenType type) { type_ = type; }
}