#include "OffScreenService.h"
// c++
#include <cassert>
// Engine
#include "Engine/Graphics/OffScreen/OffScreenRenderer.h"
// ServiceLocator
#include "ServiceLocator.h"

using namespace MiiEngine;
namespace Service {
	///-------------------------------------------/// 
	/// Setter
	///-------------------------------------------///
	// OffScreenTypeの設定
	void OffScreenService::SetOffScreenType(MiiEngine::OffScreenType type) {
		return ServiceLocator::GetOffScreenRenderer()->SetType(type);
	}
	// Dissolve
	void OffScreenService::SetDissolveData(MiiEngine::DissolveData data) {
		if (ServiceLocator::GetOffScreenRenderer()->GetType() != OffScreenType::Dissolve) return;
		return ServiceLocator::GetOffScreenRenderer()->GetDissolve()->SetData(data);
	}
	// RadiusBlur
	void OffScreenService::SetRadiusBlurData(MiiEngine::RadiusBlurData data) {
		if (ServiceLocator::GetOffScreenRenderer()->GetType() != OffScreenType::RadiusBlur) return;
		return ServiceLocator::GetOffScreenRenderer()->GetRadiusBlur()->SetData(data);
	}
	// Vignette
	void OffScreenService::SetVignetteData(MiiEngine::VignetteData data) {
		if (ServiceLocator::GetOffScreenRenderer()->GetType() != OffScreenType::Vignette) return;
		return ServiceLocator::GetOffScreenRenderer()->GetVignette()->SetData(data);
	}
	// ShatterGlass
	void OffScreenService::SetShatterGlassData(MiiEngine::ShatterGlassData data) {
		if (ServiceLocator::GetOffScreenRenderer()->GetType() != OffScreenType::ShatterGlass) return;
		return ServiceLocator::GetOffScreenRenderer()->GetShatterGlass()->SetData(data);
	}
}