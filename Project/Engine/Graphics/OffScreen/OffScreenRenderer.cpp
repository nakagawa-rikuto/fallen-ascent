#include "OffScreenRenderer.h"
// Effect
#include "Effect/CopyImageEffect.h"
#include "Effect/GrayscaleEffect.h"
#include "Effect/VignetteEffect.h"
#include "Effect/OutLineEffect.h"
#include "Effect/BoxFilter3x3Effect.h"
#include "Effect/BoxFilter5x5Effect.h"
#include "Effect/RadiusBlurEffect.h"
#include "Effect/DissolveEffect.h"
#include "Effect/ShatterGlassEffect.h"
// Service
#include "Service/Render.h"
// ImGui
#ifdef USE_IMGUI
#include <imgui.h>
#endif

namespace MiiEngine {
	///-------------------------------------------/// 
	/// デストラクタ
	///-------------------------------------------///
	OffScreenRenderer::~OffScreenRenderer() {
		activePass_ = nullptr;
		renderPass_.clear();
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
		// OffScreenTypeに対応するRenderPassを作成して登録
		renderPass_[OffScreenType::CopyImage] = std::make_unique<CopyImageEffect>();
		renderPass_[OffScreenType::Grayscale] = std::make_unique<GrayscaleEffect>();
		renderPass_[OffScreenType::Vignette] = std::make_unique<VignetteEffect>();
		renderPass_[OffScreenType::BoxFilter3x3] = std::make_unique<BoxFilter3x3Effect>();
		renderPass_[OffScreenType::BoxFilter5x5] = std::make_unique<BoxFilter5x5Effect>();
		renderPass_[OffScreenType::RadiusBlur] = std::make_unique<RadiusBlurEffect>();
		renderPass_[OffScreenType::OutLine] = std::make_unique<OutLineEffect>();
		renderPass_[OffScreenType::Dissolve] = std::make_unique<DissolveEffect>();
		renderPass_[OffScreenType::ShatterGlass] = std::make_unique<ShatterGlassEffect>();

		// 各RenderPassの初期化と入力テクスチャの設定
		for (auto& [type, pass] : renderPass_) {
			pass->Initialize(device, effectTexture_);
			pass->SetInputTexture(sceneTexture_);
		}

		// デフォルトのエフェクトはコピーイメージ	
		activePass_ = renderPass_[type_].get();
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

		// エフェクトの描画
		if (activePass_) {
			activePass_->Draw(commandList);
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
		// OffScreenTypeの名前配列
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

		// 現在のエフェクトタイプを整数に変換して表示
		int current = static_cast<int>(type_);
		if (ImGui::Begin("OffScreen Effect")) {
			ImGui::Text("Current Effect: %s", typeNames[current]);  // ここで名前表示

			if (ImGui::Combo("Effect Type", &current, typeNames, IM_ARRAYSIZE(typeNames))) {
				type_ = static_cast<OffScreenType>(current);
				SetType(type_);
			}
		}

		// エフェクトの詳細設定を表示
		if (activePass_) {
			activePass_->ImGuiInfo();
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
	RenderPass* OffScreenRenderer::GetRenderPass(OffScreenType type) const { return renderPass_.at(type).get();}

	/// ===Type=== ///
	OffScreenType OffScreenRenderer::GetType() { return type_; }

	///-------------------------------------------/// 
	/// Setter
	///-------------------------------------------///
	// Typeの設定
	void OffScreenRenderer::SetType(OffScreenType type) {
		// タイプを更新
		type_ = type;

		// タイプに対応するRenderPassをactivePass_に設定
		activePass_ = renderPass_[type].get();
	}
}