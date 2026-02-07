#include "PostEffect.h"
// c++
#include <cassert>
// Engine
#include "Engine/Graphics/OffScreen/OffScreenRenderer.h"
// Locator
#include "Locator.h"

using namespace MiiEngine;
namespace Service {
	///-------------------------------------------/// 
	/// Setter
	///-------------------------------------------///
	// OffScreenTypeの設定
	void PostEffect::SetOffScreenType(MiiEngine::OffScreenType type) {
		return Locator::GetOffScreenRenderer()->SetType(type);
	}
	// Dissolve
	void PostEffect::SetDissolveData(MiiEngine::DissolveData data) {
		if (Locator::GetOffScreenRenderer()->GetType() != OffScreenType::Dissolve) return;
		return Locator::GetOffScreenRenderer()->GetDissolve()->SetData(data);
	}
	// RadiusBlur
	void PostEffect::SetRadiusBlurData(MiiEngine::RadiusBlurData data) {
		if (Locator::GetOffScreenRenderer()->GetType() != OffScreenType::RadiusBlur) return;
		return Locator::GetOffScreenRenderer()->GetRadiusBlur()->SetData(data);
	}
	// Vignette
	void PostEffect::SetVignetteData(MiiEngine::VignetteData data) {
		if (Locator::GetOffScreenRenderer()->GetType() != OffScreenType::Vignette) return;
		return Locator::GetOffScreenRenderer()->GetVignette()->SetData(data);
	}
	// ShatterGlass
	void PostEffect::SetShatterGlassData(MiiEngine::ShatterGlassData data) {
		if (Locator::GetOffScreenRenderer()->GetType() != OffScreenType::ShatterGlass) return;
		return Locator::GetOffScreenRenderer()->GetShatterGlass()->SetData(data);
	}
}